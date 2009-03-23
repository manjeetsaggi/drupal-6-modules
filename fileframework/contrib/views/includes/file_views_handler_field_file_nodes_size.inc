<?php
// $Id: file_views_handler_field_file_nodes_size.inc,v 1.1 2008/09/23 11:05:55 miglius Exp $

/**
 * @file
 * Integrates file operations with the Views module.
 */

/**
 * Field handler to present a file size.
 */
class file_views_handler_field_file_nodes_size extends views_handler_field {
  function render($values) {
    $size = $values->{$this->field_alias};
    return isset($size) ? format_size($size) : '';
  }
}

