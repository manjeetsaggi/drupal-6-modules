<?php
// $Id: gravatar.admin.inc,v 1.1.2.29 2009/03/20 16:56:01 davereid Exp $

/**
 * @file
 * Administrative page callbacks for the gravatar module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function gravatar_admin_settings() {
  $form = array();

  // Display settings.
  $form['display'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Display'),
  );
  $form['display']['gravatar_size'] = array(
    '#type'          => 'item',
    '#title'         => t('Image size'),
    '#description'   => t('The preferred image size (maximum 512 pixels). This setting can be adjusted in the <a href="@user-picture-link">user pictures settings</a>.', array('@user-picture-link' => url('admin/user/settings', array('fragment' => 'edit-user-picture-default')))),
    '#value'         => t('@sizex@size pixels', array('@size' => _gravatar_get_size())),
  );
  $form['display']['gravatar_rating'] = array(
    '#type'          => 'select',
    '#default_value' => gravatar_var('rating'),
    '#options'       => drupal_map_assoc(array('G', 'PG', 'R', 'X')),
    '#title'         => t('Image maturity filter'),
    '#description' => theme('item_list', array(
      t('G: Suitable for display on all websites with any audience type.'),
      t('PG: May contain rude gestures, provocatively dressed individuals, the lesser swear words, or mild violence.'),
      t('R: May contain such things as harsh profanity, intense violence, nudity, or hard drug use.'),
      t('X: May contain hardcore sexual imagery or extremely disturbing violence.'),
    )),
  );
  $form['display']['gravatar_default'] = array(
    '#type'          => 'radios',
    '#title'         => t('Default image'),
    '#description'   => t('Specifies an image that should be returned if either the requested e-mail address has no associated gravatar, or that gravatar has a rating higher than is allowed by the maturity filter.'),
    '#options'       => array(
      GRAVATAR_DEFAULT_GLOBAL => t('Global default user image'),
      GRAVATAR_DEFAULT_MODULE => t('Module default image (white background)'),
      GRAVATAR_DEFAULT_MODULE_CLEAR => t('Module default image (transparent background)'),
      GRAVATAR_DEFAULT_IDENTICON => t('Gravatar.com identicon (generated)'),
      GRAVATAR_DEFAULT_WAVATAR => t('Gravatar.com wavatar (generated)'),
      GRAVATAR_DEFAULT_MONSTERID => t('Gravatar.com monsterid (generated)'),
      GRAVATAR_DEFAULT_LOGO => t('Gravatar.com logo'),
    ),
    '#prefix' => '<div class="picture js-show">'. theme('image', '', t('Default picture example'), t('Default picture example'), array('id' => 'gravatar-imagepreview'), FALSE) .'</div>',
    '#process' => array('expand_radios', 'gravatar_process_default_setting'),
  );

  // Add JavaScript and CSS to swap the defalut image previews.
  drupal_add_js(drupal_get_path('module', 'gravatar') .'/gravatar.js');
  drupal_add_css(drupal_get_path('module', 'gravatar') .'/gravatar.css');

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t('Do not modify these options unless you know what you are doing!'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['gravatar_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Gravatar URL'),
    '#default_value' => gravatar_var('url'),
  );

  return system_settings_form($form);
}

function gravatar_process_default_setting($element) {
  $element[GRAVATAR_DEFAULT_GLOBAL]['#description'] = t('This setting can be adjusted in the <a href="@user-picture-link">user pictures settings</a>.', array('@user-picture-link' => url('admin/user/settings', array('fragment' => 'edit-user-picture-default'))));
  // If the global user picture is empty, disable the respective option.
  if (!variable_get('user_picture_default', '')) {
    $element[GRAVATAR_DEFAULT_GLOBAL]['#disabled'] = TRUE;
    $element[GRAVATAR_DEFAULT_GLOBAL]['#description'] = t('There currently is not a global default user picture specified.') .' '. $option_element['#description'];
  }

  foreach ($element['#options'] as $key => $choice) {
    // Add an image to preview this default image option.
    $attributes = array(
      'id' => 'gravatar-imagepreview-'. $option_index,
      // Hide the image if the respective option is disabled.
      'class' => $option_element['#disabled'] ? 'hide' : 'js-hide',
    );
    $element[$key]['#suffix'] = theme('image', _gravatar_get_gravatar(array('default' => _gravatar_get_default_image($option_index))), $option_title, $option_title, $attributes, FALSE);
  }

  return $element;
}
