<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on xs_snapshot_policy being loaded from the database.
 *
 * This hook is invoked during $xs_snapshot_policy loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of $xs_snapshot_policy entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_xs_snapshot_policy_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a $xs_snapshot_policy is inserted.
 *
 * This hook is invoked after the $xs_snapshot_policy is inserted into the database.
 *
 * @param SnapshotPolicy $xs_snapshot_policy
 *   The $xs_snapshot_policy that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_xs_snapshot_policy_insert(SnapshotPolicy $xs_snapshot_policy) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('xs_snapshot_policy', $xs_snapshot_policy),
      'extra' => print_r($xs_snapshot_policy, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a $xs_snapshot_policy being inserted or updated.
 *
 * This hook is invoked before the $xs_snapshot_policy is saved to the database.
 *
 * @param SnapshotPolicy $xs_snapshot_policy
 *   The $xs_snapshot_policy that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_xs_snapshot_policy_presave(SnapshotPolicy $xs_snapshot_policy) {
  $xs_snapshot_policy->name = 'foo';
}

/**
 * Responds to a $xs_snapshot_policy being updated.
 *
 * This hook is invoked after the $xs_snapshot_policy has been updated in the database.
 *
 * @param SnapshotPolicy $xs_snapshot_policy
 *   The $xs_snapshot_policy that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_xs_snapshot_policy_update(SnapshotPolicy $xs_snapshot_policy) {
  db_update('mytable')
    ->fields(array('extra' => print_r($xs_snapshot_policy, TRUE)))
    ->condition('id', entity_id('xs_snapshot_policy', $xs_snapshot_policy))
    ->execute();
}

/**
 * Responds to $xs_snapshot_policy deletion.
 *
 * This hook is invoked after the $xs_snapshot_policy has been removed from the database.
 *
 * @param SnapshotPolicy $xs_snapshot_policy
 *   The $xs_snapshot_policy that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_xs_snapshot_policy_delete(SnapshotPolicy $xs_snapshot_policy) {
  db_delete('mytable')
    ->condition('pid', entity_id('xs_snapshot_policy', $xs_snapshot_policy))
    ->execute();
}

/**
 * Act on a xs_snapshot_policy that is being assembled before rendering.
 *
 * @param $xs_snapshot_policy
 *   The xs_snapshot_policy entity.
 * @param $view_mode
 *   The view mode the xs_snapshot_policy is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $xs_snapshot_policy->content prior to rendering. The
 * structure of $xs_snapshot_policy->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_xs_snapshot_policy_view($xs_snapshot_policy, $view_mode, $langcode) {
  $xs_snapshot_policy->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for xs_snapshot_policys.
 *
 * @param $build
 *   A renderable array representing the xs_snapshot_policy content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * xs_snapshot_policy content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the xs_snapshot_policy rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_xs_snapshot_policy().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_xs_snapshot_policy_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Acts on xs_snapshot_policy_type being loaded from the database.
 *
 * This hook is invoked during xs_snapshot_policy_type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of xs_snapshot_policy_type entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_xs_snapshot_policy_type_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a xs_snapshot_policy_type is inserted.
 *
 * This hook is invoked after the xs_snapshot_policy_type is inserted into the database.
 *
 * @param SnapshotPolicyType $xs_snapshot_policy_type
 *   The xs_snapshot_policy_type that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_xs_snapshot_policy_type_insert(SnapshotPolicyType $xs_snapshot_policy_type) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('xs_snapshot_policy_type', $xs_snapshot_policy_type),
      'extra' => print_r($xs_snapshot_policy_type, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a xs_snapshot_policy_type being inserted or updated.
 *
 * This hook is invoked before the xs_snapshot_policy_type is saved to the database.
 *
 * @param SnapshotPolicyType $xs_snapshot_policy_type
 *   The xs_snapshot_policy_type that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_xs_snapshot_policy_type_presave(SnapshotPolicyType $xs_snapshot_policy_type) {
  $xs_snapshot_policy_type->name = 'foo';
}

/**
 * Responds to a xs_snapshot_policy_type being updated.
 *
 * This hook is invoked after the xs_snapshot_policy_type has been updated in the database.
 *
 * @param SnapshotPolicyType $xs_snapshot_policy_type
 *   The xs_snapshot_policy_type that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_xs_snapshot_policy_type_update(SnapshotPolicyType $xs_snapshot_policy_type) {
  db_update('mytable')
    ->fields(array('extra' => print_r($xs_snapshot_policy_type, TRUE)))
    ->condition('id', entity_id('xs_snapshot_policy_type', $xs_snapshot_policy_type))
    ->execute();
}

/**
 * Responds to xs_snapshot_policy_type deletion.
 *
 * This hook is invoked after the xs_snapshot_policy_type has been removed from the database.
 *
 * @param SnapshotPolicyType $xs_snapshot_policy_type
 *   The xs_snapshot_policy_type that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_xs_snapshot_policy_type_delete(SnapshotPolicyType $xs_snapshot_policy_type) {
  db_delete('mytable')
    ->condition('pid', entity_id('xs_snapshot_policy_type', $xs_snapshot_policy_type))
    ->execute();
}

/**
 * Define default xs_snapshot_policy_type configurations.
 *
 * @return
 *   An array of default xs_snapshot_policy_type, keyed by machine names.
 *
 * @see hook_default_xs_snapshot_policy_type_alter()
 */
function hook_default_xs_snapshot_policy_type() {
  $defaults['main'] = entity_create('xs_snapshot_policy_type', array(
    // …
  ));
  return $defaults;
}

/**
 * Alter default xs_snapshot_policy_type configurations.
 *
 * @param array $defaults
 *   An array of default xs_snapshot_policy_type, keyed by machine names.
 *
 * @see hook_default_xs_snapshot_policy_type()
 */
function hook_default_xs_snapshot_policy_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}
