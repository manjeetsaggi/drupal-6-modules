<?php
// $Id: og_profiles.views.inc,v 1.1 2008/07/29 08:39:09 henrysearle Exp $
/**
 * @file
 * Provide views data and handlers for user.module
 */

/**
 * @defgroup views_og_profiles_module og_profiles.module handlers
 *
 * @{
 */

/**
 * Implementation of hook_views_data()
 */
function og_profiles_views_data() {
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['og_profiles_values']['table']['group']  = t('Organic Group Profiles');

  $data['og_profiles_values']['table']['join'] = array(
    'node' => array(
      'left_table' => 'og_profiles_values',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'users' => array(
      'left_table' => 'og_profiles_values',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $fields = og_profiles_views_get_fields();
  foreach ($fields as $field) {
    $table_name = 'og_profiles_values_'. $field->name;
    $data[$table_name] = array(
      'table' => array(
        'group' => t('Organic Groups Profiles'),
        'join' => array(
          'node' => array(
            'table' => 'og_profiles_values',
            'left_table' => 'users',
            'left_field' => 'uid',
            'field' => 'uid',
            'extra' => array(array('field' => 'fid', 'value' => $field->fid)),
          ),
          'users' => array(
            'table' => 'og_profiles_values',
            'left_field' => 'uid',
            'field' => 'uid',
            'extra' => array(array('field' => 'fid', 'value' => $field->fid)),
          ),
        ),
      ),
    );
    // All fields in the table are named 'value'.
    $data[$table_name]['value'] = og_profiles_views_fetch_field($field);
  }

  return $data;
}

/**
 * Get all profile fields
 */
function og_profiles_views_get_fields() {
  static $fields = NULL;

  if (!isset($fields)) {
    $fields = array();
    $results = db_query("SELECT * FROM {og_profiles_fields} ORDER BY category, weight");

    while ($row = db_fetch_object($results)) {
      if (!empty($row->options)) {
        if (!in_array(substr($row->options, 0, 2), array('a:', 'b:', 'i:', 'f:', 'o:', 's:', ))) {
          // unserialized fields default version
          $options = $row->options;
          unset($row->options);
          $row->options = $options;
        }
        else {
          // serialized fields or modified version
          $row->options = unserialize($row->options);
        }
      }
      $fields[$row->fid] = $row;
    }
  }
  return $fields;
}


/**
 * Add profile fields to view table
 */
function og_profiles_views_fetch_field($field) {
  $data = array(
    'title' => t('@field-name', array('@field-name' => $field->title)),
  );

  // Add fields specific to the profile type.
  switch ($field->type) {
    case 'textfield':
      $data += array(
        'help' => t('Organic Groups Profiles textfield'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
      );

      break;
    case 'textarea':
      $data += array(
        'help' => t('Organic Groups Profiles textarea'),
        'field' => array(
          'handler' => 'views_handler_field_markup',
          'format' => FILTER_FORMAT_DEFAULT,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      );

      break;
    case 'checkbox':
      $data += array(
        'help' => t('Organic Groups Profiles checkbox'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
        ),
        // @todo there ought to be a boolean argument handler
      );

      break;
    case 'url':
      $data += array(
        'help' => t('Organic Groups Profiles URL'),
        'field' => array(
          'handler' => 'views_handler_field_url',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      );

      break;
    case 'selection':
      $data += array(
        'help' => t('Organic Groups Profiles selection'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_og_profiles_selection',
          'fid' => $field->fid,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
      );

      break;
    case 'list':
      $data += array(
        'help' => t('Organic Groups Profiles freeform list %field-name.', array('%field-name' => $field->title)),
        'field' => array(
          'handler' => 'views_handler_field_og_profiles_list',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      );

      break;
    case 'date':
      $data += array(
        'help' => t('Organic Groups Profiles date %field-name.', array('%field-name' => $field->title)),
        'field' => array(
          'handler' => 'views_handler_field_og_profiles_date',
        ),
      );

      break;
  }

  // @todo: add access control to hidden fields.
  return $data;
}

/**
 * Field handler display a Organic Groups Profile date
 *
 * The dates are stored serialized, which makes them mostly useless from
 * SQL. About all we can do is unserialize and display them.
 */
class views_handler_field_og_profiles_date extends views_handler_field_date {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // we can't do "time ago" so remove it from the form.
    unset($form['date_format']['#options']['time ago']);
  }

  /**
   * Display a Organic Groups Profile field of type 'date'
   */
  function render($value) {
    if (!$value->{$this->field_alias}) {
      return;
    }
    $value = unserialize($value->{$this->field_alias});
    $format = $this->options['date_format'];
    switch ($format) {
      case 'custom':
        $format = $this->options['custom_date_format'];
        break;
      case 'small':
        $format = variable_get('date_format_short', 'm/d/Y - H:i');
        break;
      case 'medium':
        $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
        break;
      case 'large':
        $format = variable_get('date_format_long', 'l, F j, Y - H:i');
        break;
    }

    // Note: Avoid PHP's date() because it does not handle dates before
    // 1970 on Windows. This would make the date field useless for e.g.
    // birthdays.

    // But we *can* deal with non-year stuff:
    $date = gmmktime(0, 0, 0, $value['month'], $value['day'], 1970);
    $replace = array(
      // day
      'd' => sprintf('%02d', $value['day']),
      'D' => NULL,
      'l' => NULL,
      'N' => NULL,
      'S' => date('S', $date),
      'w' => NULL,
      'j' => $value['day'],
      // month
      'F' => date('F', $date),
      'm' => sprintf('%02d', $value['month']),
      'M' => date('M', $date),
      'n' => date('n', $date),

      'Y' => $value['year'],
      'y' => substr($value['year'], 2, 2),

      // kill time stuff
      'a' => NULL,
      'A' => NULL,
      'g' => NULL,
      'G' => NULL,
      'h' => NULL,
      'H' => NULL,
      'i' => NULL,
      's' => NULL,
      ':' => NULL,
      'T' => NULL,
      ' - ' => NULL,
      ':' => NULL,
    );

    return strtr($format, $replace);
  }
}

/**
 * Field handler display a Organic Groups Profile date
 *
 * The dates are stored serialized, which makes them mostly useless from
 * SQL. About all we can do is unserialize and display them.
 */
class views_handler_field_og_profiles_list extends views_handler_field_prerender_list {
  /**
   * Break up our field into a proper list.
   */
  function render($value) {
    $field = $value->{$this->field_alias};
    $this->items[$field] = array();
    foreach (split("[,\n\r]", $field) as $item) {
      if ($item != '' && $item !== NULL) {
        $this->items[$field][] = $item;
      }
    }
    return parent::render($value);
  }
}

/**
 * Filter by a selection widget in the profile.
 */
class views_handler_filter_og_profiles_selection extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    $all_options = og_profiles_views_get_fields();
    $field = $all_options[$this->definition['fid']];

    $lines = split("[,\n\r]", $field->options);
    foreach ($lines as $line) {
      if ($line = trim($line)) {
        $this->value_options[$line] = $line;
      }
    }
  }
}

/**
 * @}
 */
