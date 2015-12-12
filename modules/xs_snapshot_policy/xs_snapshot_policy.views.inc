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
}

/**
 * Implements hook_views_query_alter().
 */
function xs_snapshot_policy_views_query_alter(&$view, &$query) {
  if ($view->name == 'xs_policy_log_runs_per_user' && arg(0) == 'node' && arg(2) == 'xs-snapshot-policy' && arg(3) != 'policies-log') {
    // Remove views header when in policy view.
    $view->header = array();
  }
}
