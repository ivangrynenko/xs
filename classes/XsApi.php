<?php
/**
 * @file
 * XsApi.php
 */

/**
 * Class XsApi.
 */
class XsApi {
  public $xsApi;
  public $sessionId;
  protected $poolMembers;

  public function __construct() {
    try {
      $this->init();
    }
    catch (Exception $e) {
      xs_log($e);
    }

    if (!empty($this->xsApi)) {
      try {
        $this->sessionId = $this->xsApi->getSessionId();
      }
      catch (Exception $e) {
        xs_log($e);
      }
    }
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
  private function init() {
    $xs_api = &drupal_static(__FUNCTION__);

    if (!empty($xs_api) && $xs_api instanceof XenApi) {
      $this->xsApi = $xs_api;

      return;
    }

    // Get all defined pools.
    $xs_pools = $this->getPools();
    if (empty($xs_pools)) {
      throw new Exception('Unable to get pool data');
    }

    // Loop through defined pools to get array of members (hosts).
    foreach ($xs_pools as $pool_uuid => $xs_pool) {
      // Load members defined for this pool.
      $xs_pool_members = $this->getPoolMembers($pool_uuid);
      // Loop through every member and attempt to create a connection.
      foreach ($xs_pool_members as $pool_member) {
        // In multi-server environment we attempt to connect to every server
        // that is defined in the pool and to identify pool master. Once the pool
        // master is identified, we connect to it and skip other servers.
        if (!empty($xs_master_api[$pool_uuid])) {
          continue;
        }

        try {
          $this->xsApi = $this->connect($pool_member);
        }
        catch (Exception $e) {
          watchdog('XS', 'Error when connecting to XS: !error', array('!error' => $e->getMessage()), WATCHDOG_ERROR);

          throw new Exception(t('Error when connecting to XS: !error', array('!error' => $e->getMessage())));
        }

        $xs_host[$pool_uuid] = $this->getHostByUuid($pool_member['uuid']);
        $status[$pool_uuid] = $this->getHostStatus($xs_host[$pool_uuid]);
        // If this server is down or under maintenance, skip it.
        if (!$status[$pool_uuid]) {
          continue;
        }
        // Pool object returned from XS API.
        try {
          $xs_api_pool_id[$pool_uuid] = $this->xsApi->pool_get_by_uuid($pool_uuid);
        }
        catch (Exception $e) {
          xs_log($e);
        }

        try {
          $xs_api_pool[$pool_uuid] = $this->xsApi->pool_get_record($xs_api_pool_id[$pool_uuid]);
        }
        catch (Exception $e) {
          xs_log($e);
        }

        try {
          $pool_master[$pool_uuid] = $this->getPoolMaster($xs_api_pool[$pool_uuid]);
        }
        catch (Exception $e) {
          xs_log($e);
        }

        // Reconnect to the pool master.
        // @todo: Add skip re-connect if we connected to the master above.
        $pool_master_local = $xs_pools[$pool_uuid]['members'][$pool_master[$pool_uuid]['uuid']];
        // Skip reconnect to pool master if we already connected to it.
        if ($pool_master_local['uuid'] != $pool_member['uuid']) {
          $xs_master_api[$pool_uuid] = $this->connect($pool_master_local);
        }
        else {
          $xs_master_api[$pool_uuid] = $this->xsApi;
        }
      }
    }

    // For now only single pool is supported.
    if (!empty($xs_master_api)) {
      $xs_api = reset($xs_master_api);
    }

    if ($xs_api instanceof XenApi) {
      $this->xsApi = $xs_api;

      return;
    }
    else {
      throw new Exception('Unable to connect to host in xs_get_xs_api()');
    }
  }

  private function getPools() {
    return variable_get('xs_pools', array());
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
  private function getPoolMembers($pool_uuid) {
    $pools = variable_get('xs_pools', array());
    if (!empty($pools[$pool_uuid]['members'])) {
      return $pools[$pool_uuid]['members'];
    }

    return array();
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
  private function connect($pool_member) {

    $xs_api = FALSE;
    try {
      $xs_api = new XenApi('https://' . $pool_member['ip'], $pool_member['username'], $pool_member['password']);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    return $xs_api;
  }

  /**
   * Loads XS host via XML-RPC API.
   *
   * @param string $uuid
   *   Host UUID.
   *
   * @return mixed
   *   Array of XS host loaded via XS API or FALSE.
   */
  private function getHostByUuid($uuid) {
    if (empty($this->xsApi)) {
      return FALSE;
    }
    $xs_host = array();

    try {
      $xs_host_ref = $this->xsApi->host_get_by_uuid($uuid);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    try {
      $xs_host = $this->xsApi->host_get_record($xs_host_ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    return $xs_host;
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
  private function getHostStatus($host) {
    return !empty($host['enabled']) && $host['enabled'] == TRUE;
  }

  /**
   * Loads pool master host object via XAPI.
   *
   * @param array $pool
   *   Pool object loaded via XAPI.
   *
   * @return array
   *   Pool master host object.
   */
  private function getPoolMaster($pool) {
    if (empty($pool['master'])) {
      return FALSE;
    }

    $master = &drupal_static(__FUNCTION__ . $pool['uuid']);
    if (!empty($master)) {
      return $master;
    }

    // Otherwise load master server.
    try {
      $master = $this->xsApi->host_get_record($pool['master']);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    return $master;
  }
}
