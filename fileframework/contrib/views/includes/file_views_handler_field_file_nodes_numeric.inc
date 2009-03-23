<?php
// $Id: file_views_handler_field_file_nodes_numeric.inc,v 1.1 2008/09/23 11:05:55 miglius Exp $

/**
 * @file
 * Integrates file operations with the Views module.
 */

/**
 * Field handler to present a numeric field.
 */
class file_views_handler_field_file_nodes_numeric extends views_handler_field {
  function render($values) {
    $num = $values->{$this->field_alias};
    return isset($num) ? $num : '';
  }
}

