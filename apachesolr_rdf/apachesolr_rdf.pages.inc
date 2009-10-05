<?php
// $Id: apachesolr_rdf.pages.inc,v 1.1.2.12 2009/08/11 01:24:32 drunkenmonkey Exp $

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for
 * existing contexts.
 */
function apachesolr_rdf_context_autocomplete() {
  // The parameter to the autocomplete call gets split at slashes, but all
  // components are passed to this function. Therefore we can use this to
  // construct the actual string.
  $args = func_get_args();
  $string = implode('/', $args);

  $matches = array();
  foreach (rdf_get_contexts() as $context) {
    if (!$string || strpos($context, $string) !== FALSE) {
      $matches[$context] = check_plain($context);
    }
  }

  drupal_json($matches);
}

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for
 * existing permissions.
 */
function apachesolr_rdf_permission_autocomplete($string = '') {
  $matches = array();
  foreach (module_invoke_all('perm') as $perm) {
    if (!$string || strpos($perm, $string) !== FALSE) {
      $matches[$perm] = check_plain($perm);
    }
  }

  drupal_json($matches);
}

/**
 * Outputs an overview page for all enabled Apachesolr RDF searches.
 */
function apachesolr_rdf_search_overview($id = NULL) {
  $searches = _apachesolr_rdf_searches(TRUE);

  if (isset($id) && !isset($searches[$id])) {
    drupal_set_message(t('The search with the id "@id" is either unknown or ' .
        'disabled.', array('@id' => $id)), 'error');
  }

  if (empty($searches)) {
    $output = '<p>' . t('There are no RDF searches available.') . '</p>';
    if (user_access('administer site configuration')) {
      $output .= "\n<p>" . t('You can enable some at the <a href="@url">' .
          'Apache Solr settings page</a>.',
          array('@url' => url('admin/settings/apachesolr_rdf'))) . '</p>';
    }
    return $output;
  }

  $output = '<p>' . t('The following RDF searches are available:') . "</p>\n";
  $items = array();
  foreach ($searches as $id => $info) {
    $items[] = l($info['name'], "search/apachesolr_rdf/{$info['id']}");
  }
  $output .= theme('item_list', $items);

  return $output;
}

/**
 * Displays a search and/or search results appropriate for the selected
 * schema.
 */
function apachesolr_rdf_show_search($id, $info) {
  $output = '';
  drupal_set_title($info['name']);

  if (isset($_GET['filters'])) {
    $_GET['filters'] = _apachesolr_rdf_urldecode($_GET['filters']);
  }

  $keys = apachesolr_rdf_search_keys($id);
  $filters = isset($_GET['filters']) ? $_GET['filters'] : array();
  $sort = isset($_GET['sort']) ? $_GET['sort'] : '';
  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  $output .= drupal_get_form('search_form', '', $keys, "apachesolr_rdf/$id");

  if ($keys || $filters) {
    $execute_search = _apachesolr_rdf_get_schema_function(
        'execute_search', $info['schema']);
    if (!$execute_search) {
      watchdog('Apache Solr RDF', 'No execute_search function found for ' .
          'schema "@schema".', array('@schema' => $info['schema']),
          WATCHDOG_ERROR);
      drupal_set_message(t('@search cannot be used due to an internal error.',
          array('@search' => $info['name'])), 'error');
      drupal_goto('search/apachesolr_rdf');
    }

    watchdog('search', "$keys (apachesolr_rdf/$id)", NULL);
    $output .= $execute_search($info, $keys, $filters, $sort,
        $page);
  }

  return $output;
}

/**
 * Submit function for an Apachesolr RDF search.
 */
function apachesolr_rdf_search_submit($form, &$form_state) {
  $v = $form_state['values'];

  $keys= trim($v['keys']);
  if (!empty($v['context'])) {
    $query['filters']['context'][] = $v['context'];
  }

  $path = 'search/apachesolr_rdf/' . $v['apachesolr_rdf_id'];

  if (empty($query)) {
    if (empty($keys)) {
      form_set_error('keys', t('Please enter some keywords.'));
    }
    else {
      $form_state['redirect'] = "$path/$keys";
    }
  }
  else {
    $form_state['redirect'] = array("$path/$keys", $query);
  }
}

//
// Schema functions
//
// - dynamic fields
//

/**
 * Alters the search form by adding advanced RDF search options and telling the
 * form to use the right submit function.
 */
function apachesolr_rdf_search_form_alter_dynamic(
    &$form, &$form_state, $id, $schema_args) {
  $form['#submit'] = array('apachesolr_rdf_search_submit');

  $advanced = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Search'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_GET['filters']),
  );
  $advanced['context'] = array(
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#title' => t('Context'),
    '#default_value' => isset($_GET['filters']['context'][0])
        ? $_GET['filters']['context'][0] : '',
    '#autocomplete_path' => "apachesolr_rdf/autocomplete/context",
  );

  $advanced['advanced_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['advanced'] = $advanced;
}

/**
 * Executes a search using the "dynamic fields" schema.
 */
function apachesolr_rdf_execute_search_dynamic($info, $keys, $filters, $sort,
    $page) {
  // Get server information
  $server = apachesolr_rdf_server_load($info['server']);

  // Get solr object
  $solr = apachesolr_get_solr($server['host'], $server['port'],
      $server['path']);

  // Build filter string
  $filterarr = array();
  $fields = _apachesolr_rdf_get_fields_dynamic($info['schema_args']);
  $esc_fields = array_map(array($solr, 'escape'), $fields);

  foreach ($filters as $field => $values) {
    // "Correct" single-value $values
    if (!is_array($values)) {
      $values = array($values);
    }

    if (!empty($esc_fields[$field])) {
      $value = '"' . implode('" "',
          array_map(array($solr, 'escapePhrase'), $values)) . '"';
      $filterarr[] = "{$esc_fields[$field]}:($value)";
    }
    else {
      // Fall back to property_object field & assume $field to be predicate URI
      foreach ($values as $value) {
        $field = $solr->escapePhrase($field);
        $value = $solr->escapePhrase($value);
        $filterarr[] = "\"$field $value\"";
      }
    }
  }

  $filterstring = implode(' ', $filterarr);
  if (!empty($info['options']['filter'])) {
    $filterstring .= ' (' . $info['options']['filter'] . ')';
  }

  // Get query object
  list($module, $class) = variable_get('apachesolr_query_class',
      array('apachesolr', 'Solr_Base_Query'));
  include_once drupal_get_path('module', $module) .'/'. $class .'.php';

  try {
    $query = new $class($solr, $keys ? $solr->escape($keys) : '*:*',
        $filterstring, $sort, "search/apachesolr_rdf/{$info['id']}");

    // Set parameters
    $params = array(
      'fl' => 'uri,context,' .
          $fields[APACHESOLR_RDF_TYPE] . ',' .
          $fields[APACHESOLR_RDF_LABEL] . ',' .
          $fields[APACHESOLR_RDF_COMMENT],
      'rows' => variable_get('apachesolr_rows', 10),
      'facet' => 'true',
      'facet.mincount' => 1,
      'facet.sort' => 'true'
    );
    $params['start'] = $page * $params['rows'];

    // Search and render results
    $response = $solr->search($query->get_query_basic(), $params['start'],
        $params['rows'], $params);
    return apachesolr_rdf_render_response_dynamic
        ($info, $response, $query, $params);
  }
  catch (Exception $e) {
    watchdog('Apache Solr RDF', nl2br(check_plain($e->getMessage())), NULL,
        WATCHDOG_ERROR);
    drupal_set_message(t('@name cannot be used due to an internal error.',
        array('@name' => $info['name'])), 'error');
    return;
  }
}

/**
 * Renders a search response of the "dynamic fields" schema.
 */
function apachesolr_rdf_render_response_dynamic
    ($info, $response, $query, $params) {
  // Pager query to get a pager output
  $total = $response->response->numFound;
  if (!$total) {
    return theme('box', t('Your search yielded no results'),
        search_help('search#noresults', drupal_help_arg()));
  }
  pager_query("SELECT %d", $params['rows'], 0, NULL, $total);

  $results = array();
  $fields = _apachesolr_rdf_get_fields_dynamic($info['schema_args']);
  $type_f = $fields[APACHESOLR_RDF_TYPE];
  $label_f = $fields[APACHESOLR_RDF_LABEL];
  $comment_f = $fields[APACHESOLR_RDF_COMMENT];

  foreach ($response->response->docs as $doc) {
    $results[] = array(
      'link' => $doc->uri,
      'type' => isset($doc->$type_f) ? $doc->$type_f : FALSE,
      'title' => isset($doc->$label_f) ? $doc->$label_f : $doc->uri,
      'snippet' => isset($doc->$comment_f) ? $doc->$comment_f : FALSE,
    );
  }

  return theme('box', t('Search results'), theme('search_results', $results,
      "apachesolr_rdf/{$info['id']}"));
}

//
// - text data
//

/**
 * Alters the search form by adding advanced RDF search options and telling the
 * form to use the right submit function.
 */
function apachesolr_rdf_search_form_alter_text_data(
    &$form, &$form_state, $id, $schema_args) {
  $form['#submit'] = array('apachesolr_rdf_search_submit');

  $advanced = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Search'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_GET['filters']),
  );
  $advanced['context'] = array(
    '#type' => 'textfield',
    '#maxlength' => 255,
    '#title' => t('Context'),
    '#default_value' => isset($_GET['filters']['context'][0])
        ? $_GET['filters']['context'][0] : '',
    '#autocomplete_path' => "apachesolr_rdf/autocomplete/context",
  );

  $advanced['advanced_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['advanced'] = $advanced;
}

/**
 * Executes a search using the "text data" schema.
 */
function apachesolr_rdf_execute_search_text_data($info, $keys, $filters, $sort,
    $page) {
  // Get server information
  $server = apachesolr_rdf_server_load($info['server']);

  // Get solr object
  $solr = apachesolr_get_solr($server['host'], $server['port'],
      $server['path']);

  // Build filter string
  $filterarr = array();
  foreach ($filters as $field => $values) {
    $value = '"' . implode('" "',
        array_map(array($solr, 'escapePhrase'), $values)) . '"';
    $field = $solr->escape($field);
    $filterarr[] = "$field:($value)";
  }
  $filterstring = implode(' ', $filterarr);

  // Get query object
  list($module, $class) = variable_get('apachesolr_query_class',
      array('apachesolr', 'Solr_Base_Query'));
  include_once drupal_get_path('module', $module) .'/'. $class .'.php';

  try {
    $query = new $class($solr, $keys ? $solr->escape($keys) : '*:*',
        $filterstring, $sort, "search/apachesolr_rdf/{$info['id']}");

    // Set parameters
    $params = array(
      'fl' => 'uri,context,type,property,text',
      'rows' => variable_get('apachesolr_rows', 10),
      'facet' => 'true',
      'facet.mincount' => 1,
      'facet.sort' => 'true'
    );
    $params['start'] = $page * $params['rows'];

    // Search and render results
    $response = $solr->search($query->get_query_basic(), $params['start'],
        $params['rows'], $params);
    return apachesolr_rdf_render_response_text_data
        ($info, $response, $query, $params);
  }
  catch (Exception $e) {
    watchdog('Apache Solr RDF', nl2br(check_plain($e->getMessage())), NULL,
        WATCHDOG_ERROR);
    return;
  }
}

/**
 * Renders a search response of the "text data" schema.
 */
function apachesolr_rdf_render_response_text_data
    ($info, $response, $query, $params) {
  // Pager query to get a pager output
  $total = $response->response->numFound;
  if (!$total) {
    return theme('box', t('Your search yielded no results'),
        search_help('search#noresults', drupal_help_arg()));
  }
  pager_query("SELECT %d", $params['rows'], 0, NULL, $total);

  $results = array();
  foreach ($response->response->docs as $doc) {
    $type = is_array($doc->type) ? implode(', ', $doc->type) : $doc->type;
    $label = $comment = FALSE;
    $prop_count = count($doc->property);
    for ($i = 0; $i < $prop_count; ++$i) {
      if ($doc->property[$i] == APACHESOLR_RDF_LABEL) {
        $label = $doc->text[$i];
      }
      else if ($doc->property[$i] == APACHESOLR_RDF_COMMENT) {
        $comment = $doc->text[$i];
      }
      // When we've found all desired fields, stop looking.
      if ($label && $comment) {
        break;
      }
    }
    $results[] = array(
      'link' => $doc->uri,
      'type' => $type,
      'title' => $label ? $label : $doc->uri,
      'snippet' => $comment,
    );
  }

  return theme('box', t('Search results'), theme('search_results', $results,
      "apachesolr_rdf/{$info['id']}"));
}

/**
 * Decodes '%2F' to '/'. Necessary because of a bug in
 * drupal_query_string_encode().
 */
function _apachesolr_rdf_urldecode($array) {
  if (!is_array($array)) {
    return str_replace('%2F', '/', $array);
  }

  $ret = array();

  foreach ($array as $key => $value) {
    $key = str_replace('%2F', '/', $key);
    $value = _apachesolr_rdf_urldecode($value);
    $ret[$key] = $value;
  }

  return $ret;
}
