<?php
// $Id: file.admin.inc,v 1.26 2009/03/26 23:30:52 miglius Exp $

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// File settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function file_admin_settings() {
  // Visibility settings
  $form['visibility'] = array('#type' => 'fieldset', '#title' => t('Visibility settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['visibility']['file_show_previews'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Previews'),
    '#description'   => t('Show the available generated previews for the files.'),
    '#default_value' => FILE_SHOW_PREVIEWS,
  );
  $form['visibility']['file_handler_autoload'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Preview autoload'),
    '#description'   => t('If "Previews" are enabled, then the preview generated by the first handler is automatically loaded via AHAH call.'),
    '#default_value' => FILE_HANDLER_AUTOLOAD,
  );
  $form['visibility']['file_show_generated'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Generated (converted) files'),
    '#description'   => t('Show a list of the avialable converted files in other MIME types. The list is also available as a block.'),
    '#default_value' => FILE_SHOW_GENERATED,
  );
  $form['visibility']['file_show_file'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('File'),
    '#description'   => t('Show a file with a drop-down contextual menu.'),
    '#default_value' => FILE_SHOW_FILE,
  );
  $form['visibility']['file_view_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('View link'),
    '#description'   => t('Show a "view" link next to "download" link. Clicking the view link will bring the file with a headder content-disposition set to inline.'),
    '#default_value' => FILE_VIEW_LINK,
  );
  $form['visibility']['file_info_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Info link'),
    '#description'   => t('Show an "info" link next to "download" link. Clicking the info link will bring the information about who and when created a file node.'),
    '#default_value' => FILE_INFO_LINK,
  );
  $form['visibility']['file_properties_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Properties link'),
    '#description'   => t('Show an "properties" link next to "download" link. Clicking the properties link will bring the file metadata.'),
    '#default_value' => FILE_PROPERTIES_LINK,
  );
  $form['visibility']['file_properties_tab'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Properties tab'),
    '#description'   => t('Show a properties tab with a file\'s metadata.'),
    '#default_value' => FILE_PROPERTIES_TAB,
  );
  $form['visibility']['file_popup_size'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Max size of thickbox popup previews'),
    '#default_value' => FILE_POPUP_SIZE,
    '#size'          => 10,
    '#maxlength'     => 255,
    '#description'   => t('The default size is 800x600.'),
  );

  // Maintenance settings
  $form['maintenance'] = array('#type' => 'fieldset', '#title' => t('Maintenance settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['maintenance']['file_cron'] = array(
    '#type'          => 'radios',
    '#title'         => t('Perform automatic maintenance on cron runs'),
    '#options'       => array(FALSE => t('Disabled'), TRUE => t('Enabled')),
    '#default_value' => FILE_CRON,
    '#description'   => t('Enables automatic sanity checks and repairs for the converted files and metadata extraction. Normally you would always want to have this enabled.'),
  );
  $form['maintenance']['file_cron_limit_size'] = array(
    '#type'          => 'select',
    '#title'         => t('Megabytes to convert per cron run'),
    '#default_value' => FILE_CRON_LIMIT_SIZE,
    '#options'       => drupal_map_assoc(array(16, 32, 64, 128, 256, 512, 1024, 2048, 4096)),
    '#description'   => t('The cron job will proccess uploaded files and create missing previews or files in other MIME types. The maximum amount of megabytes that will be processed in one cron run. Set this number lower if your cron is timing out or if PHP is running out of memory. Note that individual files larger than this limit will never be processed.'),
  );
  $form['maintenance']['file_cron_limit_total'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Files to inspect per cron run'),
    '#default_value' => FILE_CRON_LIMIT_TOTAL,
    '#size'          => 6,
    '#description'   => t('The cron job will inspect only chosen number of files per run. Set it to zero to make it unlimited.'),
  );

  // OG integration.
  if (module_exists('og_vocab')) {
    $form['og'] = array('#type' => 'fieldset', '#title' => t('OG integration'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['og']['file_og_browser_tab'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Enable browser tab on the group\'s homepage'),
      '#description'   => t('If enabled, the tab will be shown on the group\'s homepage. Otherwise this functionality will be available from the menu.'),
      '#default_value' => FILE_OG_BROWSER_TAB,
    );
    $form['og']['file_og_gallery_tab'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Enable gallery tab on the group\'s homepage'),
      '#description'   => t('If enabled, the tab will be shown on the group\'s homepage. Otherwise this functionality will be available from the menu.'),
      '#default_value' => FILE_OG_GALLERY_TAB,
    );
  }

  $form['clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear data about failed conversions and metadata extractions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('When a file conversion or metadata extraction fails, it is marked as such to make sure that the same conversion or metadata extraction is not performed on the same file again from the cron. Click "Clear" to delete all data about failures. This is usefull if a conversion utility is fixed or additional format module is enabled.'),
  );
  $form['clear']['clear'] = array(
    '#type'  => 'submit',
    '#value' => t('Clear'),
  );

  $form['check'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find missing bitstreams in the bitcache'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Sanity check. Click "Check" to find out if all file nodes have a corresponding bitstreams saved in the bitcache.'),
  );
  $form['check']['check'] = array(
    '#type'  => 'submit',
    '#value' => t('Check'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset to defaults'),
  );

  return $form;
}

/**
 * Validate hook for the settings form.
 */
function file_admin_settings_validate($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  $values = $form_state['values'];
  switch ($op) {
    case t('Save configuration'):
      if (!is_numeric($values['file_cron_limit_total'])) {
        form_set_error('file_cron_limit_total', t('A total number should be numeric.'));
      }
      break;
  }
}

/**
 * Submit hook for the settings form.
 */
function file_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  $values = $form_state['values'];
  switch ($op) {
    case t('Save configuration'):
      variable_set('file_show_previews', $values['file_show_previews']);
      variable_set('file_handler_autoload', $values['file_handler_autoload']);
      variable_set('file_show_generated', $values['file_show_generated']);
      variable_set('file_show_file', $values['file_show_file']);
      variable_set('file_view_link', $values['file_view_link']);
      variable_set('file_info_link', $values['file_info_link']);
      variable_set('file_properties_link', $values['file_properties_link']);
      variable_set('file_properties_tab', $values['file_properties_tab']);
      variable_set('file_popup_size', $values['file_popup_size']);
      variable_set('file_cron', $values['file_cron']);
      variable_set('file_cron_limit_size', $values['file_cron_limit_size']);
      variable_set('file_cron_limit_total', $values['file_cron_limit_total']);
      variable_set('file_og_browser_tab', $values['file_og_browser_tab']);
      variable_set('file_og_gallery_tab', $values['file_og_gallery_tab']);
      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):
      variable_del('file_show_previews');
      variable_del('file_handler_autoload');
      variable_del('file_show_generated');
      variable_del('file_show_file');
      variable_del('file_view_link');
      variable_del('file_info_link');
      variable_del('file_properties_link');
      variable_del('file_properties_tab');
      variable_del('file_popup_size');
      variable_del('file_cron');
      variable_del('file_cron_limit_size');
      variable_del('file_cron_limit_total');
      variable_del('file_og_browser_tab');
      variable_del('file_og_gallery_tab');
      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
    case t('Clear'):
      if (!rdf_delete(NULL, 'wordnet:failed', NULL, array('repository' => FILE_RDF_REPOSITORY))) {
        watchdog('file', 'The RDF data about failed conversions and metadata extraction was not deleted.', WATCHDOG_ERROR);
      }
      drupal_set_message(t('The data about failed conversions and metadata extraction have been cleared.'));
      break;
    case t('Check'):
      $result = db_query("SELECT f.nid, f.uri, n.title FROM {file_nodes} f INNER JOIN {node} n ON f.nid = n.nid");
      $counter = 0;
      while ($row = db_fetch_object($result)) {
        if (!bitcache_exists(file_get_hash($row->uri))) {
          drupal_set_message(t('A file node !file is missing a bitstream %uri.', array('!file' => l($row->title, 'node/'. $row->nid), '%uri' => file_get_hash($row->uri))), 'warning');
          $counter++;
        }
      }
      if ($counter == 0)
        drupal_set_message(t('All file nodes have coresponding bitstreams.'));
      break;
  }

  // Rebuild the menu router.
  menu_rebuild();
}

/**
 * Implements the handler settings page.
 *
 * @return
 *   The form structure.
 */
function file_admin_handler_settings() {
  // Handler settings
  $mime_types = file_get_mime_types();
  $mime_handlers = file_get_mime_handlers();
  foreach ($mime_handlers as $handler => $data) {
    $form['handler'][$handler]['title'] = array(
      '#value' => $mime_handlers[$handler]['name'],
    );
    $mimes = array();
    foreach ($mime_types as $mime => $data) {
      foreach ($data['handlers'] as $h) {
        if ($h == $handler) {
          $mimes[] = $mime;
        }
      }
    }
    $form['handler'][$handler]['mime'] = array(
      '#value' => implode('<br />', $mimes),
    );
    if (function_exists('file_get_mime_converters')) {
      $converters = array();
      foreach (file_get_mime_converters() as $from => $tos) {
        foreach ($tos as $to => $converter) {
          foreach ($converter['handlers'] as $h) {
            if ($h == $handler) {
              $converters[] = $from .' -> '. $to;
            }
          }
        }
      }
      $form['handler'][$handler]['convert'] = array(
        '#value' => implode('<br />', $converters),
      );
    }
    $form['handler'][$handler]['parent'] = array(
      '#value' => isset($mime_handlers[$handler]['parent']) ? $mime_handlers[$mime_handlers[$handler]['parent']]['name'] : NULL,
    );
    $form['handler'][$handler]['module'] = array(
      '#value' => $mime_handlers[$handler]['module'],
    );
    $form['handler'][$handler]['enabled'] = array(
      '#type'          => 'checkbox',
      '#name'          => 'handler_'. $handler .'_enabled',
      '#default_value' => $mime_handlers[$handler]['enabled'],
    );
    $form['handler'][$handler]['weight'] = array(
      '#type'          => 'weight',
      '#name'          => 'handler_'. $handler .'_weight',
      '#delta'         => 20,
      '#default_value' => $mime_handlers[$handler]['weight'],
    );
  }
  $form['handler']['table'] = '';

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset to defaults'),
  );

  return $form;
}

/**
 * Submit hook for the handler_settings form.
 */
function file_admin_handler_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $handlers = array();
      foreach ($form_state['clicked_button']['#post'] as $name => $val) {
        if (preg_match('/^handler_.*_weight/', $name)) {
          $handler = preg_replace(array('/^handler_/', '/_weight$/'), array('', ''), $name);
          $handlers[$handler]['weight'] = $val;
          $handlers[$handler]['enabled'] = !empty($form_state['clicked_button']['#post']['handler_'. $handler .'_enabled']) ? 1 : 0;
        }
      }
      variable_set('file_handlers', $handlers);
      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):
      variable_del('file_handlers');
      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }
}

/**
 * Implements the MIME settings page.
 *
 * @return
 *   The form structure.
 */
function file_admin_mime() {
  // MIME type autodetection
  $form['autodetection'] = array('#type' => 'fieldset', '#title' => t('Additional MIME detection'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['autodetection']['file_mime_autodetection'] = array(
    '#type'          => 'radios',
    '#title'         => t('Server-side MIME type autodetection after upload'),
    '#options'       => array(t('Disabled'), t('Enabled'), t('Conditional')),
    '#default_value' => FILE_MIME_AUTODETECTION,
    '#description'   => t('Whether to always attempt MIME type autodetection on the server side for any uploaded file from the contents of the file, regardless of whether the client supplied MIME type information or the MIME type can be extracted from the file extension using the table below. The MIME type extraction will be done using \'fileinfo \' PHP extension or \'file\' unix utility whichever is available on the server. Your choice here depends on how well-setup your server is for proper MIME detection and how much you trust your users\' web browsers to report correct MIME types. Unless you are very sure of what you are doing, you are advised to leave this disabled, which corresponds with Drupal\'s default behavior.'),
  );
  $form['autodetection']['file_mime_autodetection_conditions'] = array(
    '#type'          => 'textarea',
    '#title'         => t('MIME autodetection conditions'),
    '#default_value' => FILE_MIME_AUTODETECTION_CONDITIONS,
    '#rows'          => 3,
    '#description'   => t('If "Server-side MIME type autodetection after upload" is set to "Conditional", please specify for which uploaded files MIME types or file extensions the autodetection should be performed on the server side. One MIME type or file extension without leading dot per line.'),
  );

  // MIME types
  $form['mime'] = array('#type' => 'fieldset', '#title' => t('MIME types'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $file_mime_types = '';
  foreach (variable_get('file_mime_types', array()) as $type => $extensions)
    $file_mime_types .= $type .'|'. implode('|', $extensions) ."\n";
  $form['mime']['file_mime_types'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Custom MIME types'),
    '#default_value' => $file_mime_types,
    '#rows'          => 5,
    '#description'   => t('Enter additional MIME type mappings to the table bellow or override the default ones, one mapping per line. A custom mappings will appear at the top of the table and will be compared first when detecting a MIME type from the file extension. A line should contain a MIME type, vertical bar and custom number of file extensions separated by vertical bars as in the following example:<br /><code>application/custom|cst|cs2|cs3</code>'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset to defaults'),
  );

  $form['list']['#value'] = file_admin_mime_list();

  return $form;
}

/**
 * validate hook for the MIME settings form.
 */
function file_admin_mime_validate($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  $values = $form_state['values'];
  switch ($op) {
    case t('Save configuration'):
      $form_state['file_mime_types'] = array();
      $mime_types_syntax = TRUE;
      $registry = file_mime_extensions(FALSE);
      foreach ((trim($values['file_mime_types']) ? explode("\n", drupal_strtolower(trim($values['file_mime_types']))) : array()) as $line) {
        if (count($data = explode('|', trim($line))) > 1) {
          if (!preg_match('/.+\/.+/', ($type = trim(array_shift($data)))) || count($extensions = array_values(array_unique(array_filter($data)))) == 0)
            $mime_types_syntax = FALSE;
          else
            $form_state['file_mime_types'] += array($type => $extensions);
        }
        else
          $mime_types_syntax = FALSE;
      }
      if (!$mime_types_syntax)
        form_set_error('file_mime_types', t('Bad MIME mapping syntax.'));
      break;
  }
}

/**
 * Submit hook for the MIME settings form.
 */
function file_admin_mime_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  $values = $form_state['values'];
  switch ($op) {
    case t('Save configuration'):
      variable_set('file_mime_autodetection', $values['file_mime_autodetection']);
      variable_set('file_mime_autodetection_conditions', $values['file_mime_autodetection_conditions']);
      variable_set('file_mime_types', $form_state['file_mime_types']);
      break;
    case t('Reset to defaults'):
      variable_del('file_mime_autodetection');
      variable_del('file_mime_autodetection_conditions');
      variable_del('file_mime_types');
      break;
  }
}

/**
 * Implements the registered MIMEs list page.
 *
 * @return
 *   The themed HTML page.
 */
function file_admin_mime_list() {
  $head = array(t('Type'), t('Extensions'), t('Description'), t('Module'));
  $rows = array();
  foreach (file_mime_extensions() as $type => $extensions) {
    $mime_types = file_get_mime_types();
    $rows[] = array(
      $type,
      implode(', ', $extensions),
      isset($mime_types[$type]) ? (is_array($mime_types[$type]['name']) ? reset($mime_types[$type]['name']) : $mime_types[$type]['name']) : '',
      isset($mime_types[$type]) ? (is_array($mime_types[$type]['module']) ? implode(', ', $mime_types[$type]['module']) : $mime_types[$type]['module']) : '',
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No MIME types registered.'), 'colspan' => '4'));
  }

  return theme('table', $head, $rows);
}

/**
 * Implements the file formats page.
 *
 * @return
 *   The themed HTML page.
 */
function file_admin_menu_block_page() {
  $item = menu_get_item();
  if ($content = system_admin_menu_block($item)) {
    $output = theme('admin_block_content', $content);
  }
  else {
    $output = t('You do not have any format items.');
  }
  return $output;
}

