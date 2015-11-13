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

  // Exported field_instance: 'comment-comment_node_xs_vps-comment_body'
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

  // Exported field_instance: 'node-xs_vps-field_xs_pool_uuid'
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

  // Exported field_instance: 'node-xs_vps-field_xs_vps_uuid'
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
  t('Pool UUID');
  t('VPS UUID');

  return $field_instances;
}
