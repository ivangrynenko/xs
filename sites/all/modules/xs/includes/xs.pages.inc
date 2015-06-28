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
  global $xs_api;

  $xs_api = xs_get_xs_api();

  $output = array();
  $output['header']['#markup'] .= '<p>Available XenServer pools</p>';

  // Run pre-checks and display messages if pre-checks failed.
  $xs_pools = xs_get_pools();

  $header = array(
    t('#'),
    t('SERVER'),
    t('IP-ADDRESS'),
    t('POOL'),
    t('SERVER-UUID'),
    t('STATUS'),
    t('VMs')
  );
  $i = 0;
  foreach ($xs_pools as $pool) {
    // Get servers of the pools.
    $xs_pool_servers = xs_get_pool_servers($pool);
    if (empty($xs_pool_servers)) {
      drupal_set_message(t('No XS pool servers are configured. Check the module README file for the details.', 'error'));
      return '';
    }

    foreach ($xs_pool_servers as $xs_pool_server) {
      // Connect to this server and check it's status.
      $xs_host = xs_get_host_by_uuid($xs_api, $xs_pool_server['uuid']);
      $status = xs_check_server_status($xs_host);

      $vms = xs_get_all_vms($xs_api, $xs_host);
      if (!empty($vms)) {
        foreach ($vms as $vm) {
          $vms_list[] = format_string('!name [!status]', array(
            '!name' => $vm['name_label'],
            '!status' => $vm['power_state'],
          ));
        }
      }

      $rows[] = array(
        $i,
        $xs_pool_server['name'],
        $xs_pool_server['ip'],
        $pool['name'],
        $xs_pool_server['uuid'],
        $server_status = $status ? t('Up') : t('Off'),
        implode("<br />", $vms_list),
      );
      $i++;
    }
  }
  $output['content']['#markup'] .= theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));
//  xs_connect();

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
  global $xs_api;

  $xs_api = xs_get_xs_api();

  drupal_set_title('Available Virtual Servers');
  $view = views_get_view('xs_pools_servers');
  $output = $view->preview('block');

  return $output;
}

function xs_admin_pool_templates() {
  global $xs_api;

  drupal_set_title('Available templates');
  $pools = xs_get_pools();
  foreach ($pools as $pool) {
    $servers = xs_get_pool_servers($pool);
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
