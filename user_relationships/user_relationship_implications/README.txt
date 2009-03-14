; $Id: README.txt,v 1.1.2.4 2008/04/21 00:38:18 sprsquish Exp $

User Relationship Implications Module
-------------------------------------
This is a plugin module for the User Relationships module.

It allows admins to set up implied relationships (ex: Manager implies Coworker).
These implies relationships will be automatically created. If a relationship that is
implied by another is deleted the implied by relationship is also deleted.

Implied relationships can be chained (ex: Manager implies Coworker implies Officemate)

Send comments to Jeff Smick: http://drupal.org/user/107579/contact, or post an issue at
http://drupal.org/project/user_relationships.


Scenarios
---------
User creates a "manager" relationship to another user: The "coworker" relationship will
be automatically created between them.

User removes a "coworker" relationship to another user: The "manager" relationship will
be automatically deleted.


Requirements
------------
Drupal 6
User Relationships Module


Installation
------------
* Enable User Relationship Implications in the "Site building -> modules" administration screen.
* If you want to use the implications relationships page, override the
theme_user_relationships_page in your theme's template.php with the
implications page. E.g.
  function phptemplate_user_relationships_page($uid = NULL, $relationship = NULL) {
    return return theme('user_relationship_implications_page', $uid, $relationship);
  }

Database Schema
---------------
MySQL
=====

-- 
-- Table structure for table `user_relationship_implications`
-- 
CREATE TABLE IF NOT EXISTS `user_relationship_implications` (
  `riid` int(10) unsigned NOT NULL default '0',
  `rtid` int(10) unsigned NOT NULL default '0',
  `implies_rtid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`riid`),
  KEY `rtid` (`rtid`),
  KEY `implies_rtid` (`implies_rtid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


Credits
-------
Written by Jeff Smick.
Written originally for and financially supported by OurChart Inc. (http://www.ourchart.com)
Thanks to the BuddyList module team for their inspiration