// $Id: README.txt,v 1.1 2009/01/22 15:08:57 pvhee Exp $

-- SUMMARY --

Content Complete is a CCK extension module. The module allows privileged users to tag CCK fields needed for 
completion. The module checks these tagged fields against the content provided for those content types,
based on what content types a user has privileges for to edit. The complete percentage is shown to the user
in the form of a block, together with quick links to complete the missing fields. When the percentage is 
equal to 100%, the block won't be shown.

This module is related to and based on the "Profile Complete Percent" module (http://drupal.org/project/pcp), 
which implements similar functionality for user profiles. This module is an excellent
alternative when CCK is used to maintain user profiles, and at the same time can be reused in other situations. For
example, you might have a 'settings' content type on your website with configuration settings. You could show
the administrator how much fields there are left to complete the settings using a block.

For a full description of the module, visit the project page:
  http://drupal.org/project/content_complete

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/content_complete


-- REQUIREMENTS --

CCK.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  content_complete module:

  - administer content complete

    Users in roles with the "administer content complete" permission will be able
    to administer the module.

  - access content complete

    Users in roles with the "access content complete" permission will be able
    to see the content complete statistics.

  Note that the content complete block depends on the actual permissions of the 
  viewing user. For example, if the user does not have the permission to edit 
  nodes of type "page" the content complete percentage for this type will not 
  be shown to the user.

* Customize the settings in Administer >> Site Configuration >> Content Complete.
  A block will be created for every content type added to check for completeness.


-- CUSTOMIZATION --

The block output can be entirely overridden in your template files. 
Check content_complete.module for more information.


-- FAQ --

Q: Which nodes are used to check for completion of a certain content type?
A: At present only the first node of the content type is checked for completion. In
   a future release this behavior might change.


-- CONTACT --

Current maintainers:
* Peter Vanhee (pvhee) - http://drupal.org/user/108811

This project has been sponsored by:
* Youth Agora
  Innovating online youth information. 
  Visit http://www.youthagora.org for more information.