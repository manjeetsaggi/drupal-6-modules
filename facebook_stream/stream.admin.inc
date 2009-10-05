<?php
// $Id: stream.admin.inc,v 1.1 2009/06/27 06:52:18 prajwala Exp $

/**
 * Display the Facebook stream Settings form 
 * Configure comments, posts and status messages to publish on the facebook wall
 */
function facebook_stream_settings_form() {
  $form['#prefix']='Set the options to post comments, likes and Status messages on the user wall of facebook';
  $form['stream_post_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to post comments'),
    '#default_value' => variable_get('stream_post_comments', 0),
					);
  $form['stream_post_likes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to post Likes'),
    '#default_value' => variable_get('stream_post_likes', 0),
				     );
  $form['stream_post_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to post Status messages'),
    '#default_value' => variable_get('stream_post_status', 0),
				      );
  return system_settings_form($form);	
  }

