<?php
// $Id: killfile.pages.inc,v 1.1 2008/10/28 09:57:55 arto Exp $

//////////////////////////////////////////////////////////////////////////////
// Menu callbacks

/**
 * Outputs an RSS 1.0-compatible RDF feed containing one RSS item for each
 * killfiled node on the site. Each RSS item contains additional killfile
 * metadata using the 'killfile:' namespace.
 */
function killfile_pages_feed() {
  if (!module_exists('rdf')) {
    drupal_set_message(t('The killfile RSS feed functionality requires that you install and enable the <a href="@url">RDF</a> module.', array('@url' => 'http://drupal.org/project/rdf')), 'error');
    return drupal_not_found();
  }

  global $base_url, $language;
  rdf_register_namespace('rss', 'http://purl.org/rss/1.0/');
  rdf_register_namespace('killfile', KILLFILE_NAMESPACE);

  // Construct the channel's metadata:
  $rss_url = url('killfile.rdf', array('absolute' => TRUE));
  $index   = rdf_seq();
  $channel = rdf_denormalize(array(
   $rss_url => array(
      'rdf:type'        => rdf_qname_to_uriref('rss:channel'),
      'rss:title'       => rdf_literal(t('Killfile for @site', array('@site' => variable_get('site_name', 'Drupal'))), TRUE),
      'rss:link'        => $base_url . '/',
      'rss:description' => rdf_literal(t('This feed contains an index of recently killfiled content at this site.'), TRUE),
      'rss:items'       => $index->uriref(),
    ),
  ));

  // Invoke hook_rdf_views_feed_channel() to allow third-party modules to
  // manipulate the channel metadata:
  foreach (module_implements('rdf_views_feed_channel') as $module) {
    $function = $module .'_rdf_views_feed_channel';
    $function(NULL, $channel);
  }

  // Compile the channel index and render the items into triples:
  $items  = array();
  $result = db_query('SELECT * FROM {killfile_nodes} WHERE timestamp <= %d ORDER BY timestamp DESC', time());
  while ($row = db_fetch_object($result)) {
    $item       = node_load($row->nid);
    $item->url  = url('node/'. $row->nid, array('absolute' => TRUE, 'alias' => TRUE));
    $item->link = url('node/'. $row->nid, array('absolute' => TRUE));

    // Make sure the item has all the basic mandatory RSS 1.0 properties:
    // @see http://web.resource.org/rss/1.0/spec#s5.5
    $data = array(
      array($item->url, 'rdf:type',  rdf_qname_to_uriref('rss:item')),
      array($item->url, 'rss:title', rdf_literal($item->title, $item)),
      array($item->url, 'rss:link',  $item->link),
    );

    // Invoke hook_rdf_views_feed_item() to allow our hook implementation to
    // add the 'killfile:date' and 'killfile:log' triples, and to allow
    // third-party modules to add any other useful item metadata:
    foreach (module_implements('rdf_views_feed_item') as $module) {
      $function = $module .'_rdf_views_feed_item';
      $function(NULL, $item, $data);
    }

    $index[] = $item->url;
    $items   = array_merge($items, $data);
  }

  // Invoke hook_rdf_views_feed() to allow third-party modules to manipulate
  // the full contents of the feed, including the metadata and all
  // constituent feed items.
  $data = rdf_normalize(array_merge($channel, $index->to_triples(), $items));
  foreach (module_implements('rdf_views_feed') as $module) {
    $function = $module .'_rdf_views_feed';
    $function($this, $data);
  }

  module_load_include('inc', 'rdf', 'rdf.pages');
  rdf_output('killfile', 'rdf+xml', $data); // default to serialization in RDF/XML format
}
