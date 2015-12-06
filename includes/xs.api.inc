<?php
/**
 * @file
 * xs.api.inc
 * Xen API functions here.
 */

/**
 * Loads all configured xenserver pools.
 *
 * @return mixed
 *   Array of loaded pool definitions or an empty array.
 */
function xs_get_pools() {
  $xs_pools = variable_get('xs_pools', array());

  if (empty($xs_pools)) {
    drupal_set_message('Configure XS pools and servers. Check the module README file for the details.', 'error');
  }

  return $xs_pools;
}

/**
 * Initialises connection to the server and XenAPI class.
 *
 * @throws Exception
 *   Exception.
 *
 * @return XenApi
 *   XenApi class instance.
 *
 * @throws Exception
 *   Exception.
 */
function xs_get_xs_api() {
  $xs_api = &drupal_static(__FUNCTION__);

  if (!empty($xs_api) && $xs_api instanceof XenApi) {
    return $xs_api;
  }

  // Get all defined pools.
  $xs_pools = xs_get_pools();
  if (empty($xs_pools)) {
    throw new Exception('Unable to get pool data');
  }

  // Loop through defined pools to get array of members (hosts).
  foreach ($xs_pools as $pool_uuid => $xs_pool) {
    // Load members defined for this pool.
    $xs_pool_members = xs_get_pool_members($pool_uuid);
    // Loop through every member and attempt to create a connection.
    foreach ($xs_pool_members as $pool_member) {
      // In multi-server environment we attempt to connect to every server
      // that is defined in the pool and to identify pool master. Once the pool
      // master is identified, we connect to it and skip other servers.
      if (!empty($xs_master_api[$pool_uuid])) {
        continue;
      }

      try {
        $xs_api[$pool_uuid] = xs_api_connect($pool_member);
      }
      catch (Exception $e) {
        watchdog('XS', 'Error when connecting to XS: !error', array('!error' => $e->getMessage()), WATCHDOG_ERROR);

        throw new Exception(t('Error when connecting to XS: !error', array('!error' => $e->getMessage())));
      }

      $xs_host[$pool_uuid] = xs_get_host_by_uuid($xs_api[$pool_uuid], $pool_member['uuid']);
      $status[$pool_uuid] = xs_check_server_status($xs_host[$pool_uuid]);
      // If this server is down or under maintenance, skip it.
      if (!$status[$pool_uuid]) {
        continue;
      }
      // Pool object returned from XS API.
      $xs_api_pool_id[$pool_uuid] = $xs_api[$pool_uuid]->pool_get_by_uuid($pool_uuid);
      $xs_api_pool[$pool_uuid] = $xs_api[$pool_uuid]->pool_get_record($xs_api_pool_id[$pool_uuid]);
      $pool_master[$pool_uuid] = xs_api_get_pool_master($xs_api[$pool_uuid], $xs_api_pool[$pool_uuid]);

      // Reconnect to the pool master.
      // @todo: Add skip re-connect if we connected to the master above.
      $pool_master_local = $xs_pools[$pool_uuid]['members'][$pool_master[$pool_uuid]['uuid']];
      // Skip reconnect to pool master if we already connected to it.
      if ($pool_master_local['uuid'] != $pool_member['uuid']) {
        $xs_master_api[$pool_uuid] = xs_api_connect($pool_master_local);
      }
      else {
        $xs_master_api[$pool_uuid] = $xs_api[$pool_uuid];
      }
    }
  }

  // For now only single pool is supported.
  $xs_api = reset($xs_master_api);

  if ($xs_api instanceof XenApi) {
    return $xs_api;
  }

  throw new Exception('Unable to connect to host in xs_get_xs_api()');
}

/**
 * Connects to XS pool member and initialises XAPI.
 *
 * @param array $pool_member
 *   Locally defined pool member.
 *
 * @return \XenApi
 *   XAPI object.
 */
function xs_api_connect($pool_member) {
  try {
    $xs_api = new XenApi('https://' . $pool_member['ip'], $pool_member['username'], $pool_member['password']);
  }
  catch (Exception $e) {
    watchdog('XS', 'Error when connecting to XS: !error', array('!error' => $e->getMessage()), WATCHDOG_ERROR);
  }

  return $xs_api;
}

/**
 * Loads pool master host object via XAPI.
 *
 * @param XenApi $xs_api
 *   XenAPI object.
 * @param array $pool
 *   Pool object loaded via XAPI.
 *
 * @return array
 *   Pool master host object.
 */
function xs_api_get_pool_master($xs_api, $pool) {
  if (empty($pool['master'])) {
    return FALSE;
  }

  // Otherwise load master server.
  return xs_api_get_host($xs_api, $pool['master']);
}

/**
 * Loads Host object via XAPI.
 *
 * @param XenApi $xs_api
 *   XAPI object.
 * @param string $xs_host_ref
 *   Host reference.
 *
 * @return mixed
 */
function xs_api_get_host($xs_api, $xs_host_ref) {
  return $xs_api->host_get_record($xs_host_ref);
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
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $vms_array = $xs_api->VM_get_all_records();

  $templates = array();
  foreach ($vms_array as $vm) {
    if (xs_is_a_template($vm)) {
      $templates[] = $vm;
    }
  }

  return $templates;
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
  return (bool) $vm['is_a_template'];
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
  return (bool) $vm['is_control_domain'];
}

/**
 * Checks XS Host status.
 *
 * @param array $host
 *   XS Host array loaded via XS XML-RPC API.
 *
 * @return bool
 *   TRUE if host is running or FALSE if not.
 */
function xs_check_server_status($host) {
  return !empty($host['enabled']) && $host['enabled'] == TRUE;
}

/**
 * Loads XS host via XML-RPC API.
 *
 * @param XenApi $xs_api
 *   XAPI object.
 * @param string $uuid
 *   Host UUID.
 *
 * @return mixed
 *   Array of XS host loaded via XS API or FALSE.
 */
function xs_get_host_by_uuid($xs_api, $uuid) {
  $xs_host = array();

  if (!($xs_api instanceof XenApi)) {
    return $xs_host;
  }

  try {
    $xs_host_ref = $xs_api->host_get_by_uuid($uuid);
  }
  catch (Exception $e) {
    xs_log($e);
  }

  try {
    $xs_host = $xs_api->host_get_record($xs_host_ref);
  }
  catch (Exception $e) {
    xs_log($e);
  }

  return $xs_host;
}

/**
 * Loads all VMs running in XS Host.
 *
 * @param $host
 *   Host array loaded from XS API.
 *
 * @return array
 *   All VMs resident in the $host in 'running' state.
 */
function xs_get_running_vms($host) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $all_vms = array();
  $vms = array();
  if (!empty($host['resident_VMs'])) {
    foreach ($host['resident_VMs'] as $vm) {
      $all_vms[] = $xs_api->VM_get_record($vm);
    }
  }

  if (!empty($all_vms)) {
    foreach ($all_vms as $vm) {
      // Filter out control domain.
      if (!xs_is_a_template($vm) && !xs_is_domain_controller($vm)) {
        $vms[] = $vm;
      }
    }
  }

  return $vms;
}

/**
 * Get all VMs from the $server via the XenAPI.
 *
 * @param array $host
 *   Optional XS Host to filter all VMs by.
 *
 * @return mixed
 *   All VMs array or FALSE. If $host is XS Host array, filters only VMs
 *   resident on that host.
 */
function xs_get_all_vms($host = NULL) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $all_vms = $xs_api->VM_get_all();
  $vms = array();

  if (!empty($all_vms)) {
    foreach ($all_vms as $vm) {
      $response = $xs_api->VM_get_record($vm);
      // Filter out control domain.
      if (!xs_is_a_template($response) && !xs_is_domain_controller($response)) {
        // @todo: if $host passed, filter by host.
        $vms[] = $response;
      }
    }
  }

  return $vms;
}

/**
 * Gets VM object via XAPI.
 *
 * @param string $uuid
 *   VM's UUID, as stored against VM content type.
 *
 * @return mixed
 */
function xs_api_get_vm_by_uuid($uuid) {
  $vm = &drupal_static(__FUNCTION__ . '-' . $uuid);

  if (!empty($vm)) {
    return $vm;
  }

  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $return = array();

  $vm_ref = xs_api_get_vm_ref_by_uuid($uuid);

  try {
    $vm = $xs_api->VM_get_record($vm_ref);
  }
  catch (Exception $e) {
    xs_log($e);

    return $return;
  }

  return $vm;
}

/**
 * Gets VM object via XAPI.
 *
 * @param string $uuid
 *   VM's UUID, as stored against VM content type.
 *
 * @return mixed
 *   VM ref string or error message.
 */
function xs_api_get_vm_ref_by_uuid($uuid) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $return = array();

  if (!($xs_api instanceof XenApi)) {
    return $return;
  }

  try {
    $vm_ref = $xs_api->VM_get_by_uuid($uuid);
  }
  catch (Exception $e) {
    xs_log($e);

    return $return;
  }

  return $vm_ref;
}

/**
 * Gets snapshot reference string by snapshot UUID.
 *
 * @param string $uuid
 *   Snapshot UUID.
 *
 * @return mixed
 *   Snapshot reference string or error.
 */
function xs_api_get_snapshot_ref_by_uuid($uuid) {
  return xs_api_get_vm_ref_by_uuid($uuid);
}

/**
 * Gets snapshot object by snapshot UUID.
 *
 * @param string $uuid
 *   Snapshot UUID.
 *
 * @return mixed
 *   Snapshot array or error.
 */
function xs_api_get_snapshot_by_uuid($uuid) {
  return xs_api_get_vm_by_uuid($uuid);
}

/**
 * Load pool member servers from locally defined pool array.
 *
 * @param string $pool_uuid
 *   Pool UUID string.
 *
 * @return array
 *   Array of locally defined member servers.
 */
function xs_get_pool_members($pool_uuid) {
  $pools = variable_get('xs_pools', array());
  if (!empty($pools[$pool_uuid]['members'])) {
    return $pools[$pool_uuid]['members'];
  }

  return array();
}

/**
 * Loads all servers in the $pool.
 *
 * @return mixed
 *   Array of XS pool members.
 */
function xs_get_pool_servers($pool) {
  $servers = array();
  if (!empty($pool['members'])) {
    foreach ($pool['members'] as $uuid => $member) {
      $servers[$uuid] = $member;
    }
  }

  return $servers;
}

function xs_api_get_vif($vif_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  return $xs_api->VIF_get_record($vif_ref);
}

/**
 * Gets VM Guest metrix.
 *
 * @param string $guest_metrics_ref
 *   VM Guest Metrics reference.
 *
 * @return mixed
 *   Array of guest metrics.
 */
function xs_api_get_vm_guest_metrix($vm) {
  $return = array();
  if (empty($vm['guest_metrics'])) {
    return $return;
  }

  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  try {
    $all_guest_metrics = $xs_api->VM_guest_metrics_get_all_records();
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get VM_guest_metrics_get_all_records for !vm', array('!vm' => $vm['name_label']), WATCHDOG_ERROR);
  }

  if (empty($all_guest_metrics[$vm['guest_metrics']]['uuid'])) {
    return $return;
  }

  try {
    $vm_get_metrix_ref = $xs_api->VM_guest_metrics_get_by_uuid($all_guest_metrics[$vm['guest_metrics']]['uuid']);
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get VM_guest_metrics_get_by_uuid for !vm', array('!vm' => $vm['name_label']), WATCHDOG_ERROR);
  }

  try {
    $return = $xs_api->VM_guest_metrics_get_record($vm_get_metrix_ref);
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get VM_guest_metrics_get_record for !vm', array('!vm' => $vm['name_label']), WATCHDOG_ERROR);
  }

  return $return;
}

/**
 * Gets VM_metrics object via XAPI.
 *
 * @param string $uuid
 *   VM_metrics UUID reference.
 *
 * @return mixed
 *   VM_metrics array.
 */
function xs_api_get_vm_metrics($vm) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $return = array();

  if (empty($vm['metrics'])) {
    return $return;
  }

  try {
    $return = $xs_api->VM_metrics_get_record($vm['metrics']);
  }
  catch (Exception $e) {
    watchdog('XS', 'Unable to get VM_guest_metrics_get_record for !vm', array('!vm' => $vm['name_label']), WATCHDOG_ERROR);
  }

  return $return;
}

/**
 * Reboot a VM in a clean way.
 *
 * @param string $vm_ref
 *   XenApi VM reference.
 */
function xs_api_vm_clean_reboot($vm_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VM_hard_reboot($vm_ref);
}

/**
 * Reboot a VM forcefully.
 *
 * @param string $vm_ref
 *   XenApi VM reference.
 */
function xs_api_vm_hard_reboot($vm_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VM_hard_reboot($vm_ref);
}

/**
 * Shutdown a VM in a clean way.
 *
 * @param string $vm_ref
 *   XenApi VM reference.
 */
function xs_api_vm_clean_shutdown($vm_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VM_clean_shutdown($vm_ref);
}

/**
 * Shutdown a VM forcefully.
 *
 * @param string $vm_ref
 *   XenApi VM reference.
 */
function xs_api_vm_hard_shutdown($vm_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VM_hard_shutdown($vm_ref);
}

/**
 * Start a VM.
 *
 * @param string $vm_ref
 *   XenApi VM reference.
 */
function xs_api_vm_start($vm_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VM_start($vm_ref, FALSE, FALSE);
}

/**
 * Load full snapshot object, given the snapshot reference.
 *
 * @param string $snap_ref
 *   Snapshot reference.
 *
 * @return array
 *   Snapshot array.
 */
function xs_api_get_snapshot_by_ref($snap_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  return $xs_api->VM_get_record($snap_ref);
}

/**
 * Destroys the snapshot.
 *
 * @param string $snap_ref
 *   Snapshot reference.
 *
 * @return void
 */
function xs_api_destroy_snapshot($snap_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  try {
    $xs_api->VM_destroy($snap_ref);
  }
  catch (Exception $e) {
    watchdog('XS_API', 'Exception thrown while in !function: !message', array(
      '!function' => __FUNCTION__,
      '!message' => $e->getMessage()
    ));
  }
}

/**
 * Reverts the snapshot.
 *
 * @param string $snap_ref
 *   Snapshot reference.
 *
 * @return void
 */
function xs_api_revert_snapshot($snap_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  try {
    $xs_api->VM_revert($snap_ref);
  }
  catch (Exception $e) {
    return $e->getMessage();
  }
}

/**
 * Destroys a VBD by VBD reference.
 *
 * @param string $vbd_ref
 *   Reference.
 *
 * @return void
 */
function xs_api_vbd_destory($vbd_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VBD_destroy($vbd_ref);
}

/**
 * Load VBD via vbd reference.
 *
 * @param string $vbd_ref
 *   VBD reference.
 *
 * @return bool|array
 *   VBD array.
 */
function xs_api_get_vbd($vbd_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  return $xs_api->VBD_get_record($vbd_ref);
}

/**
 * Gets VDI by reference.
 *
 * @param string $vdi_ref
 *   Reference.
 *
 * @return array
 *   VDI representation.
 */
function xs_api_get_vdi($vdi_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  return $xs_api->VDI_get_record($vdi_ref);
}

/**
 * Destroys VDI by reference.
 *
 * @param string $vdi_ref
 *   VDI reference.
 */
function xs_api_destroy_vdi($vdi_ref) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $xs_api->VDI_destroy($vdi_ref);
}

/**
 * Creates a snapshot.
 *
 * @param string $vm_ref
 *   VM reference.
 * @param string $title
 *   Title of the snapshot.
 */
function xs_api_create_snapshot($vm_ref, $title) {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    xs_log($e);
    throw new Exception('Unable to create snapshot: ' . $e->getMessage());
  }

  $xs_api->VM_snapshot($vm_ref, $title);
}

/**
 * Gets total disk space for all HDDs.
 *
 * @param array $vbd_refs
 *   VBD reference.
 *
 * @return int|void
 *   All disks size in bytes.
 */
function xs_api_get_disk_space($vbd_refs) {
  $hdd_size = 0;
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  foreach ($vbd_refs as $vbd_ref) {
    $vbd_type = $xs_api->VBD_get_type($vbd_ref);
    if ($vbd_type == 'Disk') {
      $vdi_ref = $xs_api->VBD_get_VDI($vbd_ref);
      $hdd_size += intval($xs_api->VDI_get_virtual_size($vdi_ref));
    }
  }

  return $hdd_size;
}

/**
 * Loads VDI by VBD ref.
 *
 * @param string $vbd_ref
 *   VBD reference.
 * @param string $type
 *   Disk type, values range:
 *   - 'Disk': HDD
 *   - 'CD': CD.
 *
 * @return int|void
 *   All disks size in bytes.
 */
function xs_api_get_vdi_by_vbd($vbd_ref, $type = 'Disk') {
  try {
    $xs_api = xs_get_xs_api();
  }
  catch (Exception $e) {
    return xs_log($e);
  }

  $vbd = FALSE;
  $vbd_type = $xs_api->VBD_get_type($vbd_ref);
  if ($vbd_type == $type) {
    $vdi_ref = $xs_api->VBD_get_VDI($vbd_ref);
    $vbd = xs_api_get_vdi($vdi_ref);
  }

  return $vbd;
}
