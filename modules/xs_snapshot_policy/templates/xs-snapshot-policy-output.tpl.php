<?php
/**
 * @file
 * xs-snapshot-policy-output.tpl.php
 */

/**
 * Template file to display automated snapshot policy and history.
 */
?>
<section class="node node-snapshots">
  <div class="row">

    <div class="col-md-12">

      <h3>Manage Automated Server Snapshots</h3>

      <p>Your server is limited to have
        <strong><?php print $number_of_policies; ?></strong></p>
      <?php if ($policies_exceeded) : ?>
        <p>You have reached your policies limit. You can edit or delete your current policy.</p>
      <?php else: ?>
        <p>
          <button type="button" class="btn btn-success" data-toggle="modal" data-target="#new_snapshot_policy_form">
            <span class="glyphicon glyphicon-time" aria-hidden="true"></span>&nbsp;
            <?php print t('Create Snapshot Policy'); ?>
          </button>
        </p>
        <?php print $new_snapshot_policy_form; ?>
      <?php endif; ?>

    </div>

    <div class="col-md-12">
      <?php print $snapshots_table; ?>
    </div>

    <div class="col-md-12">
      <div class="help">
        <p>
          The automated snapshots policy helps you to set your server protection
          policy. Once you create a policy, a snapshot of your server will be
          created accordingly.
        </p>

        <p class="note">
          This policy will never remove manually created snapshots. Only snapshots
          created by this policy will be rotated. Here is how this works. Let's say
          you create a policy for maximum of 20 snapshots, your server set to the
          maximum of 25 snapshots and you have created 15 manual snapshots. In this
          example, the policy you create will be able to create and rotate 5 snapshots
          automatically.
        </p>
      </div>
    </div>

  </div>
</section>
