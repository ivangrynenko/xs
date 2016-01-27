<?php
/**
 * @file
 * xs_theme.inc
 */

/**
 * Implementation of hook_preprocess.
 */
function xs_preproces_xs_actions_snapshots_output(&$variables) {
  $variables['classes_array'] = array();
  $variables['attributes_array'] = array();
  $variables['title_attributes_array'] = array();
  $variables['content_attributes_array'] = array();
  $variables['error_message'] = '';

  $variables['xs_snapshot_actions_log'] = views_embed_view('xs_action_log', 'page_user_snapshots', $variables['node']->nid);

  // Prepare variables.
  $vm_uuid = xs_get_vm_uuid_by_node($variables['node']);
  try {
    $xs_vm = new XsVm($vm_uuid);
  }
  catch (Exception $e) {
    xs_log($e);
  }

  if (!empty($xs_vm)) {
    $vm = $xs_vm->getData();
  }

  // If VM is not available, we were unable to connect to any of XS hosts.
  // Instead of displaying the confusing content, show the status message
  // and exit.
  if (empty($vm)) {
    $variables['error_message'] = t('API Error. Unable to connect to the server. If this error persists, please contact our support');

    return;
  }

  $snapshots = $xs_vm->getSnapshots();

  $variables['link'] = l('> Create new snapshot', format_string('node/!nid/xs-snapshots/create', array('!nid' => $variables['node']->nid)));
  $variables['link_url'] = url(format_string('node/!nid/xs-snapshots/create', array('!nid' => $variables['node']->nid)));
  $snapshot_create_form = drupal_get_form('xs_snapshots_create_snapshot_form', $variables['node']->nid);
  $variables['new_snapshot_form'] = drupal_render($snapshot_create_form);

  $snapshots_qty = count($snapshots);
  $variables['snapshots_qty'] = format_plural($snapshots_qty, '@count snapshot', '@count snapshots');

  $page_variables = xs_snapshots_page_output_render($snapshots, $variables['node'], $vm);

  $variables['snapshots_max'] = (int) xs_get_snapshots_limit($variables['node']);
  $variables['snapshots_table'] = $page_variables['table'];

  $variables['link_class'] = $snapshots_qty >= $variables['snapshots_max'] ? 'btn-warning' : 'btn-success';
  $variables['link_glyphicon_class'] = $snapshots_qty >= $variables['snapshots_max'] ? 'glyphicon-warning-sign' : 'glyphicon-time';
}

/**
 * Renders variables for the snapshots page.
 *
 * @param array $snapshots
 *   Array of fully loaded snapshot objects.
 * @param object $node
 *   Drupal node object.
 * @params array $vm
 *   VM object from XenApi.
 *
 * @return array
 *   Array of variables.
 *
 * @throws \Exception
 *   Exception.
 */
function xs_snapshots_page_output_render($snapshots, $node, $vm) {
  global $user;

  drupal_page_is_cacheable(FALSE);
  drupal_add_css(drupal_get_path('module', 'xs') . '/assets/css/xs.node.css');

  $date_timezone = new DateTimeZone($user->timezone);

  $headers = array(
    '#',
    'Date Taken',
    'Name',
    'Last',
    'Actions',
  );

  $rows = array();
  $current_active_snapshot = array();

  $i = 1;
  foreach ($snapshots as $snap_ref => $snapshot) {
    $snapshot_date = new DateTime($snapshot->data['snapshot_time']->scalar, $date_timezone);

    $is_current_state = $vm['parent'] == $snap_ref;

    // Detect if any snapshot is the parent of current server state.
    if ($vm['parent'] == $snap_ref) {
      $current_active_snapshot = $snapshot->data;
    }

    // Prepare actions link.
    $links = array();
    $links[] = '<div class="dropdown-links"><i class="fa fa-trash"></i> ' . l('Delete', 'node/' . $node->nid . '/xs-snapshots/destroy/' . $snapshot->uuid, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
    $links[] = '<div class="dropdown-links"><i class="fa fa-history"></i> ' . l('Revert', 'node/' . $node->nid . '/xs-snapshots/revert/' . $snapshot->uuid, array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';

    $time = format_interval((time() - $snapshot_date->getTimestamp()), 2) . t(' ago');

    // Build rows in the table.
    $rows[] = array(
      $i,
      theme('html_tag', array(
          'element' => array(
            '#tag' => 'span',
            '#attributes' => array(
              'class' => 'text-left',
              'data-toggle' => 'tooltip',
              'data-placement' => 'bottom',
              'title' => $snapshot_date->format(XS_DATE_FORMAT),
            ),
            '#value' => $time,
            '#value_prefix' => '<span class="glyphicon glyphicon-time" aria-hidden="true"></span> ',
          ),
        )
      ),
      theme('xs_snapshot_title_cell_theme', array('snapshot' => $snapshot)),
      $is_current_state ? '<span class="glyphicon glyphicon-ok" aria-hidden="true"></span>' : '',
      theme('xs_links_dropdown', array(
        'links' => $links,
        'label' => t('Actions')
      )),
    );
    $i++;
  }

  $return_variables['table'] = theme('table', array(
    'header' => $headers,
    'rows' => $rows
  ));

  return $return_variables;
}

/**
 * Theme callback for 'xs_snapshot_row_theme' to render name in snapshot table.
 *
 * @param array $snapshot
 *   Snapshot array from XenApi.
 *
 * @return string
 *   HTML.
 */
function xs_snapshot_title_cell_theme($variables) {
  // Prepare snapshot wrapper class and tooltip.
  $class = 'text-primary';
  $tooltip_text = t('This snapshot was created manually');
  if (xs_snapshot_policy_is_automated_snapshot($variables['snapshot'])) {
    $class = 'text-success';
    $tooltip_text = t('This snapshot was created by an automated policy.');
  }

  return theme('html_tag', array(
      'element' => array(
        '#tag' => 'span',
        '#attributes' => array(
          'class' => 'text-left ' . $class,
          'data-toggle' => 'tooltip',
          'data-placement' => 'bottom',
          'title' => $tooltip_text,
        ),
        '#value' => check_plain($variables['snapshot']->data['name_label']),
      ),
    )
  );
}

/**
 * Builds bootstrap dropdown links.
 *
 * @param array $links
 *   Array of links.
 *
 * @return string
 *   HTML for the dropdown.
 */
function xs_links_dropdown_theme($variables) {
  if (empty($variables['label'])) {
    $variables['label'] = t('Action');
  }

  if (empty($variables['size'])) {
    $size = 'xs';
  }
  else {
    switch ($variables['size']) {
      case 'large':
        $size = 'lg';
        break;

      case 'small':
        $size = 'sm';
        break;

      case 'extra-small':
        $size = 'xs';
        break;

      default:
        $size = 'ignore';
        break;
    }
  }

  $output = '<div role="navigation" class="btn-group btn-group-' . $size . ' . ' . $variables['classes'] . '">
  <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
    <i class="fa fa-cogs"></i>
    ' . $variables['label'] . '
    <span class="caret"></span>
  </button>';

  $output .= '<ul class="dropdown-menu" aria-labelledby="dLabel">';
  foreach ($variables['links'] as $i => $link) {
    if ($i != 0) {
      $output .= '<li role="separator" class="divider"></li>';
    }
    $output .= '<li>' . $link . '</li>';
  }
  $output .= '</ul>';
  $output .= '</div>';

  return $output;
}

/**
 * Callback for xs_server_navigation theme.
 *
 * @return string
 *   HTML.
 */
function xs_server_navigation_theme() {
  global $user;

  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'title'));
  $query->condition('uid', $user->uid, '=');
  $query->condition('type', XS_API_VPS_CONTENT_TYPE, '=');
  $query->condition('status', '0', '<>');
  $result = $query->execute();

  $options[0] = '<li class="dropdown-header">Select Server</li>';
  $options[1] = '<li role="separator" class="divider"></li>';
  $options[2] = '<li>' . l('All Servers', 'user-vps') . '</li>';
  $i = 10;
  while ($row = $result->fetchAssoc()) {
    $nodes[$row['nid']] = $row['title'];
    $row_data = xs_get_server_status_html($row['nid']);
    $options[$i] = '<li role="separator" class="divider"></li>';
    $i++;
    $options[$i] = '<li><i class="fa fa-circle ' .  $row_data['color'] .'"></i>' . l($row['title'], 'node/' . $row['nid']) . '</li>';
    if (empty($title)) {
      if (arg(1) == $row['nid']) {
        $title = '<i class="fa fa-circle ' .  $row_data['color'] .'"></i>' . $row['title'];
        unset($options[$i]);
        unset($options[$i - 1]);
      }
    }
    $i++;
  }

  if (empty($title)) {
    $title = t('Select a Server');
  }

  $return = '<div class="block xs-server-navigation">
    <div class="dropdown">
      <div class="collapse navbar-collapse" id="bs-navbar-collapse-2">
        <ul class="nav navbar-nav">
          <li class="dropdown">
            <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Select a server <span class="caret"></span></a>
            <ul class="dropdown-menu dropdown-menu-right" aria-labelledby="dropdownMenu2">';

  $return .= implode("\n", $options);

  $return .= '</ul>
            </li>
          </ul>
        </div>
      </div>
    </div>';

  return $return;
}

/**
 * Callback for xs_server_navigation theme.
 *
 * @return string
 *   HTML.
 */
function xs_user_navigation_theme() {
  global $user;

  $query_params = drupal_get_query_parameters();
  $destination = '';

  if (!empty($query_params['q'])) {
    $destination = $query_params['q'];
  }

  if (!empty($query_params['destination'])) {
    $destination = $query_params['destination'];
  }

  $options = xs_actions_get_user_navigation_links($destination);

  $return = '<div class="block xs-user-navigation">
      <div class="dropdown">
      <div class="collapse navbar-collapse" id="bs-navbar-collapse-1">
        <ul class="nav navbar-nav">
          <li class="dropdown">
            <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">' . $user->mail . ' <span class="caret"></span></a>
            <ul class="dropdown-menu dropdown-menu-right" aria-labelledby="dropdownMenu3">';

  foreach ($options as $link) {
    $return .= '<li>' . $link . '</li>';
  }

  $return .= '</ul>
           </li>
         </ul>

       </div>
       </div>
       </div>';

  return $return;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function xs_preprocess_xs_server_thumbs_navigation(&$variables) {
  $node = xs_is_vm_context();
  if (empty($node)) {
    return;
  }

  $path = current_path();

  $variables['link_href_overview'] = url('node/' . $node->nid);
  $variables['link_class_active_overview'] = $path == 'node/' . $node->nid ? 'text-danger' : 'text-info';

  if (module_exists('xs_rrd')) {
    $variables['link_href_graphs'] = url('node/' . $node->nid . '/xs-performance');
    $variables['link_class_active_graphs'] = strstr($path, 'node/' . $node->nid . '/xs-performance') ? 'text-danger' : 'text-info';
  }

  $variables['link_href_snapshots'] = url('node/' . $node->nid . '/xs-snapshots');
  $variables['link_class_active_snapshots'] = strstr($path, 'node/' . $node->nid . '/xs-snapshots') ? 'text-danger' : 'text-info';

  if (module_exists('xs_snapshot_policy')) {
    $variables['link_href_snapshot_policy'] = url('node/' . $node->nid . '/xs-snapshot-policy');
    $variables['link_class_active_snapshot_policy'] = strstr($path, 'node/' . $node->nid . '/xs-snapshot-policy') ? 'text-danger' : 'text-info';
  }
}
