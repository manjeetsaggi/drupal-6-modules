=====
About This Node
http://drupal.org/project/about_this_node
-----

About This Node creates a block that displays information about the node you're viewing. It allows users to see, at a glance, the following information without clicking "Edit" or digging into revision history:

    * node ID (NID)
    * node type (content type)
    * creation date and time
    * creation author (user who created the node)
    * last updated date and time
    * last updated author (user who made the most recent change to the node)
    * published status
    * promoted to front page status
    * sticky status
    * commenting status (enabled, read only, or disabled; also supports Node Comments)
    * translation status

Note: About This Node is not built with end-users in mind. Rather, this is an administrative tool to allow admins and other privileged users quick access to important information about a node. Requests to add end-user-facing features will be denied (with a friendly apology, of course).
Installation

This module, when enabled, creates a block that displays the information about a node. You will need to tell the module the content types for which it should display the block and display this block in one of your theme's regions (at Admin > Site Building > Blocks).

Finally, you will need to set your users' permissions. Go to Admin > User management > Permissions (or Admin > User management > Access control in Drupal 5) and look for the row titled "about_this_node module." Now check off which roles you want to administer the module and/or view the block.

=====
Customizing output
-----
To customize the output of About This Node's block, you may override the theme function theme_about_this_node_node() in your theme's template.php file.

Learn more about overriding theme functions here:
    * Drupal 6.x Theme Guide: http://drupal.org/theme-guide
    * Drupal 5.x Theme Developer's Guide: http://drupal.org/node/11811
