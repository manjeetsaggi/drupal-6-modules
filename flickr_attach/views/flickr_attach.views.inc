<?php
// $Id: flickr_attach.views.inc,v 1.1 2009/06/25 13:37:46 jbomb Exp $

/**
 * hook_views_data implementation
 */
function flickr_attach_views_data() {
  $data['node_flickr_albums'] = array(
    'table' => array(
      'group' => t('Flickr Attach'),
      'join' => array(
        'node' => array(
          'table' => 'node_flickr',
          'left_field' => 'nid',
          'field' => 'nid',
          'extra' => array(
            array(
              'field' => 'type',
              'operator' => '=',
              'value' => 'album',
              ),
            ),
          ),
        ),
      ),
    'album_id' => array(
      'field' => array(
        'title' => t('Album ID'),
        'help' => t('The Flickr ID of a Flickr attached album'),
        'handler' => 'flickr_attach_id',
        ),
      ),
    );
    
  $data['node_flickr_images'] = array(
    'table' => array(
      'group' => t('Flickr Attach'),
      'join' => array(
        'node' => array(
          'table' => 'node_flickr',
          'left_field' => 'nid',
          'field' => 'nid',
          'extra' => array(
            array(
              'field' => 'type',
              'operator' => '=',
              'value' => 'image',
              ),
            ),
          ),
        ),
      ),
    'image_id' => array(
      'field' => array(
        'title' => t('Image'),
        'help' => t('The Flickr ID of an Flickr attached image'),
        'handler' => 'flickr_attach_id',
        ),
      ),
    );
  return $data;
}

/**
 * hook_views_handlers implementation
 */
function flickr_attach_views_handlers() {
  return array(
    'info' => array(
    'path' => drupal_get_path('module', 'flickr_attach') . '/views',
      ),
    'handlers' => array(
      'flickr_attach_id' => array(
        'parent' => 'views_handler_field',
        ),
      ),
    );
}

function flickr_attach_views_default_views() {
  $views = array();
  $view = new view;
  $view->name = 'flickr_attach';
  $view->description = 'A default block to display flickr_attach images on nodes';
  $view->tag = 'flickr_attach';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'image_id' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'text' => '',
      'exclude' => 0,
      'id' => 'image_id',
      'table' => 'node_flickr_images',
      'field' => 'image_id',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'display' => '3',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 1,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler = $view->new_display('block', 'Flickr_attach ', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  
  
  $views['flickr_attach'] = $view;
  return $views;
}
