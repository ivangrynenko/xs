<?php

/**
 * Implements hook_views_default_views().
 */
function xs_snapshot_policy_views_default_views() {
  $view = new view();
  $view->name = 'xs_snapshot_policies';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'xs_snapshot_policy';
  $view->human_name = 'XenServer Snapshot Policies';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All Policies Overview';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '50, 100, 250';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'spid' => 'spid',
    'title' => 'title',
    'name' => 'name',
    'created' => 'created',
    'nothing' => 'nothing',
    'last_run' => 'last_run',
  );
  $handler->display->display_options['style_options']['default'] = 'last_run';
  $handler->display->display_options['style_options']['info'] = array(
    'spid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_run' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Relationship: Snapshot Policy: The user who created policy */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  /* Relationship: Snapshot Policy: The Server node for the policy */
  $handler->display->display_options['relationships']['target_id']['id'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['relationships']['target_id']['field'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Server Name';
  /* Field: Snapshot Policy: Snapshot policy ID */
  $handler->display->display_options['fields']['spid']['id'] = 'spid';
  $handler->display->display_options['fields']['spid']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['spid']['field'] = 'spid';
  $handler->display->display_options['fields']['spid']['exclude'] = TRUE;
  /* Field: Snapshot Policy: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Policy Name';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]/xs-snapshot-policy';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Owner';
  /* Field: Snapshot Policy: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'medium';
  /* Field: Snapshot Policy: Last_run */
  $handler->display->display_options['fields']['last_run']['id'] = 'last_run';
  $handler->display->display_options['fields']['last_run']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['last_run']['field'] = 'last_run';
  $handler->display->display_options['fields']['last_run']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['last_run']['second_date_format'] = 'medium';
  /* Filter criterion: Snapshot Policy: Type_id */
  $handler->display->display_options['filters']['type_id']['id'] = 'type_id';
  $handler->display->display_options['filters']['type_id']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['filters']['type_id']['field'] = 'type_id';
  $handler->display->display_options['filters']['type_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_id']['expose']['operator_id'] = 'type_id_op';
  $handler->display->display_options['filters']['type_id']['expose']['label'] = 'Filter by Policy Type';
  $handler->display->display_options['filters']['type_id']['expose']['operator'] = 'type_id_op';
  $handler->display->display_options['filters']['type_id']['expose']['identifier'] = 'type_id';
  $handler->display->display_options['filters']['type_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/xs/snapshot-policy';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Policies';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views['xenserver_snapshot_policies'] = $view;

  return $views;
}
