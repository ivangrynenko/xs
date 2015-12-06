<?php
/**
 * @file
 * Administration interface functions.
 */

/**
 * Generates the snapshot-policy type editing form.
 */
function xs_snapshot_policy_type_form($form, &$form_state, $snapshot_policy_type, $op = 'edit') {
  if ($op == 'clone') {
    $snapshot_policy_type->label .= ' (cloned)';
    $snapshot_policy_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $snapshot_policy_type->label,
    '#description' => t('The human-readable name of this snapshot-policy type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['time'] = array(
    '#title' => t('Time betwen runs'),
    '#type' => 'textfield',
    '#default_value' => $snapshot_policy_type->time,
    '#description' => t('Set time in seconds between each run for this policy'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($snapshot_policy_type->type) ? $snapshot_policy_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $snapshot_policy_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'xs_snapshot_policy_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this snapshot-policy type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  if (!$snapshot_policy_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete snapshot-policy type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('xs_snapshot_policy_type_form_submit_delete')
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing snapshot_policy_type.
 */
function xs_snapshot_policy_type_form_submit(&$form, &$form_state) {
  $form_state['values']['module'] = 'xs_snapshot_policy';
  $snapshot_policy_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  xs_snapshot_policy_type_save($snapshot_policy_type);

  // Redirect user back to list of snapshot-policy types.
  $form_state['redirect'] = 'admin/structure/xs/snapshot-policy-types';
}

/**
 * Deletes snapshot policy.
 */
function xs_snapshot_policy_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/xs/snapshot-policy-types/' . $form_state['xs_snapshot_policy_type']->type . '/delete';
}

/**
 * Snapshot Policy type delete form.
 */
function xs_snapshot_policy_type_form_delete_confirm($form, &$form_state, $snapshot_policy_type) {
  $form_state['snapshot_policy_type'] = $snapshot_policy_type;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['task_type_id'] = array(
    '#type' => 'value',
    '#value' => entity_id('xs_snapshot_policy_type', $snapshot_policy_type),
  );
  return confirm_form($form,
    t('Are you sure you want to delete snapshot-policy type %title?', array('%title' => entity_label('snapshot_policy_type', $snapshot_policy_type))),
    'snapshot-policy/' . entity_id('xs_snapshot_policy_type', $snapshot_policy_type),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Snapshot Policy type delete form validate handler.
 */
function xs_snapshot_policy_type_form_delete_confirm_validate($form, &$form_state) {
  
}

/**
 * Snapshot Policy type delete form submit handler.
 */
function xs_snapshot_policy_type_form_delete_confirm_submit($form, &$form_state) {
  $snapshot_policy_type = $form_state['snapshot_policy_type'];
  xs_snapshot_policy_type_delete($snapshot_policy_type);

  watchdog('xs_snapshot_policy_type', '@type: deleted %title.', array(
    '@type' => $snapshot_policy_type->type,
    '%title' => $snapshot_policy_type->label,
  ));
  drupal_set_message(t('@type %title has been deleted.', array(
    '@type' => $snapshot_policy_type->type,
    '%title' => $snapshot_policy_type->label,
  )));

  $form_state['redirect'] = 'admin/structure/xs/snapshot-policy-types';
}
