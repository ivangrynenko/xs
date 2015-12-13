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

  // Exported permission: 'administer xenserver pool'.
  $permissions['administer xenserver pool'] = array(
    'name' => 'administer xenserver pool',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs',
  );

  // Exported permission: 'administer xs_snapshot_policy entities'.
  $permissions['administer xs_snapshot_policy entities'] = array(
    'name' => 'administer xs_snapshot_policy entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'administer xs_snapshot_policy types'.
  $permissions['administer xs_snapshot_policy types'] = array(
    'name' => 'administer xs_snapshot_policy types',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'create xs_snapshot_policy entities'.
  $permissions['create xs_snapshot_policy entities'] = array(
    'name' => 'create xs_snapshot_policy entities',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'create xs_vps content'.
  $permissions['create xs_vps content'] = array(
    'name' => 'create xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
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

  // Exported permission: 'edit any xs_snapshot_policy entities'.
  $permissions['edit any xs_snapshot_policy entities'] = array(
    'name' => 'edit any xs_snapshot_policy entities',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'edit any xs_vps content'.
  $permissions['edit any xs_vps content'] = array(
    'name' => 'edit any xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own xs_snapshot_policy entities'.
  $permissions['edit own xs_snapshot_policy entities'] = array(
    'name' => 'edit own xs_snapshot_policy entities',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'edit own xs_vps content'.
  $permissions['edit own xs_vps content'] = array(
    'name' => 'edit own xs_vps content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'perform any vm actions'.
  $permissions['perform any vm actions'] = array(
    'name' => 'perform any vm actions',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs',
  );

  // Exported permission: 'perform own vm actions'.
  $permissions['perform own vm actions'] = array(
    'name' => 'perform own vm actions',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs',
  );

  // Exported permission: 'view any xs action log'.
  $permissions['view any xs action log'] = array(
    'name' => 'view any xs action log',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xs',
  );

  // Exported permission: 'view own xs action log'.
  $permissions['view own xs action log'] = array(
    'name' => 'view own xs action log',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs',
  );

  // Exported permission: 'view own xs_snapshot_policy entities'.
  $permissions['view own xs_snapshot_policy entities'] = array(
    'name' => 'view own xs_snapshot_policy entities',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  // Exported permission: 'view xs_snapshot_policy log'.
  $permissions['view xs_snapshot_policy log'] = array(
    'name' => 'view xs_snapshot_policy log',
    'roles' => array(
      'XS VPS Customer' => 'XS VPS Customer',
      'administrator' => 'administrator',
    ),
    'module' => 'xs_snapshot_policy',
  );

  return $permissions;
}
