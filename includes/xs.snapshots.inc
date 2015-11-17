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
 */
function xs_actions_snapshots_callback($node) {
  // @todo: Get available snapshots.
  // @todo: For no snapshots, display message.
  // @todo: Build form for available snapshots with actions.
  // @todo: Set hard limit for the snapshots.
  global $xs_api;

  $xs_api = xs_get_xs_api();
  $vm = xs_get_vm_by_uuid($node);

  $snapshots = xs_get_vm_snapshots($vm);

  if (empty($snapshots)) {
    return t('Your server does not have snapshots.');
  }

  return xs_actions_snapshots_output_render($snapshots, $node, $vm);
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
 * Renders output for the snapshots page.
 *
 * @param array $snapshots
 *   Array of fully loaded snapshot objects.
 * @param object $node
 *   Drupal node object.
 *
 * @return string
 * @throws \Exception
 */
function xs_actions_snapshots_output_render($snapshots, $node, $vm) {
  if (empty($snapshots)) {
    return '';
  }

  $headers = array(
    'Snapshot date',
    'Name',
    'Description',
    'Status',
  );

  $rows = array();
  foreach ($snapshots as $snap_ref => $snapshot) {
    $is_current_state = $vm['parent'] == $snap_ref;

    $rows[] = array(
      date("d M Y h:i:sA", $snapshot['snapshot_time']->timestamp),
      $snapshot['name_label'],
      $snapshot['name_description'],
      $is_current_state ? t('Last reverted from') : '',
    );
  }

  $table = theme('table', array('header' => $headers, 'rows' => $rows));
  $form = drupal_get_form('xs_actions_snapshot_form', $snapshots, $node->nid);

  $header = '<h3>Available snapshots</h3>';

  return $header . $table . render($form);
}

/**
 * Form builder for snapshot actions.
 */
function xs_actions_snapshot_form($form, &$form_state, $snapshots, $nid) {
  global $xs_api;

  $xs_api = xs_get_xs_api();

  $form['snapshot_uuid'] = array(
    '#type' => 'select',
    '#options' => _xs_actions_snapshot_prepare_options($snapshots),
  );

  $form['snapshot_actions'] = array(
    '#type' => 'select',
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

  $form['#validate'][] = 'xs_actions_snapshot_form_validate';
  $form['#submit'][] = 'xs_actions_snapshot_form_submit';

  return $form;
}

/**
 * Validation handler for xs_actions_snapshot_form.
 */
function xs_actions_snapshot_form_validate($form, &$form_state) {
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
 * Submit handler for xs_actions_snapshot_form.
 */
function xs_actions_snapshot_form_submit($form, &$form_state) {
  // Redirect to the confirmation page.
  drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots/' . $form_state['values']['snapshot_actions'] . '/' . $form_state['values']['snapshot_uuid']);
}

/**
 * Confirmation form for snapshot action.
 */
function xs_actions_confirm_snapshots_form($form, &$form_state, $node, $action, $snapshot_ref) {
  global $xs_api;

  $xs_api = xs_get_xs_api();

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
 *
 * @return array
 *   Array of options, ref as the key and time as value.
 */
function _xs_actions_snapshot_prepare_options($snapshots) {
  $options = array();
  if (empty($snapshots)) {
    return $options;
  }

  $options[''] = t('Select snapshot');

  foreach ($snapshots as $ref => $snapshot) {
    $options[$ref] = date("d M Y h:i:sA", $snapshot['snapshot_time']->timestamp);
  }

  return $options;
}
