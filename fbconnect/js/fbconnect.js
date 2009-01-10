
/*
 * The facebook_onload statement is printed out in the PHP. If the user's logged in
 * status has changed since the last page load, then refresh the page to pick up
 * the change.
 *
 * This helps enforce the concept of "single sign on", so that if a user is signed into
 * Facebook when they visit your site, they will be automatically logged in -
 * without any need to click the login button.
 *
 * @param already_logged_into_facebook  reports whether the server thinks the user
 *                                      is logged in, based on their cookies
 *
 */
function facebook_onload(already_logged_into_facebook) {
  // user state is either: has a session, or does not.
  // if the state has changed, detect that and reload.
  FB.ensureInit(function() {
      FB.Facebook.get_sessionState().waitUntilReady(function(session) {
          var is_now_logged_into_facebook = session ? true : false;

          // if the new state is the same as the old (i.e., nothing changed)
          // then do nothing
          if (is_now_logged_into_facebook == already_logged_into_facebook) {
            return;
          }

          // otherwise, refresh to pick up the state change
          refresh_page();
        });
    });
}

/*
 * Our <fb:login-button> specifies this function in its onlogin attribute,
 * which is triggered after the user authenticates the app in the Connect
 * dialog and the Facebook session has been set in the cookies.
 */
function facebook_onlogin_ready() {
  // In this app, we redirect the user back to index.php. The server will read
  // the cookie and see that the user is logged in, and will deliver a new page
  // with content appropriate for a logged-in user.
  //
  // However, a more complex app could use this function to do AJAX calls
  // and/or in-place replacement of page contents to avoid a full page refresh.
  refresh_page();
}

/*
 * Do a page refresh after login state changes.
 * This is the easiest but not the only way to pick up changes.
 * If you have a small amount of Facebook-specific content on a large page,
 * then you could change it in Javascript without refresh.
 */
function refresh_page() {
  window.location = '';
}

/*
 * Prompts the user to grant a permission to the application.
 */
function facebook_prompt_permission(permission) {
  FB.ensureInit(function() {
    FB.Connect.showPermissionDialog(permission);
  });
}

/*
 * Show the feed form. This would be typically called in response to the
 * onclick handler of a "Publish" button, or in the onload event after
 * the user submits a form with info that should be published.
 *
 */
function facebook_publish_feed_story(form_bundle_id, template_data) {
  // Load the feed form
  FB.ensureInit(function() {
          FB.Connect.showFeedDialog(form_bundle_id, template_data);
          //FB.Connect.showFeedDialog(form_bundle_id, template_data, null, null, FB.FeedStorySize.shortStory, FB.RequireConnect.promptConnect);

      // hide the "Loading feed story ..." div
     // ge('feed_loading').style.visibility = "hidden";
  });
}
