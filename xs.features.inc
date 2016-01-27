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
  return array(
    "api" => "3.0",
    'path' => drupal_get_path('module', 'xs'),
    'template path' => drupal_get_path('module', 'xs') . '/templates',
  );
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
