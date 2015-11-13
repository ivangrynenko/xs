<?php
/**
 * @file
 * xs.actions.inc
 */

/**
 * XS Actions form builder.
 */
function xs_actions_action_form($form, &$form_state, $nid) {

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['reboot'] = array(
    '#type' => 'button',
    '#value' => t('Graceful Reboot'),
  );

  $form['actions']['reboot'] = array(
    '#type' => 'button',
    '#value' => t('Graceful Reboot'),
  );

  return $form;
}

function xs_actions_access_callback($node) {
  global $user;
  $access = FALSE;
  if ($user->uid == 1) {
    return TRUE;
  }

  if (user_access('perform any vm actions')) {
    return TRUE;
  }

  if (user_access('perform own vm actions')) {

  }
}

function xs_actions_action_callback($node, $action) {
  return '';
}
