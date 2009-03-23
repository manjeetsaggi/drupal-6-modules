<?php
// $Id: file_archive.theme.inc,v 1.10 2008/08/27 19:20:04 miglius Exp $

/**
 * @file
 * Themes for archive file formats.
 */
//////////////////////////////////////////////////////////////////////////////
// Theme callbacks

/**
 * Theme for archive rendering.
 */
function theme_file_archive_archive_render($options = array()) {
  extract($options, EXTR_SKIP);

  return '<code>'. preg_replace(array('/ /', "/\n/"), array('&nbsp;', '<br />'),  $text) .'</code>';
}

