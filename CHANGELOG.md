# CHANGELOG #
xs 7.x-1.6, 2016-01-28
----------------------
[] ig: Refactored XenApi implementation to facilitate migration to Drupal 8.
[] ig: Migrated all templates to implement Bootstrap 3.
[] ig: Removed dependencies on other modules.
[] ig: Major update to the user interface.

xs 7.x-1.5, 2016-01-08
----------------------
[] ig: Added xs_rrd submodule to display VM's performance graphs.
[] ig: Updated views integration to provide mapping to custom entities.
[] ig: Added administrative views to display action log and snapshot log.

xs 7.x-1.4, 2015-12-13
----------------------
[] ig: Added xs_action_log entity.
[] ig: Added view for user to access server actions log.
[] ig: Added view for user to access snapshot actions log.
[] ig: Added view for admin to access server all actions log.
[] ig: Added view for admin to access snapshot all actions log.
[] ig: Updated policy log to assert node access permissions.

xs 7.x-1.3, 2015-12-12
----------------------
[] ig: Replaced policy events log with a view. 
[] ig: Added date handlers for policy and policy log date fields. 
[] ig: Cleaned up log types.
[] ig: Added administrative view to give overview of all policy log entries.

xs 7.x-1.1, 2015-12-08
----------------------
[] ig: Template improvements
[] ig: Added bypass to the server snapshot by an automated policy when the VM is down.
[] ig: Fixed number of tests.
[] ig: Added missing permission, that was preventing administrator from accessing
       snapshot policy pages.
[] ig: Updated snapshot policy snapshots selector to be server max less 5.
[] ig: Fixed Snapshots page layout bug in mobile devices preventing snapshot actions.
[] ig: Other bug fixes and improvements.

xs 7.x-1.0, 2015-12-05
----------------------
[] ig: Automated snapshot policies as entities.
[] ig: Entity Policy type.
[] ig: Entity Policy log.
[] ig: One policy per server.
[] ig: Ability to store policy events into designated log.
[] ig: Automated cron job that manages snapshot creation and removal.

xs 7.x-1.x-RC2, 2015-11-12
--------------------------
[] ig: VM content type available.
[] ig: Ability to allocate VM to Drupal users and add Dom-U uuid.
[] ig: Ability to create manual snapshots.
