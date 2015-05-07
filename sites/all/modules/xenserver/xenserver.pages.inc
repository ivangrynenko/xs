<?php
/**
 * @file
 * xenserver.pages.inc
 */

/**
 * Administrative for for default tab.
 */
function xenserver_settings() {
  if (user_access('administer xenserver pool')) {
    $output['add_account']['form'] = drupal_get_form('xenserver_settings_form');
  }
}

function xenserver_settings_form($form, $form_state) {

}
