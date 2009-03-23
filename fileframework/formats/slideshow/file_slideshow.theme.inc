<?php
// $Id: file_slideshow.theme.inc,v 1.12 2008/10/05 19:11:14 miglius Exp $

/**
 * @file
 * Themes for slideshow file formats.
 */

//////////////////////////////////////////////////////////////////////////////
// Theme callbacks

/**
 * Theme for admin PDF checkboxes.
 */
function theme_file_slideshow_admin_settings($form) {
  $rows = array();
  foreach ($form['pdf'] as $name => $element) {
    if (preg_match('/pdf_/', $name)) {
      $rows[] = array(
        drupal_render($form['pdf'][$name][0]),
        drupal_render($form['pdf'][$name][1]),
        drupal_render($form['pdf'][$name][2])
      );
      unset($form['pdf'][$name]);
    }
  }
  $form['pdf']['pdf'] = array('#type' => 'markup', '#value' => theme('table', NULL, $rows));
  return drupal_render($form);
}

