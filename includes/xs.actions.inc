<?php
/**
 * @file
 * xs.actions.inc
 */

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
function xs_actions_action_callback($node, $action) {
  global $xs_api;

  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get XenApi: !message', array('!message' => $e->getMessage()));
  }

  $vm = xs_get_vm_by_uuid($node);

  if (!in_array($action, $vm['allowed_operations'])) {
    drupal_set_message('This action is not allowed', 'error');
    drupal_goto('node/' . $node->nid);
  }

  $uuid = xs_get_vm_uuid_by_node($node);
  $vm_ref = xs_api_get_vm_by_uuid($uuid, 'vm_ref');

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
 * @param object $node
 *   Drupal node object of VM type.
 */
function xs_actions_action_vps_start($vm_ref) {
  xs_api_vm_start($vm_ref);
}

/**
 * Shutdown/halt a VPS action.
 *
 * @param object $node
 *   Drupal node object of VM type.
 */
function xs_actions_action_vps_shutdown($vm_ref) {
  xs_api_vm_clean_shutdown($vm_ref);
}

/**
 * Clean reboot a VPS action.
 *
 * @param object $node
 *   Drupal node object of VM type.
 */
function xs_actions_action_vps_clean_reboot($vm_ref) {
  xs_api_vm_clean_reboot($vm_ref);
}

/**
 * Hard reboot a VPS action.
 *
 * @param object $node
 *   Drupal node object of VM type.
 */
function xs_actions_action_vps_hard_reboot($vm_ref) {
  xs_api_vm_hard_reboot($vm_ref);
}


/**
 * Hard shutdown a VPS action.
 *
 * @param object $node
 *   Drupal node object of VM type.
 */
function xs_actions_action_vps_hard_shutdown($vm_ref) {
  xs_api_vm_hard_shutdown($vm_ref);
}
