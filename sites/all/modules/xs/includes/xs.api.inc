<?php
/**
 * @file
 * xs.api.inc
 * Xen API functions here.
 */

/**
 * Initialises connection to the server and XenAPI class.
 *
 * @param object $server
 *   Server object created via Drupal.
 *
 * @return \XenApi
 */
function xs_get_xs_api($server) {
  module_load_include('module', 'password_field', 'password_field');
  $password = xs_get_server_password($server);
  $username = xs_get_server_username($server);
  try {
    $xs = new XenApi(xs_get_server_ip($server), $username, $password);
  }
  catch (Exception $e) {
    watchdog('XS', 'Error when connecting to XS: !error', array('!error' => $e->getMessage()), WATCHDOG_ERROR);
  }

  return $xs;
}


function xs_api_get_servers() {
  $all_known_servers = xs_get_pools();
  foreach ($all_known_servers as $server) {
    // Attempt to connect to each server to identify if they all are up.
    $xs = xs_get_xs_api($server);
    $master = $xs->host_get_master();
  }
}

function xs_api_get_pool_master($servers) {
  // Loop through every server in the pool. Try to connect to first and get
  // pool's master. If server is not available, connect to next server.
  foreach ($servers as $server) {
    try {
      $xenapi = xs_get_xs_api($server);
//      $server_objects = $xenapi->host_get_all_records();
      $server_objects = $xenapi->pool_get_all_records();
    }
    catch (Exception $e) {
      watchdog('XS', $e->getMessage(), WATCHDOG_ERROR);
    }

    foreach ($server_objects as $object) {

    }
  }
}

/**
 * Get all VMs from the $server via the XenAPI.
 *
 * @param object $server
 *   XS Server node.
 *
 * @return mixed
 *   All VMs array or FALSE.
 */
function xs_get_all_vms($server) {
  $xenapi = xs_get_xs_api($server);

  if (!($xenapi instanceof XenApi)) {
    drupal_set_message(t('Unable to initialise XAPI when connecting to !server', array('!server' => $server->name)), 'error');

    return FALSE;
  }
  else {
    $vms_array = $xenapi->VM_get_all_records();

    return $vms_array;
  }
}

/**
 * Get all VMs from the $server via the XenAPI.
 *
 * @param object $server
 *   XS Server node.
 *
 * @return mixed
 *   All VMs array or FALSE.
 */
function xs_get_all_templates($server) {
  $xenapi = xs_get_xs_api($server);

  if (!($xenapi instanceof XenApi)) {
    drupal_set_message(t('Unable to initialise XAPI when connecting to !server', array('!server' => $server->name)), 'error');

    return FALSE;
  }
  else {
    $vms_array = $xenapi->VM_get_all_records();

    $templates = array();
    foreach ($vms_array as $vm) {
      if (xs_is_a_template($vm)) {
        $templates[] = $vm;
      }
    }

    return $templates;
  }
}

/**
 * Checks if the $vm is a template.
 *
 * @param array $vm
 *   VM returned via XenAPI.
 *
 * @return bool
 *   TRUE if the $vm is a template or FALSE otherwise.
 */
function xs_is_a_template($vm) {
  return !empty($vm['is_a_template']);
}

/**
 * Checks if the VM is a Pool Master (or domain controller).
 *
 * @param array $vm
 *   VM array, as returned via XenAPI.
 *
 * @return bool
 *   TRUE if the VM is a Pool Master or FALSE otherwise.
 */
function xs_is_domain_controller($vm) {
  return !empty($vm['is_control_domain']);
}
