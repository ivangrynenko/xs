<?php
/**
 * @file
 * XsVm.php
 */

/**
 * Class XsVm.
 */
class XsVm extends XsApi {
  public $data;
  public $uuid;
  public $ref;
  public $metrics;
  public $guestMetrics;
  public $consoleUrl;
  public $vifs;
  public $vbds;
  public $snapshots;
  public $status;
  protected $consoles;

  public function __construct($uuid) {
    if (empty($this->xsApi)) {
      parent::__construct();
    }
    if (empty($this->xsApi)) {
      throw new Exception('Unable to connect to XenServer host. Exiting.');
    }
    $this->setVmUuid($uuid);
  }

  /**
   * Get VM by UUID, stored against $node.
   *
   * @return bool|mixed
   */
  public function getData() {
    $vm = &drupal_static(__FUNCTION__ . '-' . $this->uuid);
    if (!empty($vm)) {
      $this->data = $vm;

      return $vm;
    }

    $this->setRef();

    try {
      $vm = $this->xsApi->VM_get_record($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    $this->data = $vm;

    return $vm;
  }

  /**
   * Sets VM uuid.
   *
   * @param string $uuid
   *   UUID identified of VM.
   */
  protected function setVmUuid($uuid) {
    $this->uuid = $uuid;
  }

  /**
   * Sets VM reference.
   */
  protected function setRef() {
    $vmRef = &drupal_static(__FUNCTION__ . '-' . $this->ref);
    if (!empty($vmRef)) {
      $this->ref = $vmRef;
    }

    try {
      $this->ref = $this->xsApi->VM_get_by_uuid($this->uuid);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Gets VM metrics.
   *
   * @return array
   *   VM Metrics array.
   */
  protected function setMetrics() {
    $this->metrics = &drupal_static(__FUNCTION__ . '-' . $this->ref);
    if (!empty($this->metrics)) {
      return;
    }

    try {
      $this->metrics = $this->xsApi->VM_metrics_get_record($this->data['metrics']);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Gets VM Metrics.
   *
   * @return array
   *   VM Metrics.
   */
  public function getMetrics() {
    $this->setMetrics();

    return $this->metrics;
  }

  /**
   * Sets consoles references.
   *
   * @return array
   *   Array of console references.
   */
  protected function setVmConsoles() {
    try {
      $this->consoles = $this->xsApi->VM_get_consoles($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Sets VM console URL.
   */
  public function setConsoleUrl() {
    $this->setVmConsoles();

    try {
      $this->consoleUrl = $this->xsApi->console_get_location($this->consoles[0]);
    }
    catch (Exception $e) {
      xs_log($e);
    }
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
  public function getDiskSpace($vbd_refs) {
    $hdd_size = 0;
    foreach ($vbd_refs as $vbd_ref) {
      try {
        $vbd_type = $this->xsApi->VBD_get_type($vbd_ref);
      }
      catch (Exception $e) {
        xs_log($e);
      }

      if ($vbd_type == 'Disk') {
        try {
          $vdi_ref = $this->xsApi->VBD_get_VDI($vbd_ref);
        }
        catch (Exception $e) {
          xs_log($e);
        }

        try {
          $hdd_size += intval($this->xsApi->VDI_get_virtual_size($vdi_ref));
        }
        catch (Exception $e) {
          xs_log($e);
        }
      }
    }

    return $hdd_size;
  }

  /**
   * Gets VM Guest metrics.
   *
   * @return mixed
   *   Array of guest metrics.
   */
  protected function setGuestMetrics() {
    $vm = $this->getData();
    $return = array();
    if (empty($vm['guest_metrics'])) {
      $this->guestMetrics = $return;

      return;
    }

    try {
      $all_guest_metrics = $this->xsApi->VM_guest_metrics_get_all_records();
    }
    catch (Exception $e) {
      xs_log($e);
    }

    if (empty($all_guest_metrics[$vm['guest_metrics']])) {
      $this->guestMetrics = $return;

      return;
    }

    try {
      $vm_get_metrix_ref = $this->xsApi->VM_guest_metrics_get_by_uuid($all_guest_metrics[$vm['guest_metrics']]['uuid']);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    try {
      $return = $this->xsApi->VM_guest_metrics_get_record($vm_get_metrix_ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    $this->guestMetrics = $return;
  }

  /**
   * Gets VM Guest Metrics.
   *
   * @return array
   *   Guest Metrics of VM.
   */
  public function getGuestMetrics() {
    $this->setGuestMetrics();

    return $this->guestMetrics;
  }

  /**
   * Gets VIFs of a $vm via XAPI.
   *
   * @return array
   *   Array of XAPI VIFs arrays.
   */
  protected function setVifs() {
    $vm = $this->getData();

    $vifs = array();
    if (!empty($vm['VIFs'])) {
      foreach ($vm['VIFs'] as $vif_ref) {
        try {
          $vifs[] = $this->xsApi->VIF_get_record($vif_ref);
        }
        catch (Exception $e) {
          xs_log($e);
        }
      }
    }
    $this->vifs = $vifs;
  }

  /**
   * Gets VM VIFs.
   *
   * @return array
   *   Array of VIFs.
   */
  public function getVifs() {
    $this->setVifs();

    return $this->vifs;
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
  public function getVdiByVbd($vbd_ref, $type = 'Disk') {
    $vbd = FALSE;
    try {
      $vbd_type = $this->xsApi->VBD_get_type($vbd_ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    if ($vbd_type == $type) {
      try {
        $vdi_ref = $this->xsApi->VBD_get_VDI($vbd_ref);
      }
      catch (Exception $e) {
        xs_log($e);
      }

      try {
        $vbd = $this->xsApi->VDI_get_record($vdi_ref);
      }
      catch (Exception $e) {
        xs_log($e);
      }
    }

    return $vbd;
  }

  /**
   * Starts VM.
   */
  public function start() {
    set_time_limit(300);
    try {
      $this->xsApi->VM_start($this->ref, FALSE, FALSE);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Shutdowns VM in a clean way.
   */
  public function stop() {
    set_time_limit(300);
    try {
      $this->xsApi->VM_clean_shutdown($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Reboots VM in a clean way.
   */
  public function reboot() {
    set_time_limit(300);
    try {
      $this->xsApi->VM_clean_reboot($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Reboots VM in a forceful way.
   */
  public function rebootForce() {
    set_time_limit(60);
    try {
      $this->xsApi->VM_hard_reboot($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Shutdowns VM in a forceful way.
   */
  public function stopForce() {
    set_time_limit(60);
    $this->setRef();
    $power_status = $this->getStatus();
    if ($power_status != 'Running') {
      return FALSE;
    }

    try {
      $this->xsApi->VM_hard_shutdown($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    return TRUE;
  }

  protected function setStatus() {
    if (empty($this->data)) {
      $this->data = $this->getData();
    }

    $this->status = $this->data['power_state'];
  }

  /**
   * Gets power status of VM.
   *
   * @return string
   *   Power status string.
   */
  public function getStatus() {
    if (empty($this->status)) {
      $this->setStatus();
    }

    return $this->status;
  }

  /**
   * Sets VM VBDs.
   */
  public function setVbds() {
    try {
      $this->vbds = $this->xsApi->VBD_get_record($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Gets VBD references for VM.
   *
   * @return array
   *   VBDs array.
   */
  public function getVbds() {
    $this->setVbds();

    return $this->vbds;
  }

  /**
   * Load full snapshot object, given the snapshot reference.
   *
   * @return array
   *   Snapshot array.
   */
  protected function setSnapshots() {
    $this->getData();

    if (!empty($this->data['snapshots'])) {
      foreach ($this->data['snapshots'] as $snapshot_ref) {
        try {
          $full_snapshots[$snapshot_ref] = $this->xsApi->VM_get_record($snapshot_ref);
          $full_snapshots[$snapshot_ref] = new XsSnapshot($full_snapshots[$snapshot_ref]['uuid']);
        }
        catch (Exception $e) {
          xs_log($e);
        }
      }
    }

    $this->snapshots = $this->snapshotsReorder($full_snapshots);
  }

  public function getSnapshots() {
    $this->setSnapshots();

    return $this->snapshots;
  }
  /**
   * Reorder snapshots by snapshot date.
   *
   * @param array $snapshots
   *   Array of loaded snapshot objects.
   * @param bool $ascending
   *   Whether to sort ascending (TRUE).
   *
   * @return array
   *   Reordered snapshots.
   */
  protected function snapshotsReorder($snapshots, $ascending = FALSE) {
    if (empty($snapshots)) {
      return $snapshots;
    };

    $remapped_snapshots = array();
    $ordered_snapshots = array();
    foreach ($snapshots as $snap_ref => $snapshot) {
      $snapshot->ref = $snap_ref;
      $remapped_snapshots[$snapshot->data['snapshot_time']->timestamp] = $snapshot;
    }

    if ($ascending) {
      ksort($remapped_snapshots, SORT_STRING);
    }
    else {
      krsort($remapped_snapshots, SORT_STRING);
    }

    foreach ($remapped_snapshots as $snapshot) {
      $ordered_snapshots[$snapshot->ref] = $snapshot;
    }

    return $ordered_snapshots;
  }

  /**
   * Creates VM snapshot.
   *
   * @param string $title
   *   Snapshot title.
   */
  public function createSnapshot($title) {
    $this->setRef();

    try {
      $this->xsApi->VM_snapshot($this->ref, $title);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }
}
