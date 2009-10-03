<?php
// $Id: cck_sync.admin.inc,v 1.1 2009/10/01 15:02:21 amitaibu Exp $

/**
 * @file
 * Synchronize content type by code export settings.
 */

/**
 * Menu callback; CCK sync settings form.
 */
function cck_sync_admin_sync_form() {
  $form = array();

  $form['cck_sync_files'] = array(
    '#type' => 'textarea',
    '#title' => t('Exported code location'),
    '#default_value' => variable_get('cck_sync_files', ''),
    '#description' => t('Enter the files location of your exported code. Every file in a different line. Do not start with a trailing slash.')
  );

  return system_settings_form($form);
}


/**
 * Reset according to the list of files entered in the cck sync settings.
 */
function cck_sync_admin_sync() {
  if ($files = variable_get('cck_sync_files', '')) {
    $files = explode("\r\n", $files);
    foreach ($files as $file) {
      // Check the file exists.
      if (file_exists($file)) {
        cck_sync_from_file($file);
        drupal_set_message(t('%file was used to sync CCK fields.', array('%file' => $file)));
      }
      else {
        drupal_set_message(t('%file could not be found, please check the path.', array('%file' => $file)), 'error');
      }
    }
  }
}