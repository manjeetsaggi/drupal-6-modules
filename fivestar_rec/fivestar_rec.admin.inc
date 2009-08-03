<?php
// $Id: fivestar_rec.admin.inc,v 1.1 2009/07/20 21:49:08 danithaca Exp $

/**
 * @see http://drupal.org/project/fivestar_rec
 */

function fivestar_rec_settings_form() {
  $form = array();

  $form['enable_user2user'] = array(
    '#title' => t('Enable user-to-user algorithm.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('fivestar_rec_enable_user2user', 1),
    '#description' => t('Check this box to enable 2 blocks: 1) Users who voted similarly as you; 2) Recommended nodes from users who voted similarly as you. Uncheck this box to save offline computation time.')
  );

  $form['enable_item2item'] = array(
    '#title' => t('Enable item-to-item algorithm.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('fivestar_rec_enable_item2item', 1),
    '#description' => t('Check this box to enable 2 blocks: 1) Other nodes voted similar to this one; 2) You might also interest in these nodes based on your previous votes. Uncheck this box to save offline computation time.')
  );

  $form['display_num'] = array(
    '#title' => t('Number of items to display in the blocks.'),
    '#type' => 'select',
    '#default_value' => variable_get('fivestar_rec_display_num', 5),
    '#options' => array(
      3 => '3',
      5 => '5',
      8 => '8',
      10 => '10',
      15 => '15',
    ),
    '#description' => t('Please specify how many items to show in the recommendation block list.')
  );

  $form['same_type'] = array(
    '#title' => t('Limit display only the same content type?'),
    '#type' => 'radios',
    '#default_value' => variable_get('fivestar_rec_same_type', 1),
    '#options' => array(
      1 => 'Yes',
      0 => 'No',
    ),
    '#description' => t('If you select yes, only nodes with the same content type will be displayed in the recommendation blocks. Otherwise, any nodes with Fivestar support could be displayed.')
  );

  /*$form['boost_recency'] = array(
    '#title' => t('Boost recent items'),
    '#type' => 'checkbox',
    '#description' => t('Check the box to boost recent node browsing history and recent comment history (if enabled).'),
    '#default_value' => variable_get('history_rec_boost_recency', 1),
  );*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

function fivestar_rec_settings_form_submit($form, &$form_state) {
  variable_set('fivestar_rec_enable_user2user', $form_state['values']['enable_user2user']);
  variable_set('fivestar_rec_enable_item2item', $form_state['values']['enable_item2item']);
  variable_set('fivestar_rec_display_num', $form_state['values']['display_num']);
  variable_set('fivestar_rec_same_type', $form_state['values']['same_type']);
  //variable_set('history_rec_boost_recency', $form_state['values']['boost_recency']);

  drupal_set_message(t("The configuration options have been saved."));
}