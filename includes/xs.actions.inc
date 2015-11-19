<?php
/**
 * @file
 * xs.actions.inc
 */

function xs_actions_get_links($nid, $allowed_operations) {

  // Prepare actions link.
  $links = array();

  $links[XS_API_ACTION_START] = '<div class="dropdown-links"><span class="glyphicon glyphicon-play-circle" aria-hidden="true"></span> ' . l(t('Start'), 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_START, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
  $links[XS_API_ACTION_REBOOT] = '<div class="dropdown-links"><span class="glyphicon glyphicon-refresh" aria-hidden="true"></span> ' . l(t('Reboot'), 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_REBOOT, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
  $links[XS_API_ACTION_POWEROFF] = '<div class="dropdown-links"><span class="glyphicon glyphicon-off" aria-hidden="true"></span> ' . l(t('Shut Down'), 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_POWEROFF, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
  $links[XS_API_ACTION_REBOOT_FORCE] = '<div class="dropdown-links"><span class="glyphicon glyphicon-repeat" aria-hidden="true"></span> ' . l(t('Force Reboot'), 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_REBOOT_FORCE, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
  $links[XS_API_ACTION_POWEROFF_FORCE] = '<div class="dropdown-links"><span class="glyphicon glyphicon-exclamation-sign" aria-hidden="true"></span> ' . l(t('Force Shutdown'), 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_POWEROFF_FORCE, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';

  // Filter out VM operations that are not allowed. For example, remove 'start'
  // operation for a VM that is in Running state.
  foreach ($links as $key => $option) {
    if (!in_array($key, $allowed_operations)) {
      unset($links[$key]);
    }
  }

  return $links;
}

/**
 * Defines server actions available.
 *
 * @return array
 *   Array of server actions.
 */
function xs_get_available_actions() {
  return array(
    XS_API_ACTION_NOACTION => t('Select action'),
    XS_API_ACTION_START => t('Start VPS'),
    XS_API_ACTION_REBOOT => t('Reboot VPS'),
    XS_API_ACTION_POWEROFF => t('Shut Down VPS'),
    XS_API_ACTION_REBOOT_FORCE => t('Force Reboot VPS'),
    XS_API_ACTION_POWEROFF_FORCE => t('Force Shutdown VPS'),
  );
}

/**
 * Confirmation form for server actions.
 */
function xs_actions_server_action_confirmation_form($form, &$form_state, $node, $action) {
  $actions = xs_get_available_actions();

  $form['warning'] = array(
    '#markup' => t('You about to perform a disruptive operation to your server: !action!', array(
      '!action' => $actions[$action],
    )),
    '#prefix' => '<div class="red">',
    '#suffix' => '</div>',
  );

  $form['warning-2'] = array(
    '#markup' => t('To continue, press Confirm button, or press Cancel to return to the server overview page.'),
    '#prefix' => '<div class="red warning">',
    '#suffix' => '</div>',
  );

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $action,
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  $form['cancel'] = array(
    '#markup' => l('Cancel', 'node/' . $node->nid),
  );

  $form['#validate'][] = 'xs_actions_server_action_confirmation_form_validate';
  $form['#submit'][] = 'xs_actions_server_action_confirmation_form_submit';

  return $form;
}

/**
 * Form validate handler.
 */
function xs_actions_server_action_confirmation_form_validate($form, &$form_state) {
  if (empty($form_state['values']['action'])) {
    form_set_error('action', t('Validation error. Missing the action to perform.'));
  }

  if ($form_state['values']['action'] != arg(3)) {
    form_set_error('action', t('Validation error. This action is not allowed.'));
  }

  if ($form_state['values']['nid'] != arg(1)) {
    form_set_error('action', t('Validation error. This action is not allowed on the selected server.'));
  }
}

/**
 * Form submit handler.
 */
function xs_actions_server_action_confirmation_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);
  xs_actions_action($node, $form_state['values']['action']);
}

/**
 * Callback for VM actions.
 *
 * @param object $node
 *   VM node.
 * @param string $action
 *   Action to be performed on the $node VM. Supported actions are:
 *   - reboot: graceful reboot of the VM,
 *   - poweroff: graceful poweroff of the VM,
 *   - start: start the VM,
 *   - reboot-force: forceful reboot of the VM,
 *   - poweroff-force: forceful poweroff of the VM.
 *
 * @return string
 */
function xs_actions_action($node, $action) {
  $vm = xs_get_vm_by_node($node);

  if (!in_array($action, $vm['allowed_operations'])) {
    drupal_set_message('This action is not allowed', 'error');
    drupal_goto('node/' . $node->nid);
  }

  $uuid = xs_get_vm_uuid_by_node($node);
  $vm_ref = xs_api_get_vm_ref_by_uuid($uuid);

  switch ($action) {
    case XS_API_ACTION_START:
      xs_actions_action_vps_start($vm_ref);
      drupal_set_message('The server has been started. Allow 1-2 minutes before attempting to connect.');
      drupal_set_message('You may need to refresh this page in order to reflect server\'s power state or IP addresses.');
      break;

    case XS_API_ACTION_POWEROFF:
      xs_actions_action_vps_shutdown($vm_ref);
      drupal_set_message('The server is being shut down. Allow 1-2 minutes before it has shut down.');
      drupal_set_message('You may need to refresh this page in order to reflect server\'s power state');
      break;

    case XS_API_ACTION_REBOOT:
      xs_actions_action_vps_clean_reboot($vm_ref);
      drupal_set_message('The server is being rebooted. Allow 1-2 minutes for the operation to complete.');
      break;

    case XS_API_ACTION_REBOOT_FORCE:
      xs_actions_action_vps_hard_reboot($vm_ref);
      drupal_set_message('The server has been forcefully rebooted. Allow 1-2 minutes for the operation to complete.');
      break;

    case XS_API_ACTION_POWEROFF_FORCE:
      xs_actions_action_vps_hard_shutdown($vm_ref);
      drupal_set_message('The server has been forcefully shut down. Allow 1-2 minutes for the operation to complete.');
      break;

    default:
      drupal_set_message('This action is not recognised', 'error');
      break;
  }

  drupal_goto('node/' . $node->nid);
}

/**
 * Start a VPS action.
 *
 * @param string $vm_ref
 *   VM referense string.
 */
function xs_actions_action_vps_start($vm_ref) {
  xs_api_vm_start($vm_ref);
}

/**
 * Shutdown/halt a VPS action.
 *
 * @param string $vm_ref
 *   VM referense string.
 */
function xs_actions_action_vps_shutdown($vm_ref) {
  xs_api_vm_clean_shutdown($vm_ref);
}

/**
 * Clean reboot a VPS action.
 *
 * @param string $vm_ref
 *   VM referense string.
 */
function xs_actions_action_vps_clean_reboot($vm_ref) {
  xs_api_vm_clean_reboot($vm_ref);
}

/**
 * Hard reboot a VPS action.
 *
 * @param string $vm_ref
 *   VM referense string.
 */
function xs_actions_action_vps_hard_reboot($vm_ref) {
  xs_api_vm_hard_reboot($vm_ref);
}

/**
 * Hard shutdown a VPS action.
 *
 * @param string $vm_ref
 *   VM referense string.
 */
function xs_actions_action_vps_hard_shutdown($vm_ref) {
  xs_api_vm_hard_shutdown($vm_ref);
}
