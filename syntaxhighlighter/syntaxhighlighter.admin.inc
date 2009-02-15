<?php
// $Id: syntaxhighlighter.admin.inc,v 1.2 2009/02/11 22:52:25 mattyoung Exp $

/*
 * @file
 * Syntax highlighter module admin form
 *
 * Define the admin form
 */

/**
 * An admin form to specify which language to support for highlighting
 */
function syntaxhighlighter_settings_form() {
  $path = drupal_get_path('module', 'syntaxhighlighter') .'/syntaxhighlighter';
  
  $files = file_scan_directory($path .'/scripts', 'shBrush.*\.js');
  foreach ($files as $file) {
    $lang_options[$file->basename] = substr($file->name, 7);
  }
  ksort($lang_options);
  $form['syntaxhighlighter_enabled_languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Syntax highlighter enabled languages'),
    '#options' => $lang_options,
    '#default_value' => variable_get('syntaxhighlighter_enabled_languages', array('shBrushPhp.js')),
    '#description' => t('Only the selected languages will be enabled and its corresponding required Javascript files loaded.  You may only want to choose the languages you actual display on your site to lower the amount of Javascript files loaded. For details please see <a href="http://alexgorbatchev.com/wiki/SyntaxHighlighter:Brushes">the Syntax Highlighter Javascript Library Page</a>.'),
  );
  
  $files = file_scan_directory($path .'/styles', 'shTheme.*\.css');
  foreach ($files as $file) {
    $theme_options[$file->basename] = substr($file->name, 7);
  }
  ksort($theme_options);
  $form['syntaxhighlighter_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Theme'),
    '#description' => t('Choose a syntax highlight theme.'),
    '#options' => $theme_options,
    '#default_value' => variable_get('syntaxhighlighter_theme', 'shThemeDefault.css'),
  );
  
  $form['syntaxhighlighter_tagname'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag name'),
    '#description' => t('Use different tag to markup code.'),
    '#default_value' => variable_get('syntaxhighlighter_tagname', 'pre'),
    '#size' => 10,
  );
  $form['syntaxhighlighter_legacy_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Legacy mode'),
    '#description' => t('Enable pre-2.0 style markup support.'),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#default_value' => variable_get('syntaxhighlighter_legacy_mode', 0),
  );
  return system_settings_form($form);
}

