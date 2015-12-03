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
  $variables['policies_exceeded'] = xs_snapshot_policy_get_policies_count_per_user($author_uid, $variables['node']->nid) >= XS_SNAPSHOT_MAX_POLICIES_PER_USER;

  $spid = !empty($policy->spid) ? $policy->spid : NULL;
  $policy_form = drupal_get_form('xs_snapshot_policy_form', $variables['node']->nid, $spid);
  $variables['policy_form'] = drupal_render($policy_form);
}
