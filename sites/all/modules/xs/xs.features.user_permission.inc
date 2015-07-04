<?php
/**
 * @file
 * xs.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function xs_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create xs_vps content'.
  $permissions['create xs_vps content'] = array(
    'name' => 'create xs_vps content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any xs_vps content'.
  $permissions['delete any xs_vps content'] = array(
    'name' => 'delete any xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own xs_vps content'.
  $permissions['delete own xs_vps content'] = array(
    'name' => 'delete own xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any xs_vps content'.
  $permissions['edit any xs_vps content'] = array(
    'name' => 'edit any xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own xs_vps content'.
  $permissions['edit own xs_vps content'] = array(
    'name' => 'edit own xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
