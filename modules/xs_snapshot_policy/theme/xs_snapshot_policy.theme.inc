<?php
/**
 * @file
 * Theme related functions for xs_snapshot_policy module.
 */

function xs_snapshot_policy_output_preprocess(&$variables) {
  if (empty($variables['node']) || $variables['node']->type != XS_API_VPS_CONTENT_TYPE) {
    return;
  }

  $variables['classes_array'] = array();
  $variables['attributes_array'] = array();
  $variables['title_attributes_array'] = array();
  $variables['content_attributes_array'] = array();

  $author_uid = xs_get_vps_owner_uid($variables['node']);
  $policy = xs_snapshot_policy_get_policies_per_user($author_uid, $variables['node']->nid);
  if (!empty($policy) && !empty($policy->title)) {
    $variables['policy_name'] = $policy->title;
    $variables['policy_snapshots'] = $policy->snapshots;
    $policy_types = entity_load('xs_snapshot_policy_type', array($policy->type_id));
    $policy_type = reset($policy_types);
    $variables['policy_type'] = $policy_type->label;
  }
  else {
    $variables['policy_name'] = '';
    $variables['policy_snapshots'] = '';
    $variables['policy_type'] = '';
  }

  $variables['policy_last_run'] = theme('html_tag', array(
      'element' => array(
        '#tag' => 'span',
        '#attributes' => array(
          'class' => 'text-left',
          'data-toggle' => 'tooltip',
          'data-placement' => 'bottom',
          'title' => date(XS_DATE_FORMAT, $policy->last_run),
        ),
        '#value' => format_interval((time() - $policy->last_run) , 2) . t(' ago'),
        '#value_prefix' => '<span class="glyphicon glyphicon-time" aria-hidden="true"></span> ',
      ),
    )
  );
  $variables['policies_exceeded'] = xs_snapshot_policy_get_policies_count_per_user($author_uid, $variables['node']->nid) >= XS_SNAPSHOT_MAX_POLICIES_PER_USER;

  $spid = !empty($policy->spid) ? $policy->spid : NULL;
  $policy_form = drupal_get_form('xs_snapshot_policy_form', $variables['node']->nid, $spid);
  $variables['policy_form'] = drupal_render($policy_form);
  $variables['policy_log_table'] = theme('xs_snapshot_policy_log', array('policy' => $policy));
}

/**
 * Theme callback for xs_snapshot_policy_log_theme
 * @param $variables
 *
 * @return string
 * @throws \Exception
 */
function xs_snapshot_policy_log_theme($variables) {
  $policy = $variables['policy'];
  if (empty($policy)) {
    return '';
  }

  $row_entities = xs_snapshot_policy_get_log_for_policy($policy);
  if (empty($row_entities)) {
    return '';
  }

  $headers = array(
    'Message',
    'Last run',
  );

  $rows = array();
  foreach ($row_entities as $id => $row_entity) {
    $log = reset($row_entity);

    if (strstr($log->type, 'error')) {
      $class = 'text-danger';
    }
    elseif (strstr($log->type, 'success')) {
      $class = 'text-success';
    }
    else {
      $class = 'text-info';
    }

    $time = format_interval((time() - $log->created), 2) . t(' ago');

    $rows[] = array(
      theme('html_tag', array(
          'element' => array(
            '#tag' => 'span',
            '#attributes' => array(
              'class' => $class,
            ),
            '#value' => $log->log,
          ),
        )
      ),
      theme('html_tag', array(
          'element' => array(
            '#tag' => 'span',
            '#attributes' => array(
              'data-toggle' => 'tooltip',
              'data-placement' => 'top',
              'title' => date(XS_DATE_FORMAT, $log->created),
            ),
            '#value' => $time,
          ),
        )
      ),
    );
  }

  return theme('table', array(
    'header' => $headers,
    'rows' => $rows,
  ));
}
