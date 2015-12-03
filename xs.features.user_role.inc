<?php
/**
 * @file
 * xs.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function xs_user_default_roles() {
  $roles = array();

  // Exported role: XS VPS Customer.
  $roles['XS VPS Customer'] = array(
    'name' => 'XS VPS Customer',
    'weight' => 3,
  );

  return $roles;
}
