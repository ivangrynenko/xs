<?php
/**
 * @file
 * xs_theme.inc
 */

function xs_preproces_xs_actions_snapshots_output($variables) {
  if (!empty($variables['node'])) {
    // Prepare variables.
    $vm = xs_get_vm_by_uuid($variables['node']);
    $snapshots = xs_get_vm_snapshots($vm);

    $variables['link'] = l('> Create new snapshot', format_string('node/!nid/xs-snapshots/create', array('!nid' => $variables['node']->nid)));

    $snapshots_qty = count($snapshots);
    $variables['snapshots_qty'] = format_plural($snapshots_qty, '@count snapshot', '@count snapshots');

    $page_variables = xs_snapshots_page_output_render($snapshots, $variables['node'], $vm);

    $variables['snapshots_max'] = (int) xs_get_snapshots_limit($variables['node']);
    $variables['snapshots_table'] = $page_variables['table'];
    $variables['snapshots_actions_form'] = $page_variables['form'];
  }
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
  $headers = array(
    'Snapshot date',
    'Name',
    'Description',
    'Status',
  );

  $rows = array();
  $current_active_snapshot = array();
  foreach ($snapshots as $snap_ref => $snapshot) {
    $is_current_state = $vm['parent'] == $snap_ref;

    // Detect if any snapshot is the parent of current server state.
    if ($vm['parent'] == $snap_ref) {
      $current_active_snapshot = $snapshot;
    }

    $rows[] = array(
      date("d M Y h:i:sA", $snapshot['snapshot_time']->timestamp),
      $snapshot['name_label'],
      $snapshot['name_description'],
      $is_current_state ? t('Last reverted from') : '',
    );
  }

  $return_variables['table'] = theme('table', array(
      'header' => $headers,
      'rows' => $rows
    ));

  $form = drupal_get_form('xs_snapshots_action_snapshot_form', $snapshots, $node->nid, $current_active_snapshot);
  $return_variables['form'] = render($form);

  return $return_variables;
}
