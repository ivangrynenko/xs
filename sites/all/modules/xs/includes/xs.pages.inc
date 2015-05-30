<?php
/**
 * @file
 * xs.pages.inc
 */

// @todo: Replace relevant content types with custom entities.

/**
 * Administrative for for default tab.
 */
function xs_settings() {
  $output['header']['#markup'] = l('Add XS pool', 'node/add/' . variable_get('xs_pool_content_type'), array(
    'query' => array('destination' => 'admin/config/services/xs'),
    'attributes' => array('class' => 'more'),
  ));
  $output['header']['#markup'] .= '<p>Available XenServer pools</p>';

  // Run pre-checks and display messages if pre-checks failed.
  $types = node_type_get_types();
  $xs_pools = xs_get_pool_nodes();
  if (empty($types[variable_get('xs_pool_content_type')])) {
    drupal_set_message(t('No XenServer Pool entity type is configured. Refer tot he README file'), 'error');
  }

  if (empty($xs_pools)) {
    drupal_set_messag(t('You have no XS pools available. !create at least one XL pool to continue', array('!create' => l('Create', 'node/add/' . variable_get('xs_pool_content_type')))), 'error');
  }
  else {
    foreach ($xs_pools as $pool) {
      $output['content']['#markup'] .= $pool->title . '<br />';
    }
  }

  //xs_connect();

  return $output;
}

/**
 * Gets content for pool configuration page.
 *
 * Menu callback for 'admin/config/services/xs/settings'.
 */
function xs_admin_pool_configuration() {
  return xs_settings();
}

function xs_settings_form($form, $form_state) {
  $form = system_settings_form($form);

  $form['xs_pool_is'] = array(
    '#type' => 'textarea',
    '#title' => 'XenServer IP addresses.',
    '#descripton' => t('List one or more IP addresses for every server in your XenServer pool, separated by comma'),
    '#default_value' => variable_get('xs_pool_is', ''),
  );

  array_unshift($form['#submit'], 'xs_settings_form_submit');

  return $form;
}

function xs_settings_form_submit($form, &$form_state) {
  $form_state['values']['xs_pool_is'] = filter_xss($form_state['values']['xs_pool_is']);
  variable_set('xs_pool_is', $form_state['values']['xs_pool_is']);
}

function xs_admin_pool_vms() {
  drupal_set_title('Available Virtual Servers');
  $view = views_get_view('xs_pools_servers');
  $output = $view->preview('block');

  return $output;
}

function xs_admin_pool_templates() {
  drupal_set_title('Available templates');
  $pools = xs_get_pool_nodes();
  foreach ($pools as $pool) {
    $servers = xs_get_pool_servers($pool);
//    $pool_master = xs_api_get_pool_master($servers);
    foreach ($servers as $server) {
      $templates = xs_get_all_templates($server);
      foreach ($templates as $template) {
        $template_names[] = $template['name_label'];
      }
      $content[] = array($pool->name, $template_names);
    }
    $header = array('Pool', 'Template name');
    $output[] = theme('table', array('header' => $header, 'rows' => $content));
  }

  return $output;
}

/**
 * Loads all configured xenserver pools.
 *
 * @return mixed
 *   Array of loaded entities or FALSE.
 */
function xs_get_pool_nodes() {
  return entity_load('node', FALSE, array('type' => variable_get('xs_pool_content_type')));
}

/**
 * Loads all servers in the $pool.
 *
 * @return mixed
 *   Array of loaded entities or emty array.
 */
function xs_get_pool_servers($pool) {
  $server = array();
  if (!empty($pool->field_xs_pool_member[LANGUAGE_NONE][0]['target_id'])) {
    foreach ($pool->field_xs_pool_member[LANGUAGE_NONE] as $server_target) {
      $server[$server_target['target_id']] = reset(entity_load('node', array($server_target['target_id'])));
    }
  }

  return $server;
}

/**
 * Getter for XenServer IP address.
 *
 * Loads IP address stored in field_xs_ip_address field.
 *
 * @param object $server
 *   Drupal node object.
 *
 * @return mixed
 *   IP address as a string or FALSE.
 */
function xs_get_server_ip($server) {
  if (!empty($server->field_xs_ip_address[LANGUAGE_NONE][0]['value'])) {
    return $server->field_xs_ip_address[LANGUAGE_NONE][0]['value'];
  }

  $entity = entity_metadata_wrapper('node', $server);

  return $entity->field_xs_ip_address->value();
}

/**
 * Getter for XenServer password address.
 *
 * Loads and decrypts password, stored in field_xs_password field.
 *
 * @param object $server
 *   Drupal node object.
 *
 * @return mixed
 *   Unencrypted password as a string or FALSE.
 */
function xs_get_server_password($server) {
  if (!empty($server->field_xs_password[LANGUAGE_NONE][0]['password_field'])) {
    return password_field_decrypt($server->field_xs_password[LANGUAGE_NONE][0]['password_field']);
  }

  $entity = entity_metadata_wrapper('node', $server);

  return $entity->field_xs_password->password_field();
}

/**
 * Getter for XenServer username address.
 *
 * Loads and decrypts username, stored in field_xs_username field.
 *
 * @param object $server
 *   Drupal node object.
 *
 * @return mixed
 *   Unencrypted username as a string or FALSE.
 */
function xs_get_server_username($server) {
  if (!empty($server->field_xs_username[LANGUAGE_NONE][0]['password_field'])) {
    return password_field_decrypt($server->field_xs_username[LANGUAGE_NONE][0]['password_field']);
  }

  $entity = entity_metadata_wrapper('node', $server);

  return $entity->field_xs_username->password_field();
}
