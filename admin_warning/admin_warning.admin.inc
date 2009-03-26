<?php
// $Id: admin_warning.admin.inc,v 1.1 2009/02/03 00:52:38 drothstein Exp $

/**
 * @file
 * Code that is only expected to run when administrators are viewing the site.
 */

/**
 * Menu callback; returns the page for configuring the administrator warning
 * settings.
 */
function admin_warning_settings_page() {
  return drupal_get_form('admin_warning_settings_form');
}

/**
 * Returns a form for configuring the administrator warning settings.
 */
function admin_warning_settings_form() {
  $form = array();
  $form['admin_warning_message_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text of the warning message'),
    '#description' => t('Enter the text that site administrators will see at the top of most forms on the site.'),
    '#default_value' => admin_warning_settings('admin_warning_message_text'),
  );
  $form['admin_warning_excluded_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form IDs to exclude the warning message from (advanced)'),
    '#description' => t('Enter a list of form IDs for which the site administrator warning will <em>not</em> be shown. Put each form ID on a separate line. The site administrator warning will be shown at the top of all forms except those listed here.'),
    '#default_value' => admin_warning_settings('admin_warning_excluded_form_ids'),
  );
  return system_settings_form($form);
}

/**
 * Themes the warning message that is shown to administrators at the top of
 * most forms on the site.
 *
 * @param $message
 *   The text of the warning message.
 * @return
 *   Themed HTML output.
 *
 * @ingroup themeable
 */
function theme_admin_warning_message($message) {
  // Since this CSS will potentially be shown to a few users but on many
  // different pages, it is probably a bad idea to cache it.
  drupal_add_css(drupal_get_path('module', 'admin_warning') .'/admin_warning.css', 'module', 'all', FALSE);
  return '<div class="admin-warning-message">'. $message .'</div>';
}

/**
 * Themes the help text on this module's administration page.
 *
 * @param $help_text
 *   An array containing help text.
 * @return
 *   Themed HTML output.
 *
 * @ingroup themeable
 */
function theme_admin_warning_help($help_text) {
  $output = '';
  foreach ($help_text as $text) {
    $output .= '<p>'. $text .'</p>';
  }
  return $output;
}
