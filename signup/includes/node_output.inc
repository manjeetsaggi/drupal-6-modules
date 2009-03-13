<?php
// $Id: node_output.inc,v 1.1.2.2 2008/12/20 06:31:27 dww Exp $


/**
 * @file
 * Code used to generate singup-related output when viewing nodes.
 */

/**
 * Generate all the signup-related output for a given node.
 *
 * Because of the global setting to control if the signup details and form
 * appear at the bottom of the node or on a separate tab, this function is
 * shared by multiple callers.
 *
 * @param $node
 *   The fully loaded node object.
 * @param $type
 *   The kind of output would we render: can be either 'node' or 'tab'.
 *
 * @return
 *   The fully rendered HTML for all signup-related forms and info.
 *
 * @see signup_nodeapi()
 * @see signup_node_tab()
 *
 * @todo This needs to be much more theme-friendly.
 *
 */
function _signup_node_output($node, $type = 'node') {
  $output = theme('signup_node_output_header', $node);
  $output .= _signup_current_user_signup($node, $type);
  return $output;
}

/**
 * Helper function to generate the output for the current user's signup.
 */
function _signup_current_user_signup($node, $type = 'node') {
  global $user;
  $output = '';
  // The node has been closed for signups, and the user has
  // signup permissions.  Let them know it's closed.
  if (!$node->signup_status) {
    if (user_access('sign up for content')) {
      $current_signup = '';
      // If they're logged in and already signed up, show their current
      // signup info and give them the option to cancel.
      if ($user->uid) {
        $result = db_query("SELECT * FROM {signup_log} WHERE uid = %d AND nid = %d", $user->uid, $node->nid);
        $signup_info = db_fetch_object($result);
        if (!empty($signup_info)) {
          $current_signup = _signup_print_current_signup($node, $signup_info);
        }
      }
      $output .= theme('signup_signups_closed', $node, $current_signup);
    }
  }
  else {
    $fieldset = $type == 'node' ? TRUE : FALSE;
    if ($user->uid == 0) {
      // This is an anonymous user.
      if (user_access('sign up for content')) {
        // If they can signup, render the anonymous sigup form.
        module_load_include('inc', 'signup', 'includes/signup_form');
        $output .= drupal_get_form('signup_form', $node, 'anon', $fieldset);
      }
      else {
        // If not, then display the appropriate login/register link if the
        // default authenticated user role can signup.
        $anon_login_text = '';
        $signup_roles = user_roles(FALSE, 'sign up for content');
        if (!empty($signup_roles[DRUPAL_AUTHENTICATED_RID])) {
          $token_array = array(
            '!login' => l(t('login'), 'user/login', array('query' => drupal_get_destination())),
            '!register' => l(t('register'), 'user/register', array('query' => drupal_get_destination())),
            '%node_type' => node_get_types('name', $node->type),
          );
          if (variable_get('user_register', 1) == 0) {
            $anon_login_text = t('Please !login to sign up for this %node_type.', $token_array);
          }
          else {
            $anon_login_text = t('Please !login or !register to sign up for this %node_type.', $token_array);
          }
        }
        $output .= theme('signup_anonymous_user_login_text', $anon_login_text);
      }
    }
    else {
      // An authenticated user.

      // See if the user is already signed up for this node.
      $result = db_query("SELECT * FROM {signup_log} WHERE uid = %d AND nid = %d", $user->uid, $node->nid);
      $signup_info = db_fetch_object($result);
      if (empty($signup_info)) {
        // Not yet signed up
        if (user_access('sign up for content')) {
          // User has permission to do so, so give them the form.
          module_load_include('inc', 'signup', 'includes/signup_form');
          $output .= drupal_get_form('signup_form', $node, 'auth', $fieldset);
        }
      }
      else {
        // Already signed up, display their info.
        $output .= _signup_print_current_signup($node, $signup_info);
      }
    }
  }
  return $output;
}

/**
 * Helper function to generate the list of users signed up for a node.
 */
function signup_user_list_page($node) {
  $output = '';

  // How should the list of signed-up users be displayed, if at all?
  $display_list = variable_get('signup_display_signup_user_list', 'signup');

  // If the user has the view signups perm and the admin decides to display
  // the list at the bottom of the page, display the current signups.
  if (user_access('view all signups')) {
    if ($display_list == 'signup') {
      // Admin wants the hard-coded signup listing.
      $registered_query = db_query("SELECT u.uid, u.name, s.signup_time, s.form_data FROM {signup_log} s INNER JOIN {users} u ON u.uid = s.uid WHERE s.nid = %d AND u.uid <> 0", $node->nid);
      $registered_signups = array();
      while ($signed_up_user = db_fetch_object($registered_query)) {
        $registered_signups[] = $signed_up_user;
      }
      $anon_query = db_query("SELECT * FROM {signup_log} WHERE nid = %d AND uid = 0", $node->nid);
      $anon_signups = array();
      while ($signed_up_user = db_fetch_object($anon_query)) {
        $anon_signups[] = $signed_up_user;
      }
      $output .= theme('signup_user_list', $node, $registered_signups, $anon_signups);
    }
    elseif ($display_list == 'embed-view' && module_exists('views')) {
      $signup_view = variable_get('signup_user_list_view', 'signup_user_list:page');
      $signup_view_parts = explode(':', $signup_view);
      $view_name = $signup_view_parts[0];
      $view_display = $signup_view_parts[1];
      $view = views_get_view($view_name);
      $view_args = array($node->nid);
      $output .=  $view->preview($view_display, $view_args);
    }
    // Otherwise, they're on their own, and either don't want it displayed at
    // all, or they want to handle where/how it's displayed via views.
  }
  return $output;
}

/**
 * Page handler for the optional 'signup' tab on nodes.
 *
 * This is only used if the site has configured the signup form and related
 * output to appear on a separate tab, instead of directly embedded in the
 * node.
 *
 * @param $node
 *   The node to generate a signup tab for.
 *
 * @return
 *   The contents of the signup tab.
 *
 * @see _signup_node_output()
 */
function signup_node_tab($node) {
  drupal_set_title(check_plain($node->title));
  return _signup_node_output($node, 'tab');
}

/**
 * Helper function to display the current user's signup information.
 *
 * Contains the logic to determine what should be displayed, then invokes the
 * appropriate form builder and theme functions.
 *
 * @param $node
 *   The fully-loaded node object to display signup data about.
 * @param $signup_info
 *   Database object with information about the signup to display.
 *
 * @return
 *   Themed HTML to output for the given node and signup.
 *
 * @see theme_signup_current_signup()
 */
function _signup_print_current_signup($node, $signup_info) {
  // Generate an array of data about the current signup for the theme function.
  $signup_data = array();
  $signup_data['custom_data'] = unserialize($signup_info->form_data);
  $signup_data['signup_timestamp'] = $signup_info->signup_time;

  // See if the current user is allowed to cancel their signup, and if so,
  // render the HTML for the cancel form (which is just a single button).
  $cancel_signup_form = '';
  if (user_access('cancel own signups')) {
    module_load_include('inc', 'signup', 'includes/signup_cancel_form');
    $cancel_signup_form = drupal_get_form('signup_cancel_form', $signup_info->sid);
  }

  // Hand off everything to the theme function for actual HTML generation.
  return theme('signup_current_signup', $signup_data, $cancel_signup_form);
}
