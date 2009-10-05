/*$Id: README.txt,v 1.2 2009/06/27 10:11:29 prajwala Exp $*/

== SUMMARY ==
Facebook stream module displays the facebook stream in a drupal block. To access the users stream the user has to give permission to our application. This module will the ask the user to give permissions if the user did not give permission to access his stream. The posts will update automatically after some time. This time can be configured by site administrator in the block configuration page

We can able to post status message and and can add comments and likes to the status messages.

Note: In order to publish to a user's stream, the user must grant your application the publish_stream extended permission. If the user previously granted your application the permission to publish short stories into the News Feed automatically, then you don't need to prompt for this permission in order to call this method.

Before you can prompt for the publish_stream permission, you need to apply for the whitelist(http://www.facebook.com/devhelp?category=Stream). Otherwise, the only Facebook users that can grant your application the publish_stream permission are the developers of your application. 
For more information check http://wiki.developers.facebook.com/index.php/Stream.publish

== REQUIREMENTS ==
PHP 5.2 or higher versions. 
Drupal 6.x. 
Facebook PHP Library: http://svn.facebook.com/svnroot/platform/clients/packages/facebook-platform.tar.gz 
Facebook API key: http://www.facebook.com/developers/
jquery_ui,jquery_update modules

To publish comments, likes, status messages you need to apply for the whitelist http://www.facebook.com/devhelp?category=Stream . Otherwise, only developers of the facebook application can publish.

== INSTALLATION ==

=== Fbconnect module Installation ===
  1.Download fbconnect from  http://drupal.org/project/fbconnect
  2.Before enabling module create an application in facebook (Read more about creating application: http://drupal.org/node/453420)  

  3. Upload the 'fbconnect' folder into your module directory, 
  4. Download the Facebook PHP libraries from http://svn.facebook.com/svnroot/platform/clients/packages/facebook-platform.tar.gz. 
	    Copy the facebook-platform/php folder content to the facebook-client repertory, 
  5. Activate the module through your website's modules administration pages, 
  6. Configure the module through the 'Fbconnect' section of the 'Site configuration' menu, using the information provided by Facebook (API key, Secret API key), 
  7. Activate the 'fbconnect friend list' block from the 'build/block' menu, 
  8. Edit the page.tpl.php file in your theme folder. Add the facebook xmlns for rendering FBML with Internet Explorer : <html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">. 

=== jquery ui ===

  1.Download the jquery ui module from http://drupal.org/project/jquery_ui
  2.Follow steps of http://drupal.org/node/388384#comment-1530114(Note:jqery update must be devel modul itself)
  
=== facebook_stream ===
  1.Copy facebook_stream module your modules directory
  2.Enable the module at admin/build/modules
  3.Enable facebook stream block at admin/build/blocks
  4.Configure the no of values to be displayed at user/%/fbstreamconnect
  5.Admin can configure the retrival interval at the facebook stream block configuration page
  6.Admin can configure whether to enable publish comments, likes, status message or not at admin/settings/facebook_stream


