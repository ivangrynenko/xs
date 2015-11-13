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

  return $form;
}

function xs_actions_action($node, $action) {

}
