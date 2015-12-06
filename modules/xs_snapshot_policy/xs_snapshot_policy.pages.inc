<?php

/**
 * Snapshot Policy view callback.
 */
function xs_snapshot_policy_view($task) {
  drupal_set_title(entity_label('xs_snapshot_policy', $task));
  return entity_view('xs_snapshot_policy', array(entity_id('xs_snapshot_policy', $task) => $task), 'full');
}
