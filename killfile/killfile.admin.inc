<?php
// $Id: killfile.admin.inc,v 1.1.2.1 2008/10/28 13:05:09 arto Exp $

//////////////////////////////////////////////////////////////////////////////
// Settings form

function killfile_admin_settings() {
  $form = array();

  // Warn the administrator in case there is expired content in the killfile:
  if (($lifetime = variable_get('killfile_lifetime', 0)) != 0 && ($expired_count = killfile_node_count($lifetime)) > 0) {
    drupal_set_message(format_plural($expired_count, 'There is currently 1 expired node in the killfile. You can use the button below to purge it.', 'There are currently @count expired nodes in the killfile. You can use the button below to purge them.'), 'warning');
  }

  // Content settings
  $form['node'] = array('#type' => 'fieldset', '#title' => t('Content settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['node']['killfile_log'] = array( // TODO: make this a per-content-type setting?
    '#type'          => 'radios',
    '#title'         => t('Log message on deletion'),
    '#options'       => array('hidden' => t('Hidden'), 'optional' => t('Optional'), 'required' => t('Required')),
    '#default_value' => variable_get('killfile_log', 'optional'),
    '#description'   => t('Whether to request a rationale from the user prior to allowing them to delete content. If enabled, the provided rationale will be recorded in the killfile and the node revision log.'),
  );

  // Expiration settings
  $form['purge'] = array('#type' => 'fieldset', '#title' => t('Expiration settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['purge']['killfile_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Purge killfiled content after'),
    '#default_value' => variable_get('killfile_lifetime', 0),
    '#options' => array_merge(array('<' . t('never') . '>'), drupal_map_assoc(array(60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400, 2*86400, 3*86400, 4*86400, 5*86400, 7*86400, 14*86400, 28*86400), 'format_interval')),
    '#description' => t('Select how long to retain killfiled content before it is permanently purged. Note that purging killfiled content that has expired happens via cron, so in order for this to work you must have correctly set up your cron runs.'),
  );
  $form['purge']['purge'] = array(
    '#type' => 'submit',
    '#value' => t('Purge killfiled content now'),
    '#submit' => array('killfile_admin_purge_submit'),
    '#description' => t('This will purge all currently killfiled content, regardless of how long ago it was killfiled.'),
  );

  // Views integration
  if (module_exists('views')) {
    $form['views'] = array('#type' => 'fieldset', '#title' => t('Views integration'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['views']['killfile_views_filter'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Automatically filter out killfiled content from all views'),
      '#default_value' => variable_get('killfile_views_filter', '1'),
    );
  }

  return system_settings_form($form);
}

function killfile_admin_settings_validate($form, &$form_state) {
  extract($form_state['values'], EXTR_SKIP | EXTR_REFS);
}

function killfile_admin_purge_submit($form, &$form_state) {
  drupal_set_message(format_plural(killfile_node_purge_all(), 'Purged 1 expired node from the killfile.', 'Purged @count expired nodes from the killfile.'));
}
