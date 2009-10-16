<?php
// $Id: cron_key.admin.inc,v 1.1 2009/05/25 21:37:53 doq Exp $

/**
 * @file
 * Admin page callbacks for the cron_key module.
 */

/**
 * Configure cron key.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function cron_key_settings() {
  $form['cron_key'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron key'),
  );
  $form['cron_key']['cron_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('cron_key', 'drupal'),
    '#description' => t('Cron key to protect against unauthorized access.'),
  );
  $form['cron_key']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Regenerate cron key'),
    '#submit' => array('cron_key_regenerate_submit'),
  );
  return system_settings_form($form);
}

/**
 * Submit callback; regenerate cron key.
 *
 * @ingroup forms
 */
function cron_key_regenerate_submit(&$form_state, $form) {
  cron_key_regenerate();
  drupal_set_message(t('Cron key was regenerated.'));
}
