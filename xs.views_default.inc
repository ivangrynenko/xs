<?php
/**
 * @file
 * xs.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function xs_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'xs_action_log';
  $view->description = '';
  $view->tag = 'xs';
  $view->base_table = 'xs_action_log';
  $view->human_name = 'XS Action Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Server Actions Log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own xs action log';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_table_plugin_style';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'id' => 'id',
    'log' => 'log',
    'type' => 'type',
    'created' => 'created',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 1,
    ),
    'log' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['bootstrap_styles'] = array(
    'striped' => 0,
    'bordered' => 0,
    'hover' => 0,
    'condensed' => 0,
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<span class="glyphicon glyphicon-arrow-left"></span> <a href="/node/[nid]">Back</a> to the server overview page';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Server Action Log: Referenced Server */
  $handler->display->display_options['relationships']['target_id']['id'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['table'] = 'xs_action_log';
  $handler->display->display_options['relationships']['target_id']['field'] = 'target_id';
  /* Relationship: Server Action Log: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'xs_action_log';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'User';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Server Action Log: Server action log ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Server Action Log: Log */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  $handler->display->display_options['fields']['log']['label'] = 'Log Message';
  /* Field: Server Action Log: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Action Type';
  /* Field: Server Action Log: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date/Time';
  $handler->display->display_options['fields']['created']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'medium';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Action Username';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['value'] = '';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'xs_vps' => 'xs_vps',
  );
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['fail'] = 'access denied';
  /* Filter criterion: Server Action Log: Log */
  $handler->display->display_options['filters']['log']['id'] = 'log';
  $handler->display->display_options['filters']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['log']['field'] = 'log';
  $handler->display->display_options['filters']['log']['operator'] = 'contains';
  $handler->display->display_options['filters']['log']['exposed'] = TRUE;
  $handler->display->display_options['filters']['log']['expose']['operator_id'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['label'] = 'Search log message text';
  $handler->display->display_options['filters']['log']['expose']['operator'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['identifier'] = 'log';
  $handler->display->display_options['filters']['log']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['type_1']['group_info']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['group_info']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Start',
      'operator' => '=',
      'value' => 'start',
    ),
    2 => array(
      'title' => 'Reboot',
      'operator' => '=',
      'value' => 'clean_reboot',
    ),
    3 => array(
      'title' => 'Shutdown',
      'operator' => '=',
      'value' => 'clean_shutdown',
    ),
    4 => array(
      'title' => 'Hard Reboot',
      'operator' => '=',
      'value' => 'hard_reboot',
    ),
    5 => array(
      'title' => 'Hard Shutdown',
      'operator' => '=',
      'value' => 'hard_shutdown',
    ),
  );

  /* Display: User Page (Actions) */
  $handler = $view->new_display('page', 'User Page (Actions)', 'page_user_actions');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<span class="glyphicon glyphicon-arrow-left"></span> <a href="/node/[nid]#server-log">Back</a> to the server overview page';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Server Action Log: Log */
  $handler->display->display_options['filters']['log']['id'] = 'log';
  $handler->display->display_options['filters']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['log']['field'] = 'log';
  $handler->display->display_options['filters']['log']['operator'] = 'contains';
  $handler->display->display_options['filters']['log']['exposed'] = TRUE;
  $handler->display->display_options['filters']['log']['expose']['operator_id'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['label'] = 'Search log message text';
  $handler->display->display_options['filters']['log']['expose']['operator'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['identifier'] = 'log';
  $handler->display->display_options['filters']['log']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['type_1']['group_info']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['group_info']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Start',
      'operator' => '=',
      'value' => 'start',
    ),
    2 => array(
      'title' => 'Reboot',
      'operator' => '=',
      'value' => 'clean_reboot',
    ),
    3 => array(
      'title' => 'Shutdown',
      'operator' => '=',
      'value' => 'clean_shutdown',
    ),
    4 => array(
      'title' => 'Hard Reboot',
      'operator' => '=',
      'value' => 'hard_reboot',
    ),
    5 => array(
      'title' => 'Hard Shutdown',
      'operator' => '=',
      'value' => 'hard_shutdown',
    ),
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not_starts';
  $handler->display->display_options['filters']['type']['value'] = 'snapshot';
  $handler->display->display_options['path'] = 'node/%/action-log';

  /* Display: User Page (Snapshots) */
  $handler = $view->new_display('page', 'User Page (Snapshots)', 'page_user_snapshots');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<span class="glyphicon glyphicon-arrow-left"></span> <a href="/node/[nid]/xs-snapshots#snapshot-log">Back</a> to the snapshots overview page';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Server Action Log: Log */
  $handler->display->display_options['filters']['log']['id'] = 'log';
  $handler->display->display_options['filters']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['log']['field'] = 'log';
  $handler->display->display_options['filters']['log']['operator'] = 'contains';
  $handler->display->display_options['filters']['log']['exposed'] = TRUE;
  $handler->display->display_options['filters']['log']['expose']['operator_id'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['label'] = 'Search log message text';
  $handler->display->display_options['filters']['log']['expose']['operator'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['identifier'] = 'log';
  $handler->display->display_options['filters']['log']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['type_1']['group_info']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['group_info']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Create',
      'operator' => '=',
      'value' => 'snapshot-create',
    ),
    2 => array(
      'title' => 'Delete',
      'operator' => '=',
      'value' => 'snapshot-destroy',
    ),
    3 => array(
      'title' => 'Revert',
      'operator' => '=',
      'value' => 'snapshot-revert',
    ),
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'starts';
  $handler->display->display_options['filters']['type']['value'] = 'snapshot';
  $handler->display->display_options['path'] = 'node/%/snapshot-log';

  /* Display: Admin Page (All) */
  $handler = $view->new_display('page', 'Admin Page (All)', 'admin_page');
  $handler->display->display_options['display_description'] = 'Snapshot actions + server Actions';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '25, 50, 100, 250';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Server Action Log: Server action log ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['title']['label'] = 'Server Name';
  /* Field: Server Action Log: Log */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  $handler->display->display_options['fields']['log']['label'] = 'Log Message';
  /* Field: Server Action Log: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Action Type';
  /* Field: Server Action Log: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'xs_action_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date/Time';
  $handler->display->display_options['fields']['created']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'medium';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Action Username';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Filter by server name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Server Action Log: Log */
  $handler->display->display_options['filters']['log']['id'] = 'log';
  $handler->display->display_options['filters']['log']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['log']['field'] = 'log';
  $handler->display->display_options['filters']['log']['operator'] = 'contains';
  $handler->display->display_options['filters']['log']['group'] = 1;
  $handler->display->display_options['filters']['log']['exposed'] = TRUE;
  $handler->display->display_options['filters']['log']['expose']['operator_id'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['label'] = 'Search log message text';
  $handler->display->display_options['filters']['log']['expose']['operator'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['identifier'] = 'log';
  $handler->display->display_options['filters']['log']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Server Action Log: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  $handler->display->display_options['filters']['type_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_1']['expose']['operator_id'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['expose']['operator'] = 'type_1_op';
  $handler->display->display_options['filters']['type_1']['expose']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['type_1']['group_info']['label'] = 'Type';
  $handler->display->display_options['filters']['type_1']['group_info']['identifier'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Start',
      'operator' => '=',
      'value' => 'start',
    ),
    2 => array(
      'title' => 'Reboot',
      'operator' => '=',
      'value' => 'clean_reboot',
    ),
    3 => array(
      'title' => 'Shutdown',
      'operator' => '=',
      'value' => 'clean_shutdown',
    ),
    4 => array(
      'title' => 'Hard Reboot',
      'operator' => '=',
      'value' => 'hard_reboot',
    ),
    5 => array(
      'title' => 'Hard Shutdown',
      'operator' => '=',
      'value' => 'hard_shutdown',
    ),
    6 => array(
      'title' => 'Snapshot (Any)',
      'operator' => 'starts',
      'value' => 'snapshot',
    ),
    7 => array(
      'title' => 'Snapshot Destroy',
      'operator' => '=',
      'value' => 'snapshot-destroy',
    ),
    8 => array(
      'title' => 'Snapshot Create',
      'operator' => '=',
      'value' => 'snapshot-create',
    ),
    9 => array(
      'title' => 'Snapshot Revert',
      'operator' => '=',
      'value' => 'snapshot-revert',
    ),
  );
  /* Filter criterion: Server Action Log: User */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'xs_action_log';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'admin/structure/xs/action-log';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Action Log';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['xs_action_log'] = $view;

  $view = new view();
  $view->name = 'xs_policy_log';
  $view->description = '';
  $view->tag = 'xs';
  $view->base_table = 'xs_snapshot_policy_log';
  $view->human_name = 'XS Policy Log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent Policy Events';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer xs_snapshot_policy entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 20, 40, 60';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_table_plugin_style';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'uid' => 'uid',
    'id' => 'id',
    'log' => 'log',
    'created' => 'created',
    'type' => 'type',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'log' => array(
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
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['bootstrap_styles'] = array(
    'striped' => 0,
    'bordered' => 0,
    'hover' => 0,
    'condensed' => 0,
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The policy has not run yet.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Relationship: Snapshot Policy Log: Snapshot Policy */
  $handler->display->display_options['relationships']['policy_id']['id'] = 'policy_id';
  $handler->display->display_options['relationships']['policy_id']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['relationships']['policy_id']['field'] = 'policy_id';
  $handler->display->display_options['relationships']['policy_id']['required'] = TRUE;
  /* Relationship: Snapshot Policy: The Server node for the policy */
  $handler->display->display_options['relationships']['target_id']['id'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['relationships']['target_id']['field'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['relationship'] = 'policy_id';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Snapshot Policy: Author */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'policy_id';
  $handler->display->display_options['fields']['uid']['label'] = 'Owner';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Field: Snapshot Policy Log: Snapshot policy log ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Snapshot Policy Log: Log */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  $handler->display->display_options['fields']['log']['label'] = 'Message';
  /* Field: Snapshot Policy Log: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date/Time';
  $handler->display->display_options['fields']['created']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'medium';
  /* Field: Snapshot Policy Log: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Sort criterion: Snapshot Policy Log: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Snapshot Policy Log: Type */
  $handler->display->display_options['sorts']['type']['id'] = 'type';
  $handler->display->display_options['sorts']['type']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['sorts']['type']['field'] = 'type';
  /* Contextual filter: Snapshot Policy: Target_id */
  $handler->display->display_options['arguments']['target_id']['id'] = 'target_id';
  $handler->display->display_options['arguments']['target_id']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['arguments']['target_id']['field'] = 'target_id';
  $handler->display->display_options['arguments']['target_id']['relationship'] = 'policy_id';
  $handler->display->display_options['arguments']['target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['target_id']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Snapshot Policy: Author */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'policy_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Snapshot Policy Log: Log */
  $handler->display->display_options['filters']['log']['id'] = 'log';
  $handler->display->display_options['filters']['log']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['filters']['log']['field'] = 'log';
  $handler->display->display_options['filters']['log']['operator'] = 'contains';
  $handler->display->display_options['filters']['log']['exposed'] = TRUE;
  $handler->display->display_options['filters']['log']['expose']['operator_id'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['label'] = 'Search log message text';
  $handler->display->display_options['filters']['log']['expose']['operator'] = 'log_op';
  $handler->display->display_options['filters']['log']['expose']['identifier'] = 'log';
  $handler->display->display_options['filters']['log']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Snapshot Policy Log: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'contains';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['type']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['type']['group_info']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['group_info']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Skipped',
      'operator' => 'contains',
      'value' => 'skip',
    ),
    2 => array(
      'title' => 'Deleted',
      'operator' => 'contains',
      'value' => 'delete',
    ),
    3 => array(
      'title' => 'Created',
      'operator' => 'contains',
      'value' => 'create',
    ),
    4 => array(
      'title' => 'Failed',
      'operator' => 'contains',
      'value' => 'fail',
    ),
  );

  /* Display: User Page */
  $handler = $view->new_display('page', 'User Page', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Search Policy Events Log';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
  );
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<span class="glyphicon glyphicon-arrow-left"></span> <a href="/node/[nid]/xs-snapshot-policy">Back</a> to Policy overview page';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Snapshot Policy: Target_id */
  $handler->display->display_options['arguments']['target_id']['id'] = 'target_id';
  $handler->display->display_options['arguments']['target_id']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['arguments']['target_id']['field'] = 'target_id';
  $handler->display->display_options['arguments']['target_id']['relationship'] = 'policy_id';
  $handler->display->display_options['arguments']['target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['target_id']['exception']['value'] = '';
  $handler->display->display_options['arguments']['target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['target_id']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['target_id']['validate_options']['types'] = array(
    'xs_vps' => 'xs_vps',
  );
  $handler->display->display_options['arguments']['target_id']['validate_options']['access'] = TRUE;
  $handler->display->display_options['arguments']['target_id']['validate']['fail'] = 'access denied';
  $handler->display->display_options['path'] = 'node/%/xs-snapshot-policy/policies-log';

  /* Display: Admin Page */
  $handler = $view->new_display('page', 'Admin Page', 'page_1');
  $handler->display->display_options['display_description'] = 'Admin page to display all policy runs';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '25, 50, 100, 250';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Snapshot Policy: Author */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'xs_snapshot_policy';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'policy_id';
  $handler->display->display_options['fields']['uid']['label'] = 'Owner';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Field: Snapshot Policy Log: Snapshot policy log ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['title']['label'] = 'Host Name';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]';
  /* Field: Snapshot Policy Log: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Snapshot Policy Log: Log */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  $handler->display->display_options['fields']['log']['label'] = 'Message';
  /* Field: Snapshot Policy Log: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'xs_snapshot_policy_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date/Time';
  $handler->display->display_options['fields']['created']['date_format'] = 'today time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'medium';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'admin/structure/xs/policy-log';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Policies log';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['xs_policy_log'] = $view;

  $view = new view();
  $view->name = 'xs_user_vms';
  $view->description = '';
  $view->tag = 'xs';
  $view->base_table = 'node';
  $view->human_name = 'XS User VMs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Servers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You have no servers assigned. If you expect your server to be listed here, please contact our support';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Server Status */
  $handler->display->display_options['fields']['nid_2']['id'] = 'nid_2';
  $handler->display->display_options['fields']['nid_2']['table'] = 'node';
  $handler->display->display_options['fields']['nid_2']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_2']['ui_name'] = 'Server Status';
  $handler->display->display_options['fields']['nid_2']['label'] = '';
  $handler->display->display_options['fields']['nid_2']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'span';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Server Links */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['ui_name'] = 'Server Links';
  $handler->display->display_options['fields']['nid_1']['label'] = '';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['hide_alter_empty'] = FALSE;
  /* Field: Server Actions */
  $handler->display->display_options['fields']['nid_3']['id'] = 'nid_3';
  $handler->display->display_options['fields']['nid_3']['table'] = 'node';
  $handler->display->display_options['fields']['nid_3']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_3']['ui_name'] = 'Server Actions';
  $handler->display->display_options['fields']['nid_3']['label'] = '';
  $handler->display->display_options['fields']['nid_3']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Contextual filter: Content: Owner (field_xs_vps_owner) */
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['id'] = 'field_xs_vps_owner_target_id';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['table'] = 'field_data_field_xs_vps_owner';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['field'] = 'field_xs_vps_owner_target_id';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['exception']['value'] = '';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_xs_vps_owner_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'xs_vps' => 'xs_vps',
  );

  /* Display: Wide Block */
  $handler = $view->new_display('block', 'Wide Block', 'block');
  $handler->display->display_options['display_description'] = 'Block with action links and navigation links';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_bootstrap_table_plugin_style';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'nid_2' => 'nid_2',
    'title' => 'title',
    'nid_1' => 'nid_1',
    'nid_3' => 'nid_3',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid_3' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  $handler->display->display_options['style_options']['bootstrap_styles'] = array(
    'condensed' => 'condensed',
    'striped' => 0,
    'bordered' => 0,
    'hover' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Server Status */
  $handler->display->display_options['fields']['nid_2']['id'] = 'nid_2';
  $handler->display->display_options['fields']['nid_2']['table'] = 'node';
  $handler->display->display_options['fields']['nid_2']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_2']['ui_name'] = 'Server Status';
  $handler->display->display_options['fields']['nid_2']['label'] = 'Status';
  $handler->display->display_options['fields']['nid_2']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Hostname';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'span';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Server Links */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['ui_name'] = 'Server Links';
  $handler->display->display_options['fields']['nid_1']['label'] = 'Links';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['hide_alter_empty'] = FALSE;
  /* Field: Server Actions */
  $handler->display->display_options['fields']['nid_3']['id'] = 'nid_3';
  $handler->display->display_options['fields']['nid_3']['table'] = 'node';
  $handler->display->display_options['fields']['nid_3']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_3']['ui_name'] = 'Server Actions';
  $handler->display->display_options['fields']['nid_3']['label'] = 'Actions';
  $handler->display->display_options['fields']['nid_3']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['exception']['value'] = '';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';

  /* Display: Navigation Block */
  $handler = $view->new_display('block', 'Navigation Block', 'block_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['style_options']['hide'] = TRUE;
  $handler->display->display_options['style_options']['path'] = 'view_node';
  $handler->display->display_options['style_options']['default_value'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Server Status */
  $handler->display->display_options['fields']['nid_2']['id'] = 'nid_2';
  $handler->display->display_options['fields']['nid_2']['table'] = 'node';
  $handler->display->display_options['fields']['nid_2']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_2']['ui_name'] = 'Server Status';
  $handler->display->display_options['fields']['nid_2']['label'] = '';
  $handler->display->display_options['fields']['nid_2']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'span';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['view_node']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['view_node']['alter']['text'] = 'node/[nid]';
  $handler->display->display_options['fields']['view_node']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['exception']['value'] = '';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $export['xs_user_vms'] = $view;

  return $export;
}
