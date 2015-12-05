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
  drupal_set_title(t('Snapshots of: !server', array('!server' => $node->title)));

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
  $full_snapshots = &drupal_static(__FUNCTION__ . $vm['uuid']);
  if (!empty($full_snapshots)) {
    return $full_snapshots;
  }

  $full_snapshots = array();
  if (!empty($vm['snapshots'])) {
    foreach ($vm['snapshots'] as $snapshot_ref) {
      $full_snapshots[$snapshot_ref] = xs_api_get_snapshot_by_ref($snapshot_ref);
    }
  }

  $full_snapshots = xs_snapshots_reorder($full_snapshots);

  return $full_snapshots;
}

/**
 * Reorder snapshots by snapshot date.
 *
 * @param array $snapshots
 *   Array of loaded snapshot objects.
 * @param bool $ascending
 *   Whether to sort ascending (TRUE).
 *
 * @return array
 *   Reordered snapshots.
 */
function xs_snapshots_reorder($snapshots, $ascending = FALSE) {
  if (empty($snapshots)) {
    return $snapshots;
  };

  $remapped_snapshots = array();
  $ordered_snapshots = array();
  foreach ($snapshots as $snap_ref => $snapshot) {
    $snapshot['ref'] = $snap_ref;
    $remapped_snapshots[$snapshot['snapshot_time']->timestamp] = $snapshot;
  }

  if ($ascending) {
    ksort($remapped_snapshots, SORT_STRING);
  }
  else {
    krsort($remapped_snapshots, SORT_STRING);
  }


  foreach ($remapped_snapshots as $snapshot) {
    $ordered_snapshots[$snapshot['ref']] = $snapshot;
  }

  return $ordered_snapshots;
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
  $vm = xs_get_vm_by_node($node);

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
  $vm = xs_get_vm_by_node($node);

  return !empty($vm['snapshots']) ? count($vm['snapshots']) : FALSE;
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

  $form['#prefix'] = '<div id="myModal" class="modal fade" role="dialog" aria-labelledby="gridSystemModalLabel"><div class="modal-dialog" role="document"><div class="modal-content">';
  $form['#suffix'] = '</div></div></div>';

  $form['header'] = array(
    '#markup' => '<div class="modal-header">
                  <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                  <h4 class="modal-title" id="gridSystemModalLabel">Create new snapshot</h4>
                </div>',
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name your Snapshot'),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
    '#prefix' => '<div class="modal-body"><div class="row"><div class="col-md-12">',
    '#suffix' => '</div></div></div>',
    '#description' => t('Give a description meaningful to you. We record the exact date and time of snapshot creation for you. Limited to 64 characters. Click Cancel to close this modal window or Create to continue with snapshot creation.'),
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('modal-footer'),
    ),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#attributes' => array(
      'type' => 'button',
      'class' => array('btn btn-default'),
      'data-dismiss' => 'modal',
    ),
    '#value' => t('Cancel'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => XS_API_SNAPSHOT_CREATE_BUTTON_TITLE,
    '#attributes' => array(
      'type' => 'button',
      'class' => array('btn btn-primary'),
    ),
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
  $limit = xs_get_snapshots_limit($node);
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
  $vm_ref = xs_api_get_vm_ref_by_uuid($vm_uuid);

  try {
    xs_snapshot_create_snapshot($vm_ref, $form_state['values']['title']);
  }
  catch (Exception $e) {
    throw new Exception('Unable to create snapshot: ' . $e->getMessage());
  }

  $form_state['redirect'] = 'node/' . $form_state['storage']['xs_snapshot_create']['node']->nid . '/xs-snapshots';
}

/**
 * Creates a snapshot of the VM identified by $vm_ref.
 *
 * @param string $vm_ref
 *   VM reference.
 * @param string $title
 *   Title of the snapshot.
 */
function xs_snapshot_create_snapshot($vm_ref, $title) {
  try {
    xs_api_create_snapshot($vm_ref, $title);
  }
  catch (Exception $e) {
    throw new Exception('Unable to create snapshot: ' . $e->getMessage());
  }
}

/**
 * Confirmation form for snapshot action.
 */
function xs_actions_confirm_snapshots_form($form, &$form_state, $node, $action, $snapshot_uuid) {
  drupal_add_css(drupal_get_path('module', 'xs') . '/assets/css/xs.node.css');

  // Add hidden fields to pass over to the action submit handler.
  $form['snapshot_uuid'] = array(
    '#type' => 'hidden',
    '#value' => $snapshot_uuid,
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $action,
  );

  $snapshot_created_time = xs_get_snapshot_created_time($snapshot_uuid);

  $date = date('d M Y h:i:sA', $snapshot_created_time);
  $form['warning'] = array(
    '#markup' => t('You about to !action the snapshot, created at !date. This action cannot be undone!', array(
      '!action' => strtoupper($action),
      '!date' => !empty($date) ? $date : 'N/A'
    )),
    '#prefix' => '<div class="red">',
    '#suffix' => '</div>',
  );

  if ($action == 'revert') {
    $form['warning-2'] = array(
      '#markup' => format_string('Before reverting the server to this snapshot, we recommend you to !snapshot of the current state of your server, so that you could revert to it later on.', array('!snapshot' => l(t('Create a snapshot'), 'node/' . $node->nid . '/xs-snapshots/create'))),
      '#prefix' => '<div class="red warning">',
      '#suffix' => '</div>',
    );
  }

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
  if (empty($form_state['values']['snapshot_uuid']) || empty($form_state['values']['action'])) {
    return form_set_error(NULL, 'Unable to complete the action. Validation error');
  }
}

/**
 * Submit handler for xs_actions_confirm_snapshots_form.
 */
function xs_actions_confirm_snapshots_form_submit($form, &$form_state) {
  switch ($form_state['values']['action']) {
    case 'destroy':
      $snapshot_ref = xs_api_get_vm_ref_by_uuid($form_state['values']['snapshot_uuid']);
      xs_actions_snapshot_destroy($snapshot_ref);
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
          $vm_ref = xs_api_get_vm_ref_by_uuid($uuid);

          // Shut down the VM.
          xs_actions_action_vps_hard_shutdown($vm_ref);

          // Revert to the selected snapshot.
          $snapshot_ref = xs_api_get_vm_ref_by_uuid($form_state['values']['snapshot_uuid']);
          xs_actions_snapshot_revert($snapshot_ref);

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
 * @param string $snapshot_uuid
 *   Snapshot UUID.
 *
 * @return int|bool
 *   Snapshot created timestamp or FALSE.
 */
function xs_get_snapshot_created_time($snapshot_uuid) {
  $snapshot = xs_api_get_snapshot_by_uuid($snapshot_uuid);
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
  $snapshot = xs_api_get_snapshot_by_ref($snapshot_ref);
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
    $active = !empty($current_active_snapshot['uuid']) && $snapshot['uuid'] == $current_active_snapshot['uuid'] ? t(' (Server\'s latest status)') : '';

    $prefix = '';
    if (!empty($snapshot['name_label'])) {
      $prefix = $snapshot['name_label'];
    }

    $options[$ref] = $prefix . ' [' . date("d M Y h:i:sA", $snapshot['snapshot_time']->timestamp) . '] ' . $active;
  }

  return $options;
}
