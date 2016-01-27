<?php
/**
 * @file
 * xs.actions.inc
 */

/**
 * Creates definition for server action links.
 *
 * @param int $nid
 *   VM node nid.
 * @param array $allowed_operations
 *   VM allowed operations
 * @param string $destination
 *   Destination definition, same as url().
 *
 * @return array
 */
function xs_actions_get_links($nid, $allowed_operations, $destination = '') {
  $action_links = xs_get_available_actions();
  // Prepare actions link.
  $links = array(
    XS_API_ACTION_START => '<div class="dropdown-links"><i class="fa fa-power-off"></i> ' . l($action_links[XS_API_ACTION_START], 'node/' . $nid . '/xs-actions/start', array('destination' => $destination)) . '</div>',
    XS_API_ACTION_REBOOT => '<div class="dropdown-links"><i class="fa fa-refresh"></i> ' . l($action_links[XS_API_ACTION_REBOOT], 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_REBOOT, array('query' => array('destination' => $destination), 'attributes' => array('class' => array('xs-glyphicon-link')))) . '</div>',
    XS_API_ACTION_POWEROFF => '<div class="dropdown-links"><i class="fa fa-power-off"></i> ' . l($action_links[XS_API_ACTION_POWEROFF], 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_POWEROFF, array('query' => array('destination' => $destination), 'attributes' => array('class' => array('xs-glyphicon-link')))) . '</div>',
    XS_API_ACTION_REBOOT_FORCE => '<div class="dropdown-links"><i class="fa fa-times-circle-o"></i> ' . l($action_links[XS_API_ACTION_REBOOT_FORCE], 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_REBOOT_FORCE, array('query' => array('destination' => $destination), 'attributes' => array('class' => array('xs-glyphicon-link')))) . '</div>',
    XS_API_ACTION_POWEROFF_FORCE => '<div class="dropdown-links"><i class="fa fa-circle-o-notch"></i> ' . l($action_links[XS_API_ACTION_POWEROFF_FORCE], 'node/' . $nid . '/xs-actions/' . XS_API_ACTION_POWEROFF_FORCE, array('query' => array('destination' => $destination), 'attributes' => array('class' => array('xs-glyphicon-link')))) . '</div>',
  );

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
    XS_API_ACTION_START => t('Start'),
    XS_API_ACTION_REBOOT => t('Reboot'),
    XS_API_ACTION_POWEROFF => t('Shut Down'),
    XS_API_ACTION_REBOOT_FORCE => t('Force Reboot'),
    XS_API_ACTION_POWEROFF_FORCE => t('Force Shutdown'),
  );
}

/**
 * Confirmation form for server actions.
 */
function xs_actions_server_action_confirmation_form($form, &$form_state, $node, $action) {
  $actions = xs_get_available_actions();

  drupal_set_message(t('Are you sure you want to execute the "!action" operation to your server "!server_name"?', array(
    '!action' => $actions[$action],
    '!server_name' => $node->title,
  )), 'error');

  $form['warning-2'] = array(
    '#markup' => t('To continue, press Confirm button, or press Cancel to return to the server overview page.'),
    '#prefix' => '<div class="default">',
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
  if (isset($_GET['destination'])) {
    $form_state['action'] = $_GET['destination'];
  }
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
  global $user;

  $vm_uuid = xs_get_vm_uuid_by_node($node);
  try {
    $xs_vm = new XsVm($vm_uuid);
  }
  catch (Exception $e) {
    xs_log($e);
  }

  if (!empty($xs_vm)) {
    $vm = $xs_vm->getData();
  }

  if (!in_array($action, $vm['allowed_operations'])) {
    drupal_set_message('This action is not allowed', 'error');
    drupal_goto('node/' . $node->nid);
  }

  switch ($action) {
    case XS_API_ACTION_START:
      $xs_vm->start();
      drupal_set_message('The server has been started. Allow 1-2 minutes before attempting to connect.');
      drupal_set_message('You may need to refresh this page in order to reflect server\'s power state or IP addresses.');
      $log_message = t('Server started manually by !user', array('!user' => $user->name));
      $log_type = XS_API_ACTION_START;
      break;

    case XS_API_ACTION_POWEROFF:
      $xs_vm->stop();
      drupal_set_message('The server is being shut down. Allow 1-2 minutes before it has shut down.');
      drupal_set_message('You may need to refresh this page in order to reflect server\'s power state');
      $log_message = t('Server stopped manually by !user', array('!user' => $user->name));
      $log_type = XS_API_ACTION_POWEROFF;
      break;

    case XS_API_ACTION_REBOOT:
      $xs_vm->reboot();
      drupal_set_message('The server is being rebooted. Allow 1-2 minutes for the operation to complete.');
      $log_message = t('Server rebooted manually by !user', array('!user' => $user->name));
      $log_type = XS_API_ACTION_REBOOT;
      break;

    case XS_API_ACTION_REBOOT_FORCE:
      $xs_vm->rebootForce();
      drupal_set_message('The server has been forcefully rebooted. Allow 1-2 minutes for the operation to complete.');
      $log_message = t('Server force-rebooted manually by !user', array('!user' => $user->name));
      $log_type = XS_API_ACTION_REBOOT_FORCE;
      break;

    case XS_API_ACTION_POWEROFF_FORCE:
      $status = $xs_vm->stopForce();
      if ($status) {
        drupal_set_message('The server has been forcefully shut down. Allow 1-2 minutes for the operation to complete.');
        $log_message = t('Server force-shutdown manually by !user', array('!user' => $user->name));
        $log_type = XS_API_ACTION_POWEROFF_FORCE;
      }
      else {
        drupal_set_message('Unable to poweroff the VM, as it is not running');
        $log_message = t('Server force-shutdown manually by !user fail. Server is not running.', array('!user' => $user->name));
        $log_type = XS_API_ACTION_POWEROFF_FORCE;
      }
      break;

    default:
      drupal_set_message('This action is not recognised', 'error');
      $log_message = t('Unrecognised server action by !user', array('!user' => $user->name));
      $log_type = 'error';
      break;
  }

  xs_action_log_write($log_message, $log_type);

  drupal_goto('node/' . $node->nid);
}
