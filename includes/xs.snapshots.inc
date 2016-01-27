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
 * Loads the number of available snapshots of the VM, loaded via Drupal node.
 *
 * @param array $node
 *   Drupal node
 *
 * @return bool|int
 *   Number of available snapshots or FALSE.
 */
function xs_get_vm_snapshots_count_from_node($node) {
  if (is_numeric($node)) {
    $node = node_load($node);
  }
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

  return !empty($vm['snapshots']) ? count($vm['snapshots']) : 0;
}

/**
 * Gets the snapshots limit for the VPS.
 *
 * @param object|int $node
 *   Drupal node object or nid.
 *
 * @return int|bool
 *   Number of snapshots or FALSE.
 */
function xs_get_snapshots_limit($node) {
  if (is_numeric($node)) {
    $node = node_load($node);
  }

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

  // Check if the user can add more snapshots.
  $limit = xs_get_snapshots_limit($node);
  $current_snapshots = xs_get_vm_snapshots_count_from_node($node);
  if ($current_snapshots >= $limit) {
    // Server snapshots exceeded. Remove the create form with a message.
    $form['message_error'] = array(
      '#prefix' => '<div class="modal-body"><div class="row"><div class="col-md-12"><p class="text-danger">',
      '#suffix' => '</p></div></div></div>',
      '#markup' => t('Your server is limited to have !num snapshots, but you have !snaps snapshots available.', array(
        '!num' => $limit,
        '!snaps' => $current_snapshots,
      )),
    );

    $form['message_details'] = array(
      '#prefix' => '<div class="modal-body"><div class="row"><div class="col-md-12">',
      '#suffix' => '</div></div></div>',
      '#markup' => t('Your server exceeded the maximum number of snapshots allowed. Remove older snapshots and try again.'),
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
      '#value' => t('Okay'),
    );

    return $form;
  }

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

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node,
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
  if (empty($form_state['values']['nid'])) {
    return form_set_error(NULL, 'Validation error. This form must be submitted in server context');
  }

  $node = node_load($form_state['values']['nid']);
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
  try {
    $xs_vm = new XsVm($vm_uuid);
  }
  catch (Exception $e) {
    xs_log($e);
  }

  if (!empty($xs_vm)) {
    $snapshot_title = check_plain($form_state['values']['title']);
    $xs_vm->createSnapshot($snapshot_title);
    xs_action_log_write(t('Created snapshot !name for !server', array(
      '!name' => $form_state['values']['title'],
      '!server' => $form_state['storage']['xs_snapshot_create']['node']->title,
    )), 'snapshot-create');
  }

  $form_state['redirect'] = 'node/' . $form_state['storage']['xs_snapshot_create']['node']->nid . '/xs-snapshots';
}

/**
 * Confirmation form for snapshot action.
 */
function xs_actions_confirm_snapshots_form($form, &$form_state, $node, $action, $snapshot_uuid) {
  drupal_add_css(drupal_get_path('module', 'xs') . '/assets/css/xs.node.css');

  // Add hidden fields to pass over to the action submit handler.
  $form['snapshot_uuid'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($snapshot_uuid),
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

  $date = date(XS_DATE_FORMAT, $snapshot_created_time);

  drupal_set_message(t('You requested to !action the snapshot, created at !date. This action cannot be undone!', array(
    '!action' => strtoupper($action),
    '!date' => !empty($date) ? $date : 'N/A'
  )), 'error');

  $form['warning'] = array(
    '#markup' => t('Are you sure that you want to !action the snapshot? This action cannot be undone!', array(
      '!action' => strtoupper($action),
      '!date' => !empty($date) ? $date : 'N/A'
    )),
    '#prefix' => '<div class="text-default">',
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

  // Do not allow form submission outside of node context.
  if (!(is_numeric(arg(1)) && arg(0) == 'node')) {
    return form_set_error(NULL, 'Validation error. Unable to submit your form outside of your server page');
  }
}

/**
 * Submit handler for xs_actions_confirm_snapshots_form.
 */
function xs_actions_confirm_snapshots_form_submit($form, &$form_state) {
  $snapshot = new XsSnapshot($form_state['values']['snapshot_uuid']);
  $node = node_load($form_state['values']['nid']);

  switch ($form_state['values']['action']) {
    case 'destroy':
      $snapshot->destroy();
      xs_action_log_write(t('Deleted snapshot !name for !server', array(
        '!name' => $snapshot->data['name_label'],
        '!server' => $node->title,
      )), 'snapshot-destroy');
      drupal_set_message('The selected snapshot was deleted successfully');
      drupal_goto('node/' . $form_state['values']['nid'] . '/xs-snapshots');
      break;

    case 'revert':
      if (xs_actions_access_callback($node)) {
        module_load_include('inc', 'xs', 'includes/xs.actions');

        // Load the VM.
        $vm_uuid = xs_get_vm_uuid_by_node($node);
        try {
          $xs_vm = new XsVm($vm_uuid);
        }
        catch (Exception $e) {
          xs_log($e);
        }

        // Shut down the VM.
        if (!empty($xs_vm)) {
          $xs_vm->stopForce();

          // Revert to the selected snapshot.
          $snapshot->revert();

          // Start the VM.
          $xs_vm->start();

          xs_action_log_write(t('Reverted server !server to snapshot !name', array(
            '!name' => $snapshot->data['name_label'],
            '!server' => $node->title,
          )), 'snapshot-revert');

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
  $snapshot = new XsSnapshot($snapshot_uuid);
  if (!empty($snapshot->data['snapshot_time'])) {
    return $snapshot->data['snapshot_time']->timestamp;
  }

  return FALSE;
}
