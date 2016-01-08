<?php
/**
 * @file
 * Views hooks for the XenServer Snapshot Policy module.
 */

/**
 * Implements hook_views_data_alter().
 */
function xs_snapshot_policy_views_data_alter(&$data) {
  $data['xs_snapshot_policy_log']['policy_id']['relationship'] = array(
    'base' => 'xs_snapshot_policy',
    'base field' => 'spid',
    'handler' => 'views_handler_relationship',
    'label' => t('Policy'),
    'title' => t('Snapshot Policy'),
    'help' => t('The XenServer Snapshot Policy'),
  );
  $data['xs_snapshot_policy_log']['created']['field']['handler'] = 'views_handler_field_date';
  $data['xs_snapshot_policy']['created']['field']['handler'] = 'views_handler_field_date';
  $data['xs_snapshot_policy']['updated']['field']['handler'] = 'views_handler_field_date';
  $data['xs_snapshot_policy']['last_run']['field']['handler'] = 'views_handler_field_date';

  $data['xs_snapshot_policy']['uid']['relationship'] = array(
    'base' => 'users',
    'base field' => 'uid',
    'handler' => 'views_handler_relationship',
    'label' => t('User'),
    'title' => t('The user who created policy'),
  );

  $data['xs_snapshot_policy']['target_id']['relationship'] = array(
    'base' => 'node',
    'base field' => 'nid',
    'handler' => 'views_handler_relationship',
    'label' => t('VM Node'),
    'title' => t('The Server node for the policy'),
  );

  $data['node']['nid']['relationship'] = array(
    'base' => 'xs_snapshot_policy',
    'base field' => 'target_id',
    'handler' => 'views_handler_relationship',
    'label' => t('VM Policy'),
    'title' => t('The Server node for the policy'),
    'group' => t('Entity Reference'),
  );

  $data['xs_snapshot_policy']['reverse_xs_snapshot_policy']['relationship'] = array(
    'base' => 'node',
    'base field' => 'nid',
    'field_name' => 'target_id',
    'field table' => 'xs_snapshot_policy',
    'field field' => 'target_id',
    'handler' => 'views_handler_relationship_entity_reverse',
    'label' => t('Referenced Server'),
    'title' => t('Server for the Policy'),
    'help' => t('Links back to node table to make server node fields available'),
    'group' => t('Entity Reference'),
  );
}

/**
 * Implements hook_views_query_alter().
 */
function xs_snapshot_policy_views_query_alter(&$view, &$query) {
  if ($view->name == 'xs_policy_log' && arg(0) == 'node' && arg(2) == 'xs-snapshot-policy' && arg(3) != 'policies-log') {
    // Remove views header when in policy view.
    $view->header = array();
  }
}
