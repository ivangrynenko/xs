<?php

/**
 * @file
 * XsSnapshot.php
 */
class XsSnapshot extends XsApi {
  public $uuid;
  public $ref;
  public $data;
  public $vbds;

  public function __construct($uuid) {
    $this->setUuid($uuid);
    parent::__construct();
    $this->data = $this->getSnapshot();
  }

  /**
   * Get VM by UUID, stored against $node.
   *
   * @return bool|mixed
   */
  public function getSnapshot() {
    $snapshot = &drupal_static(__FUNCTION__ . '-' . $this->uuid);
    if (!empty($snapshot)) {
      $this->data = $snapshot;

      return $snapshot;
    }

    $this->setRef();

    try {
      $snapshot = $this->xsApi->VM_get_record($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    $this->data = $snapshot;

    return $this->clear($snapshot);
  }

  /**
   * Sets snapshot reference.
   */
  protected function setRef() {
    if (!empty($this->ref)) {
      return;
    }

    try {
      $this->ref = $this->clear($this->xsApi->VM_get_by_uuid($this->uuid));
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Sets snapshot UUID.
   *
   * @param string $uuid
   *   UUID of the snapshot.
   */
  protected function setUuid($uuid) {
    $this->uuid = $uuid;
  }

  /**
   * Destroys a snapshot.
   */
  public function destroy() {
    $this->setRef();
    $this->destroyVbds();
    try {
      $this->xsApi->VM_destroy($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Revert VM to the selected snapshot.
   */
  public function revert() {
    $this->setRef();

    try {
      $this->xsApi->VM_revert($this->ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }
  }

  /**
   * Gets VBD by VBD reference.
   *
   * @param string $vbd_ref
   *   VBD reference.
   *
   * @return array
   *   VBD array.
   */
  public function getVbd($vbd_ref) {
    try {
      $vbd = $this->xsApi->VBD_get_record($vbd_ref);
    }
    catch (Exception $e) {
      xs_log($e);
    }

    return $this->clear($vbd);
  }

  /**
   * Destroys VBDs for the $snapshot.
   */
  function destroyVbds() {
    if (empty($this->data['VBDs'])) {
      return;
    }

    foreach ($this->data['VBDs'] as $vbd_ref) {
      $vbd = $this->getVbd($vbd_ref);
      if ($vbd['VDI'] != XS_API_EMPTY_REF) {
        $this->xsApi->VDI_destroy($vbd['VDI']);
      }
    }
  }
}
