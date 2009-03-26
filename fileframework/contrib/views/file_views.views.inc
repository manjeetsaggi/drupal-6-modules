<?php
// $Id: file_views.views.inc,v 1.12 2008/09/23 11:05:55 miglius Exp $

/**
 * @file
 * Integrates file operations with the Views module.
 */

//////////////////////////////////////////////////////////////////////////////
// Views API hooks

/**
 * Implementation of hook_views_handlers().
 */
function file_views_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'file_views') .'/includes',
    ),
    'handlers' => array(
      'file_views_handler_field_file_nodes_numeric' => array(
        'parent' => 'views_handler_field',
      ),
      'file_views_handler_field_file_nodes_size' => array(
        'parent' => 'views_handler_field',
      ),
      'file_views_handler_field_file_nodes_type' => array(
        'parent' => 'views_handler_field',
      ),
      'file_views_handler_field_file_nodes_download' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'file_views_handler_field_file_nodes_view' => array(
        'parent' => 'views_handler_field_node_link',
      ),
      'file_views_handler_field_file_nodes_uri' => array(
        'parent' => 'views_handler_field',
      ),
      'file_views_handler_field_file_attachments_fnid' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'file_views_handler_field_file_attachments_nid' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function file_views_views_data() {
  $data = array();

  // ----------------------------------------------------------------------
  // file_nodes table

  $data['file_nodes']['table']['group']  = t('Files');

  $data['file_nodes']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['file_nodes']['size'] = array(
    'title' => t('Size'),
    'help' => t('The size of the file.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_nodes_size',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['file_nodes']['type'] = array(
    'title' => t('Type'),
    'help' => t('The MIME type of the file.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_nodes_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['file_nodes']['views'] = array(
    'title' => t('Viewed'),
    'help' => t('Display the number of times the file has been viewed.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_nodes_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['file_nodes']['downloads'] = array(
    'title' => t('Downloaded'),
    'help' => t('Display the number of times the file has been downloaded.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_nodes_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['file_nodes']['download'] = array(
    'field' => array(
      'title' => t('Download link'),
      'help' => t('Provide a simple link to download file.'),
      'handler' => 'file_views_handler_field_file_nodes_download',
    ),
  );

  $data['file_nodes']['view'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to view file.'),
      'handler' => 'file_views_handler_field_file_nodes_view',
    ),
  );

  $data['file_nodes']['uri'] = array(
    'field' => array(
      //'title' => t('Thumbnail'),
      'title' => t('File'),
      //'help' => t('Provide a thumbnail of the file.'),
      'help' => t('Provide a file.'),
      'handler' => 'file_views_handler_field_file_nodes_uri',
    ),
  );

  // ----------------------------------------------------------------------
  // file_attachments table

  $data['file_attachments']['table']['group']  = t('Files');

  $data['file_attachments']['table']['name']  = 'file_attachments';
  $data['file_attachments']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['file_attachments']['fnid'] = array(
    'title' => t('Attached files'),
    'help' => t('All files attached to a node.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_attachments_fnid',
    ),
    'relationship' => array(
      'title' => t('Node has attached files'),
      'help' => t('Add a relationship to show only nodes which have attachments.'),
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node has attachments'),
    ),
  );

  $data['file_attached_to']['table']['group']  = t('Files');

  $data['file_attached_to']['table']['name']  = 'file_attachments';
  $data['file_attached_to']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['file_attached_to']['nid'] = array(
    'title' => t('Attached to'),
    'help' => t('All nodes the file is attached to.'),
    'field' => array(
      'handler' => 'file_views_handler_field_file_attachments_nid',
    ),
  );

  return $data;
}
