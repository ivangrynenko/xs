# README #

### Summary ###

Drupal module that integrates Drupal with XenServer pool of servers to allow client access to the VMs hosted in XenServer environment.
This module is for hosting companies that use XenServer for virtualisation and need to give their customers access to perform power actions on their virtual servers, as well as manage server snapshots.

Aims to provide customer access to manage their VPS via Drupal.

Using REST/Curl with XML-RPC Xen API to access VPS status and configuration options.

### Features ###

- Provides custom content type for XenServer DOMu/VPS
- Allows site administrator to manage XenServer guests
- Provides customer management page to browse VM configuration and power state (Running or Down)
- Provides VPS management page to power on/off VM, restart VM, force-restart VM
- Provides snapshots management page to browse available snapshots, revert VPS status to any snapshot available, destroy any existing snapshot
- Ability to create snapshot policy, allowing customers to select automated snapshots frequency and retention policy

## Planned features ##

### Currently in development ###

* Jave web-based console
* Next on the roadmap

### To be implemented in 2016 ###

* VPS power events history. Overview of VPS power events (time and date of all of the following: reboot, power off, power on, force shutdown, force reboot
* VPS traffic/bandwidth usage for selected time period
* Stats for the VPS resource usage: CPU, RAM, HDD, Network. Stats available as graphs with date range selector
* Ability to template VPS, allowing to easily revert to the initial state

Full feature set is coming. Planned public release by the end of 2015.

The development of this module is sponsored by https://www.redy.host If you are interested to sponsor the development or become a co-maintainer of this project (to get it's release quicker), please contact us.

### How do I get set up? ###

This is a Drupal module and should be installed as any Drupal module would.
1. Before enabling the module, you need to define your XenServer pools and servers. To do so, add the code to your Drupal's settings.php file:

```
#!php

$conf['xs_pools'] = array(
  '283e03e3-226e-3e58-cd35-d8dcf1359a16' => array(
    'name' => 'Test Pool',
    'members' => array(
      '9a14a77a-d75a-4f47-8a56-e0e00810102a' => array(
        'name' => 'XS--1',
        'ip' => '10.1.1.2',
        'uuid' => '9a14a77a-d75a-4f47-8a56-e0e00810102a',
        'username' => 'root',
        'password' => '{root_password}',
      ),
    ),
  ),
);
```
2. After adding the pools definition to the settings.php file, enable the module. This will create 'XS VPS' content type and a user role 'XS VPS Customer' to work with it. 
3. Setup module permissions to give permissions to work with 'XS VPS' to the new role created by this module.
4. Create at least one user in 'XS VPS Customer' role
5. Create at least one node of 'XS VPS', give it pool UUID and VPS UUID. Link it to the user created in the p. above.
6. Login as the user and navigate to the node URL created above. Additionally, you may add menu link to 'user/vps', which will list all VPS for the user.

For support, submit your requests to https://www.drupal.org/project/issues/2484429
 No newline at end of file