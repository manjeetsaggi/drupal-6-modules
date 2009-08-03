<?php
// $Id: perm.inc,v 1.6 2009/07/10 17:06:10 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based on user permission strings.
 */

/**
 * Implementation of specially named hook_ctools_access().
 */
function ctools_perm_ctools_access() {
  $args['perm'] = array(
    'title' => t("User: permission"),
    'description' => t('Control access by permission string.'),
    'callback' => 'ctools_perm_ctools_access_check',
    'default' => array('perm' => 'access content'),
    'settings form' => 'ctools_perm_ctools_access_settings',
    'summary' => 'ctools_perm_ctools_acesss_summary',
    'required context' => new ctools_context_required(t('User'), 'user'),
  );

  return $args;
}

/**
 * Settings form for the 'by perm' access plugin
 */
function ctools_perm_ctools_access_settings(&$form, &$form_state, $conf) {
  $perms = array();
  // Get list of permissions
  foreach (module_list(FALSE, FALSE, TRUE) as $module) {
    // By keeping them keyed by module we can use optgroups with the
    // 'select' type.
    if ($permissions = module_invoke($module, 'perm')) {
      $perms[$module] = drupal_map_assoc($permissions);
    }
  }

  $form['settings']['perm'] = array(
    '#type' => 'select',
    '#options' => $perms,
    '#title' => t('Permission'),
    '#default_value' => $conf['perm'],
    '#description' => t('Only users with the selected permission flag will be able to access this.'),
  );
}

/**
 * Check for access.
 */
function ctools_perm_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }

  return user_access($conf['perm'], $context->data);
}

/**
 * Provide a summary description based upon the checked roles.
 */
function ctools_perm_ctools_acesss_summary($conf, $context) {
  if (!isset($conf['perm'])) {
    return t('Error, unset permission');
  }

  return t('@identifier has "@perm"', array('@identifier' => $context->identifier, '@perm' => $conf['perm']));
}
