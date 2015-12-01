<?php

/**
 * xs_snapshot_policy1.class.php
 */
class XsSnapshotPolicy {
  public $name;
  public $uid;
  public $target_id;
  public $created;
  public $changed;
  public $type;
  public $type_id;
  public $status;

  protected $id;

  const POLICIES_LIMIT = XS_SNAPSHOT_MAX_POLICIES_PER_USER;
  const MAX_SNAPSHOTS = XS_API_SNAPSHOTS_MAX;

  public function __construct($uid, $id = NULL, $max_snapshots = self::MAX_SNAPSHOTS) {
    $this->uid = $uid;
    if ($id) {
      $this->id = $id;
    }
  }

  public function load() {
    $query = db_select('xs_snapshot_policy1', 'p');
    $query->join('xs_snapshot_policy_id_type_id', 't', 'p.id = t.id');
    $query->join('xs_snapshot_policy_uid_id', 'u', 'u.id = p.id');
    $query->join('xs_snapshot_policy_type', 'type', 'type.type_id = t.type_id');
    $query->fields('p', array('id', 'target_id', 'name', 'status', 'created', 'changed'));
    $query->fields('t', array('type_id'));
    $query->fields('u', array('uid'));
    $query->fields('type', array('type'));
    if ($this->id) {
      $query->condition('p.id', $this->id, '=');
    }
    $query->condition('u.uid', $this->uid, '=');
    $result = $query->execute();

    // @todo: Needs debugging.
    while ($row = $result->fetchAssoc()) {
      foreach ($row as $field_name => $value) {
        if (property_exists('XsSnapshotPolicy', $field_name)) {
          $this->{$field_name} = $value;
        }
      }
    }
  }

  public function save() {
    if ($this->insertValidate()) {
      $this->insert();
    }
    else {
      $this->update();
    }
  }

  protected function insert() {
    $this->created = time();
    $this->changed = time();

    $query = db_insert('xs_snapshot_policy1', array(
      'target_id' => $this->target_id,
      'name' => $this->name,
      'status' => $this->status,
      'created' => $this->created,
      'changed' => $this->changed,
    ));
    $insert = $query->execute();

    // @todo: get last insert ID (policy ID).
    $query = db_insert('xs_snapshot_policy_id_type_id', array(
      'id' => $this->id,
      'type_id' => $this->type_id,
    ));
    $query->execute();

    $query = db_insert('xs_snapshot_policy_uid_id', array(
      'id' => $this->id,
      'uid' => $this->uid,
    ));
  }

  public function getTypes($type_id = NULL) {
    $query = db_select('xs_snapshot_policy_type', 't');
    $query->fields('t', array('type_id', 'type'));
    if ($type_id) {
      $query->condition('type_id', $type_id, '=');
    }
    $result = $query->execute();

    $type = array();
    while ($row = $result->fetchAssoc()) {
      $type[$row['type_id']] = $row['type'];
    }

    return $type;
  }

  protected function update() {
    $this->changed = time();

    $query = db_update('xs_snapshot_policy1');
    $query->fields(array(
      'target_id' => $this->target_id,
      'name' => $this->name,
      'status' => $this->status,
      'changed' => $this->changed,
    ));
    $query->condition('id', $this->id, '=');
    $result = $query->execute();

    $query = db_update('xs_snapshot_policy_id_type_id');
    $query->fields(array(
      'type_id' => $this->type_id,
    ));
    $query->condition('id', $this->id, '=');
    $result = $query->execute();

    $query = db_update('xs_snapshot_policy_uid_id', array(
      'uid' => $this->uid,
    ));
    $query->condition('id', $this->id, '=');
    $result = $query->execute();
  }

  /**
   * Validates if it is okay to insert a policy for the user.
   */
  protected function insertValidate() {
    // @todo: Add validation of all required fields, using
    // xs_snapshot_policy_schema().
    $all_policies = $this->load();
    if (count($all_policies) >= self::POLICIES_LIMIT) {
      return FALSE;
    }

    return TRUE;
  }
}
