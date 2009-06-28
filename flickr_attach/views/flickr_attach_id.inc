<?php
// $Id: flickr_attach_id.inc,v 1.1 2009/06/09 21:15:36 jbomb Exp $
/**
 * Field handler to present a link to the node.
 */
class flickr_attach_id extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['data'] = 'data';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Provide a settings menu so admins can select how a flickr attach 
    // image is displayed in a view.
    if ($this->table == 'node_flickr_images') {
      $form['display'] = array(
        '#type' => 'radios',
        '#title' => t('Format'),
        '#options' => array(
          0 => t('Square'),
          1 => t('Thumbnail'),
          2 => t('Small'),
          3 => t('Medium'),
          4 => t('Original'),
          'id' => t('Flickr ID'),
          ),
        '#default_value' => $this->options['display'],
        '#required' => TRUE,
        );
    }
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $data = $values->{$this->aliases['data']};
    $data = unserialize($data);

    // We only provide the flickr id for albums
    if ($this->table == 'node_flickr_albums' OR $this->options['display'] == 'id') {
      return $data['id'];
    }
    // review settings for flickr images
    $images = flickr_photo_get_sizes($data['id']);
    $image = $images[$this->options['display']]['source'];
    if (!empty($image)) {
      return theme('image', $image, NULL, NULL, NULL, FALSE);
    }
  }
}
