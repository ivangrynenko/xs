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
  $policies_limit = xs_snapshot_policy_get_policies_count_per_user($author_uid, $variables['node']->nid);
  $variables['number_of_policies'] = format_plural($policies_limit, '@count automated snapshot policy', '@count automated snapshot policies', array('@count' => $policies_limit));
  $variables['policies_exceeded'] = $policies_limit >= XS_SNAPSHOT_MAX_POLICIES_PER_USER;
  $new_snapshot_form = drupal_get_form('xs_snapshot_policy_create_form', $variables['node']->nid);
  $variables['new_snapshot_policy_form'] = drupal_render($new_snapshot_form);
}
