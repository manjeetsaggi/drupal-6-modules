<?php
// $Id: apachesolr.index.inc,v 1.1.2.6.2.18 2010/01/02 23:44:04 robertDouglass Exp $

/**
 * @file
 *   Functions used when indexing content to Apache Solr.
 */

/**
 * Strip html tags and also control characters that cause Jetty/Solr to fail.
 */
function apachesolr_clean_text($text) {
  // Add spaces before stripping tags to avoid running words together.
  $text = filter_xss(str_replace(array('<', '>'), array(' <', '> '), $text), array());
  // Decode entities and then make safe any < or > characters.
  return htmlspecialchars(html_entity_decode($text, ENT_NOQUOTES, 'UTF-8'), ENT_NOQUOTES, 'UTF-8');
}

/**
 * Given a node, return a document representing that node.
 */
function apachesolr_node_to_document($node, $namespace) {
  $document = FALSE;
  // Let any module exclude this node from the index.
  $build_document = TRUE;
  foreach (module_implements('apachesolr_node_exclude') as $module) {
    $exclude = module_invoke($module, 'apachesolr_node_exclude', $node, $namespace);
    if (!empty($exclude)) {
      $build_document = FALSE;
    }
  }

  if ($build_document) {
    // Build the node body.
    $node->build_mode = NODE_BUILD_SEARCH_INDEX;
    $node = node_build_content($node, FALSE, FALSE);
    $node->body = drupal_render($node->content);
    $node->title = apachesolr_clean_text($node->title);

    $text = $node->body;

    // Fetch extra data normally not visible, including comments.
    // We do this manually (with module_implements instead of node_invoke_nodeapi)
    // because we want a keyed array to come back. Only in this way can we decide
    // whether to index comments or not.
    $extra = array();
    $exclude_comments = in_array($node->type, variable_get('apachesolr_exclude_comments_types', array()), TRUE);
    foreach (module_implements('nodeapi') as $module) {
      if ($exclude_comments && $module == 'comment') {
        // Don't add comments.
        continue;
      }
      if ($output = module_invoke($module, 'nodeapi', $node, 'update index', FALSE, FALSE)) {
        $extra[$module] = $output;
      }

    }
    if (!variable_get('apachesolr_index_comments_with_node', TRUE)) {
      unset($extra['comment']);
    }

    $text .= "\n\n" . implode(' ', $extra);

    $document = new Apache_Solr_Document();
    $document->id = apachesolr_document_id($node->nid);
    $document->site = url(NULL, array('absolute' => TRUE));
    $document->hash = apachesolr_site_hash();
    $document->entity = 'node';
    $document->nid = $node->nid;
    $document->uid = $node->uid;
    $document->title = $node->title;
    $document->status = $node->status;
    $document->sticky = $node->sticky;
    $document->promote = $node->promote;
    $document->moderate = $node->moderate;
    $document->tnid = $node->tnid;
    $document->translate = $node->translate;
    if (!empty($node->language)) {
      $document->language = $node->language;
    }
    else {
      // 'und' is the language-neutral code in Drupal 7.
      // Note - this was changed from 'zxx'.
      $document->language = 'und';
    }
    $document->body = apachesolr_clean_text($text);
    $document->type  = $node->type;
    $document->type_name = node_get_types('name', $node);
    $document->created = apachesolr_date_iso($node->created);
    $document->changed = apachesolr_date_iso($node->changed);
    $last_change = (isset($node->last_comment_timestamp) && $node->last_comment_timestamp > $node->changed) ? $node->last_comment_timestamp : $node->changed;
    $document->last_comment_or_change = apachesolr_date_iso($last_change);
    $document->comment_count = isset($node->comment_count) ? $node->comment_count : 0;
    $document->name = $node->name;

    $path = 'node/' . $node->nid;
    $document->url = url($path, array('absolute' => TRUE));
    $document->path = $path;
    // Path aliases can have important information about the content.
    // Add them to the index as well.
    if (function_exists('drupal_get_path_alias')) {
      // Add any path alias to the index, looking first for language specific
      // aliases but using language neutral aliases otherwise.
      $language = empty($node->language) ? '' : $node->language;
      $output = drupal_get_path_alias($path, $language);
      if ($output && $output != $path) {
        $document->path_alias = $output;
      }
    }

    // Get CCK fields list
    $cck_fields = apachesolr_cck_fields();
    foreach ($cck_fields as $key => $cck_info) {
      if (isset($node->$key)) {
        // Got a CCK field. See if it is to be indexed.
        $function = $cck_info['indexing_callback'];
        if ($cck_info['indexing_callback'] && function_exists($function)) {
          $field = $function($node, $key);
        }
        else {
          $field = $node->$key;
        }
        $index_key = apachesolr_index_key($cck_info);
        if (is_array($field)) {
          foreach ($field as $value) {
            // Don't index NULLs or empty strings
            // We can use 'value' rather than 'safe' since we strip tags and later check_plain().
            // Furthermore, what is being indexed is the KEY for the CCK value. It will need
            // a trip through content_format() later to display the value.
            switch ($cck_info['field_type']) {
              case 'nodereference':
                $index_value = (isset($value['nid']) && strlen($value['nid'])) ? $value['nid'] : FALSE;
                break;
  
              case 'userreference':
                $index_value = (isset($value['uid']) && strlen($value['uid'])) ? $value['uid'] : FALSE;
                break;
  
              default:
                $index_value = (isset($value['value']) && strlen($value['value'])) ? $value['value'] : FALSE;
                break;
            }
            if ($index_value) {
              if ($cck_info['multiple']) {
                $document->setMultiValue($index_key, apachesolr_clean_text($index_value));
              }
              else {
                $document->$index_key = apachesolr_clean_text($index_value);
              }
            }
          }
        }
      }
    }
    // Index book module data.
    if (!empty($node->book['bid'])) {
      // Hard-coded - must change if apachesolr_index_key() changes.
      $document->is_book_bid = (int) $node->book['bid'];
    }
    apachesolr_add_tags_to_document($document, $text);
    apachesolr_add_taxonomy_to_document($document, $node);

    // Let modules add to the document.
    foreach (module_implements('apachesolr_update_index') as $module) {
      $function = $module .'_apachesolr_update_index';
      $function($document, $node, $namespace);
    }
  }
  return $document;
}

/**
 * Extract taxonomy from $node and add to dynamic fields.
 */
function apachesolr_add_taxonomy_to_document(&$document, $node) {
  if (isset($node->taxonomy) && is_array($node->taxonomy)) {
    foreach ($node->taxonomy as $term) {
      // Double indexing of tids lets us do effecient searches (on tid)
      // and do accurate per-vocabulary faceting.

      // By including the ancestors to a term in the index we make
      // sure that searches for general categories match specific
      // categories, e.g. Fruit -> apple, a search for fruit will find
      // content categorized with apple.
      $ancestors = taxonomy_get_parents_all($term->tid);
      foreach ($ancestors as $ancestor) {
        $document->setMultiValue('tid', $ancestor->tid);
        $document->setMultiValue('im_vid_'. $ancestor->vid, $ancestor->tid);
        $name = apachesolr_clean_text($ancestor->name);
        $document->setMultiValue('vid', $ancestor->vid);
        $document->{'ts_vid_'. $ancestor->vid .'_names'} .= ' '. $name;
        // We index each name as a string for cross-site faceting
        // using the vocab name rather than vid in field construction .
        $document->setMultiValue('sm_vid_'. apachesolr_vocab_name($ancestor->vid), $name);
      }
    }
  }
}

/**
 * Helper function - return a safe (PHP identifier) vocabulary name.
 */
function apachesolr_vocab_name($vid) {
  static $names = array();

  if (!isset($names[$vid])) {
    $vocab_name = db_result(db_query('SELECT v.name FROM {vocabulary} v WHERE v.vid = %d', $vid));
    $names[$vid] = preg_replace('/[^a-zA-Z0-9_\x7f-\xff]/', '_', $vocab_name);
    // Fallback for names ending up all as '_'.
    $check = rtrim($names[$vid], '_');
    if (!$check) {
      $names[$vid] = '_' . $vid . '_';
    }
  }
  return $names[$vid];
}

/**
 * Extract HTML tag contents from $text and add to boost fields.
 *
 * $text must be stripped of control characters before hand.
 */
function apachesolr_add_tags_to_document(&$document, $text) {
  $tags_to_index = variable_get('apachesolr_tags_to_index', array(
    'h1' => 'tags_h1',
    'h2' => 'tags_h2_h3',
    'h3' => 'tags_h2_h3',
    'h4' => 'tags_h4_h5_h6',
    'h5' => 'tags_h4_h5_h6',
    'h6' => 'tags_h4_h5_h6',
    'u' => 'tags_inline',
    'b' => 'tags_inline',
    'i' => 'tags_inline',
    'strong' => 'tags_inline',
    'em' => 'tags_inline',
    'a' => 'tags_a'
  ));

  // Strip off all ignored tags.
  $text = strip_tags($text, '<'. implode('><', array_keys($tags_to_index)) .'>');

  preg_match_all('@<('. implode('|', array_keys($tags_to_index)) .')[^>]*>(.*)</\1>@Ui', $text, $matches);
  foreach ($matches[1] as $key => $tag) {
    // We don't want to index links auto-generated by the url filter.
    if ($tag != 'a' || !preg_match('@(?:http://|https://|ftp://|mailto:|smb://|afp://|file://|gopher://|news://|ssl://|sslv2://|sslv3://|tls://|tcp://|udp://|www\.)[a-zA-Z0-9]+@', $matches[2][$key])) {
      $document->{$tags_to_index[$tag]} .= ' '. $matches[2][$key];
    }
  }
}

/**
 * Additional index utility functions
 */
 
/**
 * hook_cron() helper to try to make {apachesolr_search_node} consistent with {node}.
 */
function apachesolr_cron_check_node_table() {
  // Check for unpublished content that wasn't deleted from the index.
  $result = db_query("SELECT n.nid, n.status FROM {apachesolr_search_node} asn INNER JOIN {node} n ON n.nid = asn.nid WHERE asn.status != n.status");
  $node_lists = array();
  $nodes = array();
  // Update or delete at most this many in each Solr query.
  $limit = variable_get('apachesolr_cron_mass_limit', 500);
  while ($node = db_fetch_object($result)) {
    $nodes[$node->nid] = $node;
    if (count($nodes) == $limit) {
      $node_lists[] = $nodes;
      $nodes = array();
    }
  }
  // Any remaning ones if the limit is not reached.
  if (count($nodes)) {
    $node_lists[] = $nodes;
  }
  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_update() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_nodeapi_mass_update($nodes)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
  // Check for deleted content that wasn't deleted from the index.
  $result = db_query("SELECT asn.nid FROM {apachesolr_search_node} asn LEFT JOIN {node} n ON n.nid = asn.nid WHERE n.nid IS NULL");
  $node_lists = array();
  $nodes = array();
  while ($node = db_fetch_object($result)) {
    $nodes[$node->nid] = $node;
    if (count($nodes) == $limit) {
      $node_lists[] = $nodes;
      $nodes = array();
    }
  }
  // Any remaning ones if the limit is not reached.
  if (count($nodes)) {
    $node_lists[] = $nodes;
  }
  foreach ($node_lists as $nodes) {
    watchdog('Apache Solr', 'On cron running apachesolr_nodeapi_mass_delete() on nids @nids', array('@nids' => implode(',', array_keys($nodes))), WATCHDOG_WARNING);
    if (!apachesolr_nodeapi_mass_delete($nodes)) {
      // Solr query failed - so stop trying.
      break;
    }
  }
}
 
function apachesolr_nodeapi_mass_update($nodes) {
  if (empty($nodes)) {
    return TRUE;
  }
  $ids = array();
  foreach ($nodes as $node) {
    if (!$node->status) {
      $ids[] = apachesolr_document_id($node->nid);
    }
  }
  $time = time();
  try {
    $solr = apachesolr_get_solr();
    $solr->deleteByMultipleIds($ids);
    apachesolr_index_updated($time);
    foreach ($nodes as $node) {
      // There was no exception, so update the table.
      db_query("UPDATE {apachesolr_search_node} SET changed = %d, status = %d WHERE nid  = %d", $time, $node->status, $node->nid);
    }
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}
 
function apachesolr_nodeapi_mass_delete($nodes) {
  if (empty($nodes)) {
    return TRUE;
  }
  $ids = array();
  $nids = array();
  foreach ($nodes as $node) {
    $ids[] = apachesolr_document_id($node->nid);
    $nids[] = $node->nid;
  }
  try {
    $solr = apachesolr_get_solr();
    $solr->deleteByMultipleIds($ids);
    apachesolr_index_updated($time);
    // There was no exception, so update the table.
    db_query("DELETE FROM {apachesolr_search_node} WHERE nid  IN (" . db_placeholders($nids) . ")", $nids);
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
}

