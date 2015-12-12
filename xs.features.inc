<?php
/**
 * @file
 * xs.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function xs_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function xs_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function xs_node_info() {
  $items = array(
    'xs_vps' => array(
      'name' => t('XS VPS'),
      'base' => 'node_content',
      'description' => t('Xenserver VPS'),
      'has_title' => '1',
      'title_label' => t('VM name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_themekey_features_rule_chain().
 */
function xs_themekey_features_rule_chain() {
if (!defined('THEMEKEY_PAGECACHE_UNSUPPORTED')) {
    define('THEMEKEY_PAGECACHE_UNSUPPORTED', 0);
    define('THEMEKEY_PAGECACHE_SUPPORTED', 1);
    define('THEMEKEY_PAGECACHE_TIMEBASED', 2);
  }
$rules = array(
  0 => array(
    'rule' => array(
      'property' => 'user:role',
      'operator' => '=',
      'value' => 'XS VPS Customer',
      'theme' => 'bootstrap',
      'enabled' => 1,
      'wildcards' => 'a:0:{}',
      'module' => 'xs',
    ),
    'string' => '"user:role = XS VPS Customer >>> bootstrap"',
    'childs' => array(
      0 => array(
        'rule' => array(
          'property' => 'drupal:get_q',
          'operator' => '*',
          'value' => 'xs-snapshots',
          'theme' => 'bootstrap',
          'enabled' => 1,
          'wildcards' => 'a:0:{}',
          'module' => 'xs',
        ),
        'string' => '"drupal:get_q * xs-snapshots >>> bootstrap"',
        'childs' => array(),
      ),
      1 => array(
        'rule' => array(
          'property' => 'drupal:get_q',
          'operator' => '*',
          'value' => 'xs-snapshot-policy',
          'theme' => 'bootstrap',
          'enabled' => 1,
          'wildcards' => 'a:0:{}',
          'module' => 'xs',
        ),
        'string' => '"drupal:get_q * xs-snapshot-policy >>> bootstrap"',
        'childs' => array(),
      ),
      2 => array(
        'rule' => array(
          'property' => 'drupal:path',
          'operator' => '=',
          'value' => 'user/vps',
          'theme' => 'bootstrap',
          'enabled' => 1,
          'wildcards' => array(),
          'module' => 'xs',
        ),
        'string' => '"drupal:path = user/vps >>> bootstrap"',
        'childs' => array(),
      ),
      3 => array(
        'rule' => array(
          'property' => 'node:type',
          'operator' => '=',
          'value' => 'xs_vps',
          'theme' => 'bootstrap',
          'enabled' => 1,
          'wildcards' => 'a:0:{}',
          'module' => 'xs',
        ),
        'string' => '"node:type = xs_vps >>> bootstrap"',
        'childs' => array(),
      ),
    ),
  ),
);

return $rules;
}
