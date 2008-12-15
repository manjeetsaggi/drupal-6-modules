<?php
// $Id: og_profiles.pages.inc,v 1.1 2008/07/29 08:39:09 henrysearle Exp $

/**
 * @file
 * User page callbacks for the organic groups profile module.
 */

/**
 * Menu callback; display a list of user information.
 *//*
function og_profiles_browse() {
  // Ensure that the path is converted to 3 levels always.
  list(, $name, $value) = array_pad(explode('/', $_GET['q'], 3), 3, '');

  $field = db_fetch_object(db_query("SELECT DISTINCT(fid), type, title, page, visibility FROM {og_profiles_fields} WHERE name = '%s'", $name));

  if ($name && $field->fid) {
    // Only allow browsing of fields that have a page title set.
    if (empty($field->page)) {
      drupal_not_found();
      return;
    }
    // Do not allow browsing of private and hidden fields by non-admins.
    if (!user_access('administer users') && ($field->visibility == og_profiles_PRIVATE || $field->visibility == og_profiles_HIDDEN)) {
      drupal_access_denied();
      return;
    }

    // Compile a list of fields to show.
    $fields = array();
    $result = db_query('SELECT name, title, type, weight, page FROM {og_profiles_fields} WHERE fid <> %d AND visibility = %d ORDER BY weight', $field->fid, og_profiles_PUBLIC_LISTINGS);
    while ($record = db_fetch_object($result)) {
      $fields[] = $record;
    }

    // Determine what query to use:
    $arguments = array($field->fid);
    switch ($field->type) {
      case 'checkbox':
        $query = 'v.value = 1';
        break;
      case 'textfield':
      case 'selection':
        $query = "v.value = '%s'";
        $arguments[] = $value;
        break;
      case 'list':
        $query = "v.value LIKE '%%%s%%'";
        $arguments[] = $value;
        break;
      default:
        drupal_not_found();
        return;
    }

    // Extract the affected users:
    $sql = "SELECT u.uid, u.access FROM {users} u INNER JOIN {og_profiles_values} v ON u.uid = v.uid WHERE v.fid = %d AND ". $query ." AND u.access <> 0 AND u.status <> 0 ORDER BY u.access DESC";
    $result = pager_query($sql, 20, 0, NULL, $arguments);

    $content = '';
    while ($account = db_fetch_object($result)) {
      $account = user_load(array('uid' => $account->uid));
      $profile = _og_profiles_update_user_fields($fields, $account);
      $content .= theme('og_profiles_listing', $account, $profile);
    }
    $output = theme('og_profiles_wrapper', $content);
    $output .= theme('pager', NULL, 20);

    if ($field->type == 'selection' || $field->type == 'list' || $field->type == 'textfield') {
      $title = strtr(check_plain($field->page), array('%value' => theme('placeholder', $value)));
    }
    else {
      $title = check_plain($field->page);
    }

    drupal_set_title($title);
    return $output;
  }
  else if ($name && !$field->fid) {
    drupal_not_found();
  }
  else {
    // Compile a list of fields to show.
    $fields = array();
    $result = db_query('SELECT name, title, type, weight, page FROM {og_profiles_fields} WHERE visibility = %d ORDER BY category, weight', og_profiles_PUBLIC_LISTINGS);
    while ($record = db_fetch_object($result)) {
      $fields[] = $record;
    }

    // Extract the affected users:
    $result = pager_query('SELECT uid, access FROM {users} WHERE uid > 0 AND status <> 0 AND access <> 0 ORDER BY access DESC', 20, 0, NULL);

    $content = '';
    while ($account = db_fetch_object($result)) {
      $account = user_load(array('uid' => $account->uid));
      $profile = _og_profiles_update_user_fields($fields, $account);
      $content .= theme('og_profiles_listing', $account, $profile);
    }
    $output = theme('og_profiles_wrapper', $content);
    $output .= theme('pager', NULL, 20);

    drupal_set_title(t('User list'));
    return $output;
  }
}*/

/*
 * function which outputs a member list of a group as a themed table
 * 
 * this should only show fields defined for this group that have the visibility set to
 * show in the menu list
 */
function og_profiles_member_list($group_id) {
  // first of all I need the list of fields for this group
  $sql = "SELECT fid, title, type FROM {og_profiles_fields} WHERE visibility = %d AND group_id = %d ORDER BY weight";
  $result = db_query($sql, og_profiles_PUBLIC_LISTINGS, $group_id);
  $sort_by = arg(4);
  if (!isset($sort_by)) $sort_by = 0;
  // We need to counter so we know which column we want to sort
  $counter = 0;
  // the username will always be shown
  $header[] = l(($sort_by == 0) ? $image = theme_tablesort_indicator('asc') .'User' : 'User', "og/users/$group_id/list/0", array('html' => TRUE));
  while ($field = db_fetch_array($result)) {
    $counter++;
    $fields[] = array('fid' => $field['fid'], 'type' => $field['type']);

    // generates the headers for the table based on the visibile field titles
    $header[] = l(($sort_by == $counter) ? $image = theme_tablesort_indicator('asc') . $field['title'] : $field['title'], "og/users/$group_id/list/$counter", array('html' => TRUE));
  }
  
  // as in og, the admin should be able to manage and make admin from the member list
  if (is_group_admin($group_id)) {
    $header[] = 'Manage';
    $header[] = 'Admin';
  }

  // now find all the profile values for this group
  $sql = 'SELECT {og_profiles_fields}.fid, uid, value, type FROM {og_profiles_fields} INNER JOIN {og_profiles_values} ON {og_profiles_fields}.fid={og_profiles_values}.fid  WHERE group_id = %d ORDER BY name';
  $result = db_query($sql, $group_id);
  while ($value = db_fetch_array($result)) {
    // arrange them by field id then user id so it is easier to reference later without if statements
      $profile_values[$value['fid']][$value['uid']] = array('value' => $value['value'], 'type' => $value['type']);
  }
  // find the group manager here instead of every time we look at a user
  $group_manager = group_manager($group_id);
  
  // now find all the users in the group and get whether they are an admin or not
  // order by username as it isn't possible to order by this later
  $sql = "SELECT {users}.uid, name, {og_uid}.is_admin FROM {og_uid} INNER JOIN {users} ON {og_uid}.uid={users}.uid WHERE nid = %d ORDER BY is_admin DESC, name ASC";
  $result = db_query($sql, $group_id);
  while ($member = db_fetch_array($result)) {
    // define this here to make referencing simpler later on
    $uid =  $member['uid'];
    // the username will always be shown
    $admin = ($member['is_admin']) ? 'Admin: ' : '';
    $row[] = $admin . l($member['name'], "user/$uid/$group_id");
    
    // iterate through all the fields for this group
    if (isset($fields)) {
      foreach ($fields as $value) {
        // if there is profile value for this user in this group
        if (isset($profile_values[$value['fid']][$uid]['value'])) {
          $cell_value = $profile_values[$value['fid']][$uid]['value'];
          }
        else {
          $cell_value = '';
        }
        // if this type is a checkbox and there is some information show yes or no instead of 1 or 0
        if ($value['type'] == 'checkbox' && isset($cell_value))
          if ($cell_value == '1') 
            $row[] = 'Yes';
          else
            $row[] = 'No';
        else
        // otherwise just show the value stored
        // we want this to happen even if there is a null value to preserve formatting on the table  
        $row[] = $cell_value;
      }
    }
    
    // if the user accessing the list is a group admin show links to make or remove admin
    // and remove from group
    // TODO add edit membership so the admin can change the users profile

    // if the user we are looking at is the group manager don't let the admin remove them or change admin status
    if (is_group_admin($group_id) && $uid != $group_manager) {
      $row[] = l('Remove', "og/unsubscribe/$group_id/$uid");
      
      if ($member['is_admin'] == 1)
        $row[] = l('Remove Admin', "og/delete_admin/$group_id/$uid");
      else 
        $row[] = l('Make Admin', "og/create_admin/$group_id/$uid");
    }
    // if we are looking at the group admin print "manger" isntead of a link
    else if (is_group_admin($group_id)) {
      $row[] = 'Manager';
      $row[] = 'Manager';
    }
    // this will generate the rows to export to the table
    $rows[] = $row;
    // clear out the row so it can start again
    unset($row); 
  }

  // function that usort uses 
  function sort_results($a, $b) {
    $sort_by = arg(4);
    // if the first array has no value then moce if down
    if ($a[$sort_by] == NULL) 
      return 1;
    // else if the second array has no value then move if down
    else if ($b[$sort_by] == NULL)
      return -1;
    // else do a case insensitve comparision and return that
    else{
      //TODO check what this does with numbers
      return strcasecmp($a[$sort_by], $b[$sort_by]);
    }
  }

  // if we are not ordering by username (which is already in order)
  if ($sort_by != 0)
    usort($rows, "sort_results");

  // return the header and rows as a thememed table
  return theme_table($header, $rows, array('width' => '100%'));
}

function group_manager($group_id) {
  $sql = 'SELECT uid FROM {node} where nid = %d';
  return db_result(db_query($sql, $group_id));
}

/**
 * Callback to allow autocomplete of profile text fields.
 */
function og_profiles_autocomplete($field, $string) {
  $matches = array();
  if (db_result(db_query("SELECT COUNT(*) FROM {og_profiles_fields} WHERE fid = %d AND autocomplete = 1", $field))) {
    $result = db_query_range("SELECT value FROM {og_profiles_values} WHERE fid = %d AND LOWER(value) LIKE LOWER('%s%%') GROUP BY value ORDER BY value ASC", $field, $string, 0, 10);
    while ($data = db_fetch_object($result)) {
      $matches[$data->value] = check_plain($data->value);
    }
  }

  drupal_json($matches);
}
