1. Install the module as usual Drupal module.
2. Adjust permissions to Peform actions with own VM and any VM as you require.
3. Add the following code to the bottom of you settings.php file. This code
   contains pool ID and is not recommended to go to version control.
   Adjust this code as you require, with correct UUIDs, IP addresses and
   username/password to manage your Xenserver pool.

   Code:
   $conf['xs_pools'] = array(
     '{add-uuid-of-your-pool-and-not-server}' => array(
       'name' => '{add-Name-to-your-pool}',
       'members' => array(
         '{add-uuid-of-your-server}' => array(
           'name' => '{add-name-your-server}',
           'ip' => {'add-ip-address'},
           'uuid' => {add-uuid-of-your-server-duplicate-of-above},
           'username' => '{add-username-typically-root}',
           'password' => '{add-user-password-to-access-xapi}',
         ),
       ),
     ),
   );

   Example config:
   <?php
   $conf['xs_pools'] = array(
     '283e04ee-226e-3e58-cd35-d9dcf1359b16' => array(
       'name' => 'Test Pool',
       'members' => array(
         '8c14a77a-d75a-4f47-8a59-e0e00810302a' => array(
           'name' => 'XS--1',
           'ip' => '10.1.1.24',
           'uuid' => '8c14a77a-d75a-4f47-8a59-e0e00810302a',
           'username' => 'root',
           'password' => 'password',
         ),
       ),
     ),
   );
