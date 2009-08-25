<?php

function og_panels_ct_list_mission() {
  $items['og_mission'] = array(
    'title' => t('Welcome message'),
    'icon' => 'images/user-multiple.png',
    'path' => drupal_get_path('module', 'og_panels'). '/',
    'description' => t('The welcome message for the group. Specified on group edit form.'),
    'category' => array(t('Organic groups'), -10),
    'required context' => new ctools_context_required(t('Group'), 'node'),
  );
  return $items;
}

function og_panels_ct_list_description() {
  $items['og_description'] = array(
    'title' => t('Group description'),
    'icon' => 'images/user-multiple.png',
    'path' => drupal_get_path('module', 'og_panels'). '/',
    'description' => t('The group description as specified on the group edit form.'),
    'category' => array(t('Organic groups'), -5),
    'required context' => new ctools_context_required(t('Group'), 'node'),
  );
  return $items;
}

function og_panels_ct_render_callback_mission($subtype, $conf, $panel_args, &$contexts) {
  dargs();
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block->module = 'og_panels';

  $block->subject = t('Welcome');
  if ($node) {
    // This rendering somewhat verbose technique exactly matches og_view_group().
    $value = check_markup($node->body, $node->format, FALSE);
    $form['og_mission'] = array(
      '#theme' => 'og_mission', 
      '#value' => $value,
      '#node' => $node,
      '#weight' => -3,
    );
    $block->content = drupal_render($form['og_mission']);
    
    $block->delta = $node->nid;
  }
  else {
    $block->content = t('Welcome statement goes here.');
    $block->delta = 'unknown';
  }

  return $block;
}

function og_panels_ct_render_callback_description($subtype, $conf, $panel_args, &$contexts) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block->module = 'og_panels';

  $block->subject = t('Description');
  if ($node) {
    $block->content = check_markup($node->og_description);
    $block->delta = $node->nid;
  }
  else {
    $block->content = t('Description goes here.');
    $block->delta = 'unknown';
  }

  return $block;
}

/**
 * Edit form for the mission content type.
 */
function og_panels_ct_edit_form_mission(&$form, &$form_state) {
  // Empty form to ensure we have the override title + context gadgets.
}

/**
 * Edit form for the description content type.
 */
function og_panels_ct_edit_form_description(&$form, &$form_state) {
  // Empty form to ensure we have the override title + context gadgets.
}
