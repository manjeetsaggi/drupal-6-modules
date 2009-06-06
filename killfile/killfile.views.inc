<?php
// $Id: killfile.views.inc,v 1.1 2008/10/28 09:57:55 arto Exp $

//////////////////////////////////////////////////////////////////////////////
// Views API hooks

/**
 * Implementation of hook_views_handlers().
 */
function killfile_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'killfile') . '/includes',
    ),
    'handlers' => array(),
  );
}

/**
 * Implementation of hook_views_data().
 */
function killfile_views_data() {
  return array(
    'killfile_nodes' => array(
      'table' => array(
        'group' => t('Killfile'),
        'join'  => array(
          'node'  => array('left_field' => 'nid', 'field' => 'nid'),
          'users' => array('left_field' => 'uid', 'field' => 'uid'),
        ),
      ),
      'nid' => array(
        'title'    => t('Node'),
        'help'     => t('The killfiled node.'),
        'relationship' => array(
          'base'    => 'node',
          'field'   => 'nid',
          'handler' => 'views_handler_relationship',
          'label'   => t('Node'),
        ),
      ),
      'uid' => array(
        'title'    => t('User'),
        'help'     => t('The user who killfiled the node.'),
        'field'    => array('handler' => 'views_handler_field_user_name', 'click sortable' => TRUE),
        'argument' => array('handler' => 'views_handler_argument_user_uid'),
        'filter'   => array('handler' => 'views_handler_filter_user_name', 'title' => t('Name')),
        'sort'     => array('handler' => 'views_handler_sort'),
        'relationship' => array(
          'base'    => 'users',
          'field'   => 'uid',
          'handler' => 'views_handler_relationship',
          'label'   => t('User'),
        ),
      ),
      'timestamp' => array(
        'title'    => t('Date'),
        'help'     => t('The date when the node was killfiled.'),
        'field'    => array('handler' => 'views_handler_field_date'),
        'sort'     => array('handler' => 'views_handler_sort_date'),
        'filter'   => array('handler' => 'views_handler_filter_date'),
      ),
    ),
  );
}
