<?php
/**
 * @file
 * Views hooks for the XenServer module.
 */

/**
 * Implements hook_views_data_alter().
 */
function xs_views_data_alter(&$data) {
  $data['xs_action_log']['target_id']['relationship'] = array(
    'base' => 'node',
    'base field' => 'nid',
    'handler' => 'views_handler_relationship',
    'label' => t('Server'),
    'title' => t('Referenced Server'),
    'help' => t('The XenServer Action Log'),
  );
  $data['xs_action_log']['created']['field']['handler'] = 'views_handler_field_date';

  $data['xs_action_log']['uid'] = array(
    'title' => t('User'), // The item it appears as on the UI,
    'help' => t('The user who run server action'), // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('User who actioned'),
    ),
  );
}

/**
 * Implements hook_views_query_alter().
 */
function xs_views_query_alter(&$view, &$query) {
  if ($view->name == 'xs_action_log' && arg(0) == 'node' && arg(2) != 'action-log' && arg(2) != 'snapshot-log') {
    // Remove views header when in policy view.
    $view->header = array();
  }
}

