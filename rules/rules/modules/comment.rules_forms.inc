<?php
// $Id: comment.rules_forms.inc,v 1.1.2.2 2009/04/19 15:03:43 fago Exp $


/**
 * @file
 * Rules configuration forms for the comment module.
 */

/**
 * Action "Load a comment by id" variable label callback.
 */
function rules_action_load_comment_variable_label($settings) {
  return t('Comment with id @id', array('@id' => $settings['cid']));
}

