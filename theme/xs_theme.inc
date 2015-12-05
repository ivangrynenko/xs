<?php
/**
 * @file
 * xs_theme.inc
 */

/**
 * Implementation of hook_preprocess.
 */
function xs_preproces_xs_actions_snapshots_output(&$variables) {
  // Prepare variables.
  $vm = xs_get_vm_by_node($variables['node']);

  // If VM is not available, we were unable to connect to any of XS hosts.
  // Instead of displaying the confusing content, show the status message
  // and exit.
  if (empty($vm)) {
    $variables['error_message'] = t('API Error. Unable to connect to the server. If this error persists, please contact our support');

    return;
  }

  $snapshots = xs_get_vm_snapshots($vm);

  $variables['link'] = l('> Create new snapshot', format_string('node/!nid/xs-snapshots/create', array('!nid' => $variables['node']->nid)));
  $variables['link_url'] = url(format_string('node/!nid/xs-snapshots/create', array('!nid' => $variables['node']->nid)));
  $variables['new_snapshot_form'] = drupal_render(drupal_get_form('xs_snapshots_create_snapshot_form', $variables['node']->nid));

  $snapshots_qty = count($snapshots);
  $variables['snapshots_qty'] = format_plural($snapshots_qty, '@count snapshot', '@count snapshots');

  $page_variables = xs_snapshots_page_output_render($snapshots, $variables['node'], $vm);

  $variables['snapshots_max'] = (int) xs_get_snapshots_limit($variables['node']);
  $variables['snapshots_table'] = $page_variables['table'];
  $variables['snapshots_actions_form'] = $page_variables['form'];
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

  drupal_add_css(drupal_get_path('module', 'xs') . '/assets/css/xs.node.css');

  $date_timezone = new DateTimeZone($user->timezone);

  $headers = array (
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
    $snapshot_date = new DateTime($snapshot['snapshot_time']->scalar, $date_timezone);

    $is_current_state = $vm['parent'] == $snap_ref;

    // Detect if any snapshot is the parent of current server state.
    if ($vm['parent'] == $snap_ref) {
      $current_active_snapshot = $snapshot;
    }

    // Prepare actions link.
    $links = array();
    $links[] = '<div class="dropdown-links"><span class="glyphicon glyphicon-trash" aria-hidden="true"></span> ' . l('Delete', 'node/' . $node->nid . '/xs-snapshots/destroy/' . $snapshot['uuid'], array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';
    $links[] = '<div class="dropdown-links"><span class="glyphicon glyphicon-repeat" aria-hidden="true"></span> ' . l('Revert to', 'node/' . $node->nid . '/xs-snapshots/revert/' . $snapshot['uuid'], array('attributes' => array('class' => 'xs-glyphicon-link'))) . '</div>';

    $time = format_interval((time() - $snapshot_date->getTimestamp()) , 2) . t(' ago');

    // Build rows in the table.
    $rows[] = array(
      $i,
      theme('html_tag', array(
          'element' => array(
            '#tag' => 'span',
            '#attributes' => array(
              'class' => 'text-left',
              'data-toggle' => 'tooltip',
              'data-placement' => 'top',
              'title' => $snapshot_date->format(XS_DATE_FORMAT),
            ),
            '#value' => $time,
            '#value_prefix' => '<span class="glyphicon glyphicon-time" aria-hidden="true"></span> ',
          ),
        )
      ),
      theme('xs_snapshot_title_cell_theme', $snapshot),
      $is_current_state ? '<span class="glyphicon glyphicon-ok" aria-hidden="true"></span>' : '',
      theme('xs_links_dropdown', array('links' => $links, 'label' => t('Actions'))),
    );
    $i++;
  }

  $return_variables['table'] = theme('table', array(
    'header' => $headers,
    'rows' => $rows
  ));

  $form = drupal_get_form('xs_snapshots_action_snapshot_form', $snapshots, $node->nid, $current_active_snapshot);
  $return_variables['form'] = render($form);

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
function xs_snapshot_title_cell_theme($snapshot) {
  // Prepare snapshot wrapper class and tooltip.
  $class = 'text-primary';
  $tooltip_text = t('This snapshot was created manually');
  if (xs_snapshot_policy_is_automated_snapshot($snapshot)) {
    $class = 'text-success';
    $tooltip_text = t('This snapshot was created by an automated policy.');
  }

  return theme('html_tag', array(
      'element' => array(
        '#tag' => 'span',
        '#attributes' => array(
          'class' => 'text-left',
          'data-toggle' => 'tooltip',
          'data-placement' => 'top',
          'title' => $tooltip_text,
        ),
        '#value' => $snapshot['name_label'],
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
  $output = '<div class="btn-group">
  <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
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
