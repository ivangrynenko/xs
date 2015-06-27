<?php
/**
 * @file
 * xs.settings.inc
 * Contstants relevant to XS module.
 */

// Reister custom XS API autoloader.
spl_autoload_register('xs_autoloader', TRUE, TRUE);

require_once __DIR__ . '/xs.api.inc';

/**
 * SPL autoloader.
 *
 * @param string $class
 *   Class name.
 */
function xs_autoloader($class) {
  $filepath = DRUPAL_ROOT . '/sites/all/modules/xs/classes/xenapi.php';
  if ($class == 'XenApi' && file_exists($filepath)) {
    require_once $filepath;
  }
}

$conf['xs_pools'] = array(
  '283e03ee-226e-3e58-cd35-d9dcf1359a16' => array(
    'name' => 'Test Pool',
    'members' => array(
      '9c14a77a-d75a-4f47-8a59-e0e00810402a' => array(
        'name' => 'XS--1',
        'ip' => '10.1.1.23',
        'uuid' => '9c14a77a-d75a-4f47-8a59-e0e00810402a',
        'username' => 'root',
        'password' => 'yfnfif12',
      ),
    ),
  ),
);
