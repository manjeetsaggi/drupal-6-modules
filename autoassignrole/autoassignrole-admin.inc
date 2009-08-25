<?php
// $Id: autoassignrole-admin.inc,v 1.1.2.10 2009/08/12 17:35:28 cyberswat Exp $

/**
 * @file
 *
 * The autoassignrole-admin.inc file
 *
 * Contains the admin form and associated functionality for the autoassignrole
 * module
 */

function autoassignrole_admin_form() {
  $form['autoassignrole_settings_auto'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic Role Assignment'),
    '#description' => t('Automatically assigned roles will be attached to
      accounts created through the administrative interface as well as normal
      new user registration according to the following settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['autoassignrole_settings_auto']['auto_active'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic role assignment'),
    '#default_value' => _autoassignrole_get_settings('auto_active'),
    '#description' => t('Automatic role assignment occurs when the user first
      logins to the account.  This happens without the users knowledge.  Set to
      Enabled to allow this functionality or Disabled to not allow.'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
  );
/*
   $form['autoassignrole_settings_auto']['auto_admin_active'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic role assignment for admin created accounts'),
    '#default_value' => _autoassignrole_get_settings('auto_admin_active'),
    '#description' => t('Automatically assign roles if the user account is being
      created by an administrator.'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
  );
*/
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  if ($roles) {
    $form['autoassignrole_settings_auto']['auto_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => _autoassignrole_get_settings('auto_roles'),
      '#description' => t('Check the specific Roles the user will automatically
        be assigned to when created by an administrator or through the new user
        registration process.  The Authenticated User role is automatically
        assigned by Drupal core and can not be edited.'),
      '#options' => $roles,
    );
  }

  $form['autoassignrole_settings_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Assign from Path'),
    '#description' => t('Assign user roles based on the path the user
      is creating the account from.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $result = db_query("SELECT rid, display, path, title, description, format, weight, menu FROM {autoassignrole_page}");
  $defaults = array();
  while ($setting = db_fetch_object($result)) {
    $defaults[$setting->rid] = array(
      'display' => $setting->display,
      'path' => $setting->path,
      'title' => $setting->title,
      'description' => $setting->description,
      'format' => $setting->format,
      'weight' => $setting->weight,
      'menu' => $setting->menu
    );
  }
  foreach ($roles as $k => $v) {
    $active = (isset($defaults[$k]['display']) ? 1 : 0);
    $form['autoassignrole_settings_page'][$k] = array(
      '#type' => 'fieldset',
      '#title' => $v,
      '#collapsible' => TRUE,
      '#collapsed' => ($active ? FALSE : TRUE),
    );
    $form['autoassignrole_settings_page'][$k]["path_active_$k"] = array(
      '#type' => 'radios',
      '#title' => check_plain($v) .' '. t('status'),
      '#default_value' => $active,
      '#description' => t('Enable or disable this role for path based role assignment'),
      '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
    );
  $form['autoassignrole_settings_page'][$k]["path_display_$k"] = array(
    '#type' => 'radios',
    '#title' => t('Display Method'),
    '#default_value' => (isset($defaults[$k]['display']) ? $defaults[$k]['display'] : 0),
    '#description' => t('Menu Items are displayed as normal menu items.  Tabs on registration pages display on the standard user/* pages.  Pages with no navigation do not display in the menu and rely on you to provide a method for end users to reach them.'),
    '#options' => array(0 => t('Menu Items'), 1 => t('Tabs on registration page'), 2 => 'Pages with no navigation'),
  );
  $form['autoassignrole_settings_page'][$k]["path_menu_$k"] = array(
    '#type' => 'select',
    '#title' => t('Menu'),
    '#default_value' => $defaults[$k]['menu'],
    '#description' => t('If this path is going to be displayed as a menu item which parent menu should it appear under?  This setting will be ignored for all Display Methods except Menu Items.'),
    '#options' => menu_get_menus(),
  );
   $form['autoassignrole_settings_page'][$k]["path_weight_$k"] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => (isset($defaults[$k]['weight']) ? $defaults[$k]['weight'] : '0'),
    '#delta' => 10,
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
  );
    $form['autoassignrole_settings_page'][$k]["path_$k"] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#description' => t('The Drupal path for this role.  This is a relative path based on your Drupal installation.'),
      '#default_value' => (isset($defaults[$k]['path']) ? $defaults[$k]['path'] : ''),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );
    $form['autoassignrole_settings_page'][$k]["path_title_$k"] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('The title that will be displayed to users in menus, if applicable, and on the path landing page.'),
      '#default_value' => (isset($defaults[$k]['title']) ? $defaults[$k]['title'] : ''),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );
    $form['autoassignrole_settings_page'][$k]["path_description_$k"] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#description' => t('The description displayed to the end user when they are
        selecting thier role during registration.  This description is filtered
        in a similar fashion to node body content.'),
      '#default_value' => (isset($defaults[$k]['description']) ? $defaults[$k]['description'] : ''),
      '#required' => FALSE,
    );
    $format = (isset($defaults[$k]['format']) ? $defaults[$k]['format'] : FILTER_FORMAT_DEFAULT);
    $form['autoassignrole_settings_page'][$k]["format"] = filter_form($format);
    foreach ($form['autoassignrole_settings_page'][$k]["format"] as $key => $value) {
      if (is_numeric($key) && isset($value['#parents'][0])) {
        $form['autoassignrole_settings_page'][$k]["format"][$key]['#parents'][0] = "path_format_$k";
      }
    }
  }

  $form['autoassignrole_settings_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allow User to Choose'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['autoassignrole_settings_user']['user_active'] = array(
    '#type' => 'radios',
    '#title' => t('User Role Assignment'),
    '#default_value' => _autoassignrole_get_settings('user_active'),
    '#description' => t('The end user will be allowed to select the following
      roles when they log in.'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
  );
  if ($roles) {
    $form['autoassignrole_settings_user']['user_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => _autoassignrole_get_settings('user_roles'),
      '#options' => $roles,
    );
  }

  $form['autoassignrole_settings_user']['selection']['user_multiple'] = array(
    '#type' => 'radios',
    '#title' => t('User Role Selection'),
    '#default_value' => _autoassignrole_get_settings('user_multiple'),
    '#description' => t('Should the end user be allowed to choose a single role
      or can they choose multiple roles?'),
    '#options' => array(0 => t('Single Role'), 1 => t('Multiple Roles')),
  );
  $form['autoassignrole_settings_user']['user_selection'] = array(
    '#type' => 'radios',
    '#title' => t('Selection Method'),
    '#default_value' => _autoassignrole_get_settings('user_selection'),
    '#description' => t('The type of form elements the end user will be presented with.'),
    '#options' => array(0 => t('Radio Buttons'), 1 => t('Selection Box'), 2 => t('Check Boxes')),
  );
  $form['autoassignrole_settings_user']['user_required'] = array(
    '#type' => 'radios',
    '#title' => t('Required'),
    '#default_value' => _autoassignrole_get_settings('user_required'),
    '#description' => t('Should the end user be required to choose a role?'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
  );
  $form['autoassignrole_settings_user']['user_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sorting'),
    '#default_value' => _autoassignrole_get_settings('user_sort'),
    '#description' => t('Default sort order of roles the user will see.'),
    '#options' => array(
      'SORT_ASC' => t('Ascending'),
      'SORT_DESC' => t('Descending')
    ),
  );
  $form['autoassignrole_settings_user']['user_fieldset_title'] = array(
    '#type' => 'textfield',
    '#title' => t('User Role Fieldset Title'),
    '#description' => t('The title of the fieldset that contains role
      options.'),
    '#default_value' => _autoassignrole_get_settings('user_fieldset_title'),
    '#size' => 60,
    '#maxlength' => 128
  );
  $form['autoassignrole_settings_user']['user_title'] = array(
    '#type' => 'textfield',
    '#title' => t('User Role Title'),
    '#description' => t('The title of the field that contains the role options
      the end user sees during registration.'),
    '#default_value' => _autoassignrole_get_settings('user_title'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['autoassignrole_settings_user']['user_description'] = array(
    '#type' => 'textarea',
    '#title' => t('User Role Description'),
    '#description' => t('The description displayed to the end user when they are
      selecting thier role during registration.'),
    '#default_value' => _autoassignrole_get_settings('user_description'),
    '#required' => FALSE,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}
function autoassignrole_admin_form_validate($form_id, $form_values) {

  // path based role validation
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  foreach ($roles as $rid => $role) {
    if ($form_values['values']["path_active_$rid"]) {
      $format = $form_values['values']["path_format_$rid"];
      $path = $form_values['values']["path_$rid"];
      $title = $form_values['values']["path_title_$rid"];
      $description = $form_values['values']["path_description_$rid"];
      $display = $form_values['values']["path_display_$rid"];
      
      // Check that a display option is set
      if ($display == '') {
        form_set_error("path_display_$rid", t('Please select a valid display method for enabled URL role assignments'));
      }

      // check that path exists, is syntactically correct, and that it is not
      // an existing Drupal path
      if ($path) {
        if (ereg("[^A-Za-z0-9/_-]+", $path) || strpos(trim($path), '/') === 0) {
          form_set_error("path_$rid", '\''. check_plain($path) .'\' '. t('is not a valid path'));
        }
        else {
          $menu_link = db_fetch_object(db_query("SELECT count(mlid) AS count FROM {menu_links} where link_path like '%s'", $path));
          if (drupal_lookup_path('source', $path) || $menu_link->count > 0) {
            // the menu item exists so need to check if the path has
            // something other than autoassign_role_path registered
            // otherwise throw an error
            $menu_item = menu_get_item($path);
            if (!isset($menu_item['page_callback']) && $menu_item['page_callback'] != 'autoassign_role_path') {
              form_set_error("path_$rid", '\''. check_plain($path) .'\' '. t('is already in use as a path'));
            }
          }
          
          // if the menu items are being displayed as tabs on user/* the path can
          // not be constructed in a way that makes the tab a child of an existing
          // tab.  register/* and password/* will fail
          if (($display == 1) && (strpos(trim($path), 'register') === 0 || strpos(trim($path), 'password') === 0)) {
            form_set_error("path_$rid", '\''. check_plain($path) .'\' '. t('can only be used as a path if the Display Method is not Tabs on registration page.'));
          }
        }
      }
      else {
        form_set_error("path_$rid", t('Please enter a valid path for enabled URL role assignments'));
      }

      // If the display option is a menu item or links we need a title to display
      if (($display == '0' || $display == '1')) {
        $title = ereg_replace('[:space:]', '', $title);
        if (!strlen($title)) {
          form_set_error("path_title_$rid", t('Please enter a valid title for enabled URL role assignments'));
        }
      }
    }
  }

  // User chooses their own role validation
  if ($form_values['values']['user_active'] == 1) {
    if ($form_values['values']['user_multiple'] == 0 &&
        $form_values['values']['user_selection'] == 2) {
      form_set_error('user_selection', t('If a user can only select a single role you need to set the selection method as Radio Buttons or Selection Box.'));
    }
    if ($form_values['values']['user_multiple'] == 1 &&
        $form_values['values']['user_selection'] == 0) {
      form_set_error('user_selection', t('If a user can select multiple roles they can not use Radio Buttons as the selection method.'));
    }
    if (strlen(trim($form_values['values']['user_title'])) == 0) {
      form_set_error('user_title', t('Enter the title of the form fields the user will be presented with.'));
    }
  }
}
function autoassignrole_admin_form_submit($form_id, $form_values) {
  $sql = "UPDATE {autoassignrole} set value = '%s' where arid = '%s'";
  db_query($sql, $form_values['values']['auto_active'], 'auto_active');
  db_query($sql, serialize($form_values['values']['auto_roles']), 'auto_roles');
  db_query($sql, $form_values['values']['user_active'], 'user_active');
  db_query($sql, serialize($form_values['values']['user_roles']), 'user_roles');
  db_query($sql, $form_values['values']['user_multiple'], 'user_multiple');
  db_query($sql, $form_values['values']['user_title'], 'user_title');
  db_query($sql, $form_values['values']['user_fieldset_title'], 'user_fieldset_title');
  db_query($sql, $form_values['values']['user_required'], 'user_required');
  db_query($sql, $form_values['values']['user_sort'], 'user_sort');
  db_query($sql, $form_values['values']['user_description'], 'user_description');
  db_query($sql, $form_values['values']['user_selection'], 'user_selection');


  // Role assignment based on path
  db_query("truncate {autoassignrole_page}");
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  foreach ($roles as $k => $v) {
    if ($form_values['values']["path_active_$k"]) {
      $path = $form_values['values']["path_$k"];
      $title = $form_values['values']["path_title_$k"];
      $description = $form_values['values']["path_description_$k"];
      $display = $form_values['values']["path_display_$k"];
      $format = $form_values['values']["path_format_$k"];
      $weight = $form_values['values']["path_weight_$k"];
      $menu = $form_values['values']["path_menu_$k"];
      db_query("INSERT INTO {autoassignrole_page} (rid, display, path, title, description, format, weight, menu) VALUES(%d, '%s', '%s', '%s', '%s', %d, %d, '%s')", $k, $display, $path, $title, $description, $format, $weight, $menu);
    }
  }
  // rebuild the menu
  menu_rebuild();
  drupal_set_message(t('The changes have been saved.'));
}