// $Id: README.txt,v 1.1 2009/07/06 19:39:31 amitaibu Exp $

Overview
---------
CCK provides CRUD (Create, Update, Delete) functionality to add fields via code.
CCK also provides a utility module called content copy.

This module uses both of them so you, the developer, don't have to be bothered
keeping your content type in sync - you simply export your CCK content type to
code or file and cck_sync_crud($content) will do the rest.

The module also CRUDs group fields.


Usage example
-------------
1) In your development environment create a content type called
   "Album" and add some CCK fields and groups to it.
2) Export the content type to code via admin/content/types/export.
3) Place the exported code in a your custom module. If your module name is
   'default_config', you can place it under:
   default_config/export_content/6000_album.php
4) file will look similar to this (including the <?php opening tag):

<?php

/***
 * @file
 * Album  content type export.
 */

$content['type']  = array (
  'name' => 'Album',
  'type' => 'albumr',
  ...
  // The rest of the content export code will be here.
);

return $content;


5) Note that we added as a prefix to the name of the file, the number that will
   be used in hook_update_N() - in this case 6000.
6) In your default_config.install file add this code:

/**
 * Update album content type.
 */
function default_config_update_6000() {
  $ret = array();
  // Sync the album content type.
  cck_sync_from_file(drupal_get_path('module', 'default_config') .'/content_export/6000_album.php');
  return $ret;
}

7) On your production environment, when update.php will be executed the Album 
   content type will be synchronized according to the exported code.