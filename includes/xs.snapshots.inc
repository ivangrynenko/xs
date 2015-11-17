<?php
/**
 * @file
 * xs.snapshots.inc
 *
 * Performs snapshot-related actions.
 */

/**
 * Snapshots callback function.
 *
 * @param object $node
 *   Drupal node object.
 *
 * @return string
 *   Rendered output.
 */
function xs_actions_snapshots_callback($node) {
  global $xs_api;

  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get XenApi: !message', array('!message' => $e->getMessage()));
  }

  return theme('xs_actions_snapshots_output', array('node' => $node));
}

/**
 * Loads full snapshot objects for a single $vm.
 *
 * @param array $vm
 *   A VM array.
 *
 * @return array
 *   Array of fully loaded shapshot arrays, keyed by snapshot reference.
 */
function xs_get_vm_snapshots($vm) {
  $full_snapshots = array();
  if (!empty($vm['snapshots'])) {
    foreach ($vm['snapshots'] as $snapshot_ref) {
      $full_snapshots[$snapshot_ref] = xs_get_snapshot_by_ref($snapshot_ref);
    }
  }

  return $full_snapshots;
}

/**
 * Gets available snapshots for the server, loaded from drupal node.
 *
 * @param object $node
 *   Drupal node.
 *
 * @return array
 *   Array of available snapshots.
 */
function xs_get_vm_snapshots_from_node($node) {
  $vm = xs_get_vm_by_uuid($node);

  return xs_get_vm_snapshots($vm);
}

/**
 * Loads the number of available snapshots of the VM, loaded via Drupal node.
 *
 * @param array $node
 *   Drupal node
 *
 * @return bool|int
 *   Number of available snapshots or FALSE.
 */
function xs_get_vm_snapshots_count_from_node($node) {
  $vm = xs_get_vm_by_uuid($node);

  return !empty($vm['snapshots']) ? count($vm['snapshots']) : FALSE;
}

/**
 * Loads snapshot information from the VM.
 *
 * @param string $snap_ref
 *   Snapshot reference.
 *
 * @return array
 *   Array of fully loaded snapshots.
 */
function xs_get_snapshot_by_ref($snap_ref) {
  return xs_api_get_snapshot($snap_ref);
}

/**
 * Gets the snapshots limit for the VPS.
 *
 * @param object $node
 *   Drupal node object.
 *
 * @return int|bool
 *   Number of snapshots or FALSE.
 */
function xs_get_snapshots_limit($node) {
  if (!empty($node->field_xs_vps_snapshots_limit)) {
    return (int) $node->field_xs_vps_snapshots_limit[LANGUAGE_NONE][0]['value'];
  }

  return FALSE;
}

/**
 * Title callback for the add new snapshot page.
 *
 * @param object $node
 *   Drupal node.
 *
 * @return null|string
 *   Title.
 */
function xs_snapshots_create_snapshot_title_callback($node) {
  return t('Create a snapshot of "!node_title" server', array('!node_title' => $node->title));
}

/**
 * Snapshot create form.
 */
function xs_snapshots_create_snapshot_form($form, &$form_state, $node) {
  $form['link'] = array(
    '#markup' => l('< Back to snapshots', 'node/' . $node->nid . '/xs-snapshots'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Snapshot name'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Short description'),
    '#required' => FALSE,
    '#cols' => 60,
    '#rows' => 2,
    '#description' => t('Give optional description to this snapshot'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  return $form;
}

/**
 * Validate handler for xs_snapshots_create_snapshot_form.
 */
function xs_snapshots_create_snapshot_form_validate($form, &$form_state) {
  $nid = is_numeric(arg(1)) ? arg(1) : FALSE;
  if (!$nid) {
    return form_set_error(NULL, 'Validation error. This form must be submitted in server context');
  }

  $node = node_load($nid);
  $form_state['storage']['xs_snapshot_create']['node'] = $node;
  // Check if the user can add more snapshots.
  $limit  = xs_get_snapshots_limit($node);
  $current_snapshots = xs_get_vm_snapshots_count_from_node($node);
  if ($current_snapshots >= $limit) {
    return form_set_error(NULL, 'Validation error. You have reached the maximum number of snapshots this server could have.');
  }
}

/**
 * Submit handler for xs_snapshots_create_snapshot_form.
 */
function xs_snapshots_create_snapshot_form_submit($form, &$form_state) {
  $vm_uuid = xs_get_vm_uuid_by_node($form_state['storage']['xs_snapshot_create']['node']);
  $vm_ref = xs_api_get_vm_by_uuid($vm_uuid, 'vm_ref');

  xs_snapshot_create_snapshot($vm_ref, $form_state['values']['title'], $form_state['values']['description']);
}

/**
 * Creates a snapshot of the VM identified by $vm_ref.
 *
 * @param string $vm_ref
 *   VM reference.
 * @param string $title
 *   Title of the snapshot.
 * @param string $description
 *   Description of the snapshot.
 */
function xs_snapshot_create_snapshot($vm_ref, $title, $description) {
  return xs_api_create_snapshot($vm_ref, $title);
}

/**
 * Form builder for snapshot actions.
 */
function xs_snapshots_action_snapshot_form($form, &$form_state, $snapshots, $nid, $current_active_snapshot) {
  $form['snapshot_uuid'] = array(
    '#type' => 'select',
    '#options' => _xs_actions_snapshot_prepare_options($snapshots, $current_active_snapshot),
    '#description' => t('Select a snapshot to perform an action, then select the action below'),
  );

  $form['snapshot_actions'] = array(
    '#type' => 'select',
    '#description' => t('To destroy a snapshot, select Destroy. To revert your Server to the selected snapshot, select Revert'),
    '#options' => array(
      '' => t('Select Action'),
      'destroy' => t('Destroy'),
      'revert' => t('Revert'),
    ),
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
  );

  $form['#validate'][] = 'xs_snapshots_action_snapshot_form_validate';
  $form['#submit'][] = 'xs_snapshots_action_snapshot_form_submit';

  return $form;
}

/**
 * Validation handler for xs_snapshots_action_snapshot_form.
 */
function xs_snapshots_action_snapshot_form_validate($form, &$form_state) {
  if ($form_state['values']['nid'] != arg(1)) {
    return form_set_error('snapshot_actions', 'Validation error');
  }

  if (empty($form_state['values']['snapshot_actions'])) {
    return form_set_error('snapshot_actions', 'Please select an action');
  }

  if (empty($form_state['values']['snapshot_uuid'])) {
    return form_set_error('snapshot_uuid', 'Please select a snapshot');
  }
}

/**
 * Submit handler for xs_snapshots_action_snapshot_form.
 */
function xs_snapshots_action_snapshot_form_submit($form, &$form_state) {
  // Redirect to the confirmation page.
  drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots/' . $form_state['values']['snapshot_actions'] . '/' . $form_state['values']['snapshot_uuid']);
}

/**
 * Confirmation form for snapshot action.
 */
function xs_actions_confirm_snapshots_form($form, &$form_state, $node, $action, $snapshot_ref) {
  global $xs_api;

  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get XenApi: !message', array('!message' => $e->getMessage()));
  }

  // Add hidden fields to pass over to the action submit handler.
  $form['snapshot_ref'] = array(
    '#type' => 'hidden',
    '#value' => $snapshot_ref,
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $action,
  );

  $snapshot_created_time = xs_get_snapshot_created_time($snapshot_ref);

  $date = date('d M Y h:i:sA', $snapshot_created_time);
  $form['warning'] = array(
    '#markup' => t('You about to !action snapshot created at !date. This action cannot be undone!', array(
      '!action' => strtoupper($action),
      '!date' => !empty($date) ? $date : 'N/A'
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ucwords($action),
  );

  $form['cancel'] = array(
    '#markup' => l('Cancel', 'node/' . $node->nid . '/xs-snapshots'),
  );

  $form['#validate'][] = 'xs_actions_confirm_snapshots_form_validate';
  $form['#submit'][] = 'xs_actions_confirm_snapshots_form_submit';

  return $form;
}

/**
 * Validator for xs_actions_confirm_snapshots_form.
 */
function xs_actions_confirm_snapshots_form_validate($form, &$form_state) {
  if (empty($form_state['values']['snapshot_ref']) || empty($form_state['values']['action'])) {
    drupal_set_message('Unable to complete the action. Validation error', 'error');
    drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots');
  }
}

/**
 * Submit handler for xs_actions_confirm_snapshots_form.
 */
function xs_actions_confirm_snapshots_form_submit($form, &$form_state) {
  switch ($form_state['values']['action']) {
    case 'destroy':
      xs_actions_snapshot_destroy($form_state['values']['snapshot_ref']);
      drupal_set_message('The selected snapshot was deleted successfully');
      drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots');
      break;

    case 'revert':
      if (is_numeric(arg(1)) && arg(0) == 'node') {
        $node = node_load(arg(1));
        if (xs_actions_access_callback($node)) {
          module_load_include('inc', 'xs', 'includes/xs.actions');

          // Load the VM.
          $uuid = xs_get_vm_uuid_by_node($node);
          $vm_ref = xs_api_get_vm_by_uuid($uuid, 'vm_ref');

          // Shut down the VM.
          xs_actions_action_vps_hard_shutdown($vm_ref);

          // Revert to the selected snapshot.
          xs_actions_snapshot_revert($form_state['values']['snapshot_ref']);

          // Start the VM.
          xs_actions_action_vps_start($vm_ref);

          drupal_set_message('Your Server has been reverted to the selected snapshot and started successfully');
        }
      }
      drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots');
      break;
  }
}

/**
 * Gets snapshot created time by reference.
 *
 * @param string $snapshot_ref
 *   Snapshot reference.
 *
 * @return int|bool
 *   Snapshot created timestamp or FALSE.
 */
function xs_get_snapshot_created_time($snapshot_ref) {
  $snapshot = xs_get_snapshot_by_ref($snapshot_ref);
  if (!empty($snapshot['snapshot_time'])) {
    return $snapshot['snapshot_time']->timestamp;
  }

  return FALSE;
}

/**
 * Destroys the snapshot.
 *
 * @param string $snapshot_ref
 *   Reference of the snapshot to destroy.
 */
function xs_actions_snapshot_destroy($snapshot_ref) {
  $snapshot = xs_get_snapshot_by_ref($snapshot_ref);
  xs_action_snapshot_vbd_destroy($snapshot);

  return xs_api_destroy_snapshot($snapshot_ref);
}

/**
 * Destroys VBD for the $snapshot.
 *
 * @param array $snapshot
 *   Snapshot array.
 */
function xs_action_snapshot_vbd_destroy($snapshot) {
  if (empty($snapshot['VBDs'])) {
    return;
  }

  foreach ($snapshot['VBDs'] as $vbd_ref) {
    $vbd = xs_api_get_vbd($vbd_ref);
    if ($vbd['VDI'] != XS_API_EMPTY_REF) {
      xs_api_destroy_vdi($vbd['VDI']);
    }
  }
}

/**
 * Reverts the snapshot.
 *
 * @param string $snapshot_ref
 *   Reference of the snapshot to destroy.
 */
function xs_actions_snapshot_revert($snapshot_ref) {
  xs_api_revert_snapshot($snapshot_ref);
}

/**
 * Loads snapshots as list form element options.
 *
 * @param array $snapshots
 *   All snapshots for the VM.
 * @param array $current_active_snapshot
 *   The snapshot that is currently active (if any).
 *
 * @return array
 *   Array of options, ref as the key and time as value.
 */
function _xs_actions_snapshot_prepare_options($snapshots, $current_active_snapshot) {
  $options = array();
  if (empty($snapshots)) {
    return $options;
  }

  $options[''] = t('Select snapshot');

  foreach ($snapshots as $ref => $snapshot) {
    // Mark the snapshot that is currently active.
    $active = !empty($current_active_snapshot['uuid']) && $snapshot['uuid'] == $current_active_snapshot['uuid'] ? t(' (Last reverted from)') : '';

    $options[$ref] = date("d M Y h:i:sA", $snapshot['snapshot_time']->timestamp) . $active;
  }

  return $options;
}
