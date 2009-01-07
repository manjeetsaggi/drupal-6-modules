<?php
// $Id: salt.admin.inc,v 1.1.2.1 2008/03/29 21:57:44 karthik Exp $

/**
 * @file
 * Admin page callbacks for the salt module.
 */

/**
 * Menu callback: Display the salt module settings form.
 */
function salt_settings_form() {
  $form['salt'] = array(
    '#type' => 'textfield',
    '#title' => t('Salt'),
    '#description' => t('Enter the salt that you would like appended to user passwords. Changing the salt will automatically invalidate existing passwords which will need to be recovered and reset.'),
    '#default_value' => variable_get('salt', ''),
    '#required' => TRUE
  );

  return system_settings_form($form);
}

/**
 * Validate the salt settings form submission.
 */
function salt_settings_form_validate($form, &$form_state) {
  // Trim the salt string to avoid whitespace issues during concatenation.
  form_set_value($form['salt'], trim($form_state['values']['salt']), $form_state);
}
