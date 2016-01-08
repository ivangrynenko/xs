<?php
/**
 * @file
 * xs.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function xs_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'comment-comment_node_xs_vps-comment_body'.
  $field_instances['comment-comment_node_xs_vps-comment_body'] = array(
    'bundle' => 'comment_node_xs_vps',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'comment',
    'field_name' => 'comment_body',
    'label' => 'Comment',
    'required' => TRUE,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-xs_vps-field_xs_pool_uuid'.
  $field_instances['node-xs_vps-field_xs_pool_uuid'] = array(
    'bundle' => 'xs_vps',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter UUID of the pool in which the VM is running.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_xs_pool_uuid',
    'label' => 'Pool UUID',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-xs_vps-field_xs_vps_last_rrd_update'.
  $field_instances['node-xs_vps-field_xs_vps_last_rrd_update'] = array(
    'bundle' => 'xs_vps',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_xs_vps_last_rrd_update',
    'label' => 'Last RRD update',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-xs_vps-field_xs_vps_owner'.
  $field_instances['node-xs_vps-field_xs_vps_owner'] = array(
    'bundle' => 'xs_vps',
    'default_value' => array(
      0 => array(
        'target_id' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Select VPS owner.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_xs_vps_owner',
    'label' => 'Owner',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-xs_vps-field_xs_vps_snapshots_limit'.
  $field_instances['node-xs_vps-field_xs_vps_snapshots_limit'] = array(
    'bundle' => 'xs_vps',
    'default_value' => array(
      0 => array(
        'value' => 10,
      ),
    ),
    'deleted' => 0,
    'description' => 'Set the number of snapshots the user will be able to create.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_xs_vps_snapshots_limit',
    'label' => 'Max number of snapshots',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-xs_vps-field_xs_vps_uuid'.
  $field_instances['node-xs_vps-field_xs_vps_uuid'] = array(
    'bundle' => 'xs_vps',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the UUID of the VPS.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_xs_vps_uuid',
    'label' => 'VPS UUID',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Comment');
  t('Enter UUID of the pool in which the VM is running.');
  t('Enter the UUID of the VPS.');
  t('Last RRD update');
  t('Max number of snapshots');
  t('Owner');
  t('Pool UUID');
  t('Select VPS owner.');
  t('Set the number of snapshots the user will be able to create.');
  t('VPS UUID');

  return $field_instances;
}
