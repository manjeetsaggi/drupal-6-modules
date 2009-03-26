/* $Id: README.txt,v 1.1 2009/03/16 20:05:06 mfu Exp $ */

-- SUMMARY --

The HTPasswd Sync module let you synchronize a htpasswd and a
htgroup file with the user database.

For a full description of the module, visit the project page:
  http://drupal.org/project/htpasswdsync

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/htpasswdsync


-- REQUIREMENTS --

The syncrhonization only happen on password change. Hence, this module shall be 
installed before any user creation.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure synchronized files in Administer >> User management >> HTPasswd Sync >>
  
  htpasswdsync module:

  - htpasswd file
    
    The file that will contain users and password, password are crypted, using
    the standard crypt function, with a random two charaters seed.

  - htgroup file
  
    The file that will synchronize the roles.

  - roles
  
    The roles you want to export in the htgroup file.


-- CUSTOMIZATION --

None.

-- TROUBLESHOOTING --



-- FAQ --


-- CONTACT --

Current maintainers:
* Marc Furrer (m.fu) - http://drupal.org/user/310415
