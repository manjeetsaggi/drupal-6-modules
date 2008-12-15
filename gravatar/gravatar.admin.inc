<?php
// $Id: gravatar.admin.inc,v 1.1.2.8 2008/11/08 21:22:20 narno Exp $

/**
 * Formbuilder function for gravatar_admin_settings 
 *
 * @return (mixed)
 *   array according to form-api 
 */
function gravatar_admin_settings () {
  $form = array();

  $form['gravatar_preferences'] = array(
    '#type'        => 'fieldset',
  );
  $form['gravatar_preferences']['gravatar_toggle'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Enable Gravatar integration'),
    '#description' => t('Enable or disable integration of Gravatar for this site.'),
    '#options'     => array(
      0 => t('Disable'),
      1 => t('Enable'), 
    ),
    '#default_value' => variable_get('gravatar_toggle', 0),
    '#disabled'      => _gravatar_check_required() ? FALSE : TRUE,
  );
  $form['gravatar_preferences']['gravatar_disabled_by_users'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Users can enable (and disable) Gravatar'),
    '#description'   => t('Authenticated users can disable their own Gravatar in their profile and own contents.'),
    '#options'       => array(
      0 => t('Disable'),
      1 => t('Enable'), 
    ),
    '#default_value' => variable_get('gravatar_disabled_by_users', 0),
    '#disabled'      => variable_get('gravatar_toggle', 0) ? FALSE : TRUE,
  );
  
  if (variable_get('user_picture_default', '')) {
      $user_picture_default_setting_text = t('settings');
  } else {
      $user_picture_default_setting_text = t('empty, go settings to change that');
  }
  $form['gravatar_display'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Display preferences'),
    '#description' => t('Control how Gravatars are displayed'),
    '#collapsible' => TRUE,
    '#collapsed'   => variable_get('gravatar_toggle', 0) ? FALSE : TRUE,
  );
  $form['gravatar_display']['gravatar_displaysize'] = array(
    '#type'          => 'select',
    '#title'         => t('Image size'),
    '#description'   => t('Select your preferred gravatar image size (max. 80x80)'),
    '#default_value' => variable_get('gravatar_displaysize', 48),
    '#options'       => drupal_map_assoc(array(16, 22, 24, 32, 48, 64, 72, 80), '_gravatar_admin_settings_map_size'),
    '#disabled'      => variable_get('gravatar_toggle', 0) ? FALSE : TRUE,
  );
  $form['gravatar_display']['gravatar_imagerating'] = array(
    '#type'          => 'select',
    '#title'         => t('Maturity filter'),
    '#description'   => t('An optional maturity rating parameter with a value (MPAA nomenclatura) that determines the highest rating (inclusive) that will be returned.'),
    '#default_value' => variable_get('gravatar_imagerating', 'PG'), 
    '#options'       => drupal_map_assoc(array(0, 'G', 'PG', 'R', 'X')),
    '#disabled'      => variable_get('gravatar_toggle', 0) ? FALSE : TRUE,
  );
  $form['gravatar_display']['gravatar_imagedefault'] = array(
    '#type'          => 'radios',
    '#title'         => t('Default avatar image'),
    '#description'   => t('Specifies an image that should be returned if either the requested email address has no associated Gravatar, or that Gravatar has a rating higher than is allowed by the "rating" parameter.'),
    '#options'       => array(
      //0 => t('No default image'), sound nice in theory, but there is no way of knowing 'no' gravatar is available
      1 => t("Drupal default user image (!user_picture_default_setting)", array('!user_picture_default_setting' => l($user_picture_default_setting_text, 'admin/user/settings', array('fragment' => 'edit-user-picture-default-wrapper')))),      
      2 => t('Module default user image'),
      3 => t('Image provided by Gravatar.com'),
    ),
    '#default_value' => variable_get('gravatar_imagedefault', 2),
    '#suffix'        => '<div id="default-gravatar-preview">' . _gravatar_admin_settings_previewimage() . '</div>',
    '#disabled'      => variable_get('gravatar_toggle', 0) ? FALSE : TRUE,
  );
  // "identicon" patch
  $form['gravatar_display']['gravatar_default_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Generated default gravatar type'),
    '#description'   => t('By selecting a type of avatar, each user will have associated an static automatically generated gravatar of that type which will be his/her identity'),
    '#options'       => array(
      1 => 'identicon',
      2 => 'wavatar',
      3 => 'monsterid',
      4 => 'default',
    ),
    '#default_value' => variable_get('gravatar_default_type', 4),
    '#disabled'      => (variable_get('gravatar_imagedefault', 1) == 3) ? FALSE : TRUE,
  );

  return system_settings_form($form);
} // gravatar_admin_settings

/**
 * Admin settings for the Gravatar module (advanced)
 */
function gravatar_admin_advanced_settings() {
  $form = array();
  $form['gravatar_preferences_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
  );
  $form['gravatar_preferences_advanced']['gravatar_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar URL'),
    '#default_value' => variable_get('gravatar_url', GRAVATAR_SERVICEURL),
    '#description' => t("If you are not sure don't modify this !")
  );
  return system_settings_form($form);
} // gravatar_admin_advanced_settings

/**
 * Tools page (and form builder)
 */
function gravatarcheckemail_form() {
  $form['email'] = array(
    '#title' => t('Email'),
    '#type'  => 'textfield',
  );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Check !'),
  );
  return $form;
} // gravatarcheckemail_form

function gravatarcheckemail_form_validate($form, &$form_state) {
  if (empty($form['email']['#value']) || !valid_email_address($form['email']['#value'])) {
    form_error($form, t('Please enter a valid email address.'));    
  }
} // gravatarcheckemail_form_validate

function gravatarcheckemail_form_submit($form, &$form_state) {
  drupal_goto('http://en.gravatar.com/site/check/' . $form_state['values']['email']);
} // gravatarcheckemail_form_submit

/**
 * Map pixel size to helper string
 * 
 * Helper function 
 *
 * @param (int) $size
 *   pixel size
 * @return (string)
 *   helper text
 */
function _gravatar_admin_settings_map_size ($size) {
  return str_replace('@num', $size, '@numx@num px');
} // _gravatar_admin_settings_map_size

/**
 * Previews the selected replacement image in the form
 *
 * @return (string)
 *   html formatted string
 */
function _gravatar_admin_settings_previewimage () {
  $option = variable_get('gravatar_imagedefault', 0);
  $size = variable_get('gravatar_displaysize', 48);

  switch ($option) {
  // system default image
  case 1:
  $image = variable_get('user_picture_default', '');
  break;
  // module default image
  case 2:
    $image = drupal_get_path('module', 'gravatar') . '/avatar.png';
    break;
  // please try not to abuse this...
  case 3:
    //$image = variable_get('gravatar_url', GRAVATAR_SERVICEURL);
    // "identicon" patch
    $image = variable_get('gravatar_url', GRAVATAR_SERVICEURL) . _gravatar_translate_type();
    break;
  default:
    $image = '';
    break;
  }

  if (!empty($image)) {
    return theme('image', $image, t('Default gravatar preview'), t('Default gravatar preview'), array('width' => $size, 'height' => $size), FALSE); 
  }
} // _gravatar_admin_settings_previewimage
