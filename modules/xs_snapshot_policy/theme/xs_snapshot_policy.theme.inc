<?php
/**
 * @file
 * Theme related functions for xs_snapshot_policy module.
 */

function xs_snapshot_policy_output_preprocess(&$variables) {
  $variables['new_snapshot_policy_form'] = drupal_render(drupal_get_form('xs_snapshot_policy_create_form', $variables['node']->nid));
}
