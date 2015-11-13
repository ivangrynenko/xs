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
