<?php

/**
 * Generates the snapshot-policy type editing form.
 *
 * @todo: in use.
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
    '#value' => t('Save snapshot-policy type'),
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
 *
 * @todo: In use.
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
 *
 * @todo: In use.
 */
function xs_snapshot_policy_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/xs/snapshot-policy-types/' . $form_state['xs_snapshot_policy_type']->type . '/delete';
}

/**
 * Snapshot Policy type delete form.
 *
 * @todo: In use.
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
 * Snapshot Policy type delete form submit handler.
 *
 * @todo: In use.
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

/**
 * Page to select snapshot-policy Type to add new snapshot-policy.
 *
 * @todo: NOT IN USE.
 */
function xs_snapshot_policy_admin_add_page() {
  $items = array();
  foreach (xs_snapshot_policy_types() as $task_type_key => $snapshot_policy_type) {
    $items[] = l(entity_label('xs_snapshot_policy_type', $snapshot_policy_type), 'snapshot-policy/add/' . $task_type_key);
  }
  return array(
    'list' => array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Select type of snapshot-policy to create.')
    )
  );
}

/**
 * Add new snapshot-policy page callback.
 *
 * @todo: not in use.
 */
function xs_snapshot_policy_add($type_id) {
  $snapshot_policy_type = xs_snapshot_policy_types($type_id);

  $task = entity_create('xs_snapshot_policy', array(
    'type_id' => $type_id,
    'title' => $snapshot_policy_type->title,
    'uid' => $snapshot_policy_type->uid,
    'created' => $snapshot_policy_type->created,
    'changed' => $snapshot_policy_type->changed,
  ));

  //drupal_set_title(t('Create @name', array('@name' => entity_label('snapshot_policy_type', $snapshot_policy_type))));

//  $output = drupal_get_form('xs_snapshot_policy_form', $task);
//
//  return $output;
}

/**
 * Snapshot Policy Form.
 *
 * @todo: Not In use.
 */
function __xs_snapshot_policy_form($form, &$form_state, $snapshot_policy) {
  $form_state['snapshot-policy'] = $snapshot_policy;

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $snapshot_policy->title,
  );

//  $form['time'] = array(
//    '#type' => 'textfield',
//    '#required' => TRUE,
//    '#title' => t('Time in seconds'),
//    '#description' => t('Set the number of seconds between task runs'),
//    '#default_value' => $snapshot_policy->time,
//  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $snapshot_policy->uid,
  );

  field_attach_form('xs_snapshot_policy', $snapshot_policy, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save snapshot-policy'),
    '#submit' => $submit + array('xs_snapshot_policy_form_submit'),
  );

  // Show Delete button if we edit snapshot-policy.
  $task_id = entity_id('xs_snapshot_policy', $snapshot_policy);
  if (!empty($task_id) && xs_snapshot_policy_access('edit', $snapshot_policy)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('xs_snapshot_policy_form_submit_delete'),
    );
  }

  $form['#validate'][] = 'xs_snapshot_policy_form_validate';

  return $form;
}

function xs_snapshot_policy_form_validate($form, &$form_state) {
}

/**
 * Snapshot Policy submit handler.
 *
 * @todo: NOT IN USE.
 */
function xs_snapshot_policy_form_submit($form, &$form_state) {
  $snapshot_policy = $form_state['snapshot-policy'];

  entity_form_submit_build_entity('xs_snapshot_policy', $snapshot_policy, $form, $form_state);

  xs_snapshot_policy_save($snapshot_policy);

  $snapshot_policy_uri = entity_uri('xs_snapshot_policy', $snapshot_policy);

  $form_state['redirect'] = $snapshot_policy_uri['path'];

  drupal_set_message(t('Snapshot Policy %title saved.', array('%title' => entity_label('xs_snapshot_policy', $snapshot_policy))));
}

/**
 * @todo: NOT IN USE.
 */
function xs_snapshot_policy_form_submit_delete($form, &$form_state) {
  $task = $form_state['snapshot-policy'];
  $snapshot_policy_uri = entity_uri('xs_snapshot_policy', $task);
  $form_state['redirect'] = $snapshot_policy_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 *
 * @todo: NOT IN USE.
 */
function xs_snapshot_policy_delete_form($form, &$form_state, $task) {
  $form_state['snapshot-policy'] = $task;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['task_type_id'] = array(
    '#type' => 'value',
    '#value' => entity_id('xs_snapshot_policy', $task)
  );
  $snapshot_policy_uri = entity_uri('xs_snapshot_policy', $task);
  return confirm_form($form,
    t('Are you sure you want to delete snapshot-policy %title?', array('%title' => entity_label('xs_snapshot_policy', $task))),
    $snapshot_policy_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 *
 * @todo: NOT IN USE.
 */
function xs_snapshot_policy_delete_form_submit($form, &$form_state) {
  $task = $form_state['snapshot-policy'];
  xs_snapshot_policy_delete($task);

  drupal_set_message(t('Snapshot Policy %title deleted.', array('%title' => entity_label('xs_snapshot_policy', $task))));

  $form_state['redirect'] = '<front>';
}
