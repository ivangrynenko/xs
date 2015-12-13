<?php
/**
 * @file
 * xs_action_log entity classes.
 */

/**
 * Class XsActionLogController
 */
class XsActionLogController extends EntityAPIController {
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'target_id' => arg(1),
      'log' => '',
      'type' => '',
      'uid' => $user->uid,
      'created' => REQUEST_TIME,
    );

    return parent::create($values);
  }
}

/**
 * XsActionLog class.
 */
class XsActionLog extends Entity {
  public $id;
  public $target_id;
  public $uid;
  public $log;
  public $type;
  public $created;

  public function __construct($values = array()) {
    parent::__construct($values, 'xs_action_log');
  }
}
