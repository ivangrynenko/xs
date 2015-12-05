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

    <div class="col-md-4">

      <h3>Manage Automated Server Snapshot Policy</h3>


      <?php if ($policy_name) : ?>
        <p class="text-success">
          Your server is protected!
        </p>
        <div>
          <h5>Policy Details</h5>

          <p>
            <div>
              Name: <?php print $policy_name; ?>
            </div>
            <div>
              Type: <?php print $policy_type; ?>
            </div>
            <div>
              Snapshots limit: <?php print $policy_snapshots; ?>
            </div>
          <div>
              Last run:
              <?php if ($policy_last_run) : ?>
              <?php print $policy_last_run; ?>
              <?php else: ?>
              This policy never run
              <?php endif; ?>
            </div>
          </p>
        </div>
      <?php else: ?>
        <p class="text-danger">Your server is not protected by an automated snapshots policy!</p>
      <?php endif; ?>

      <p>
        <button type="button" class="btn btn-success" data-toggle="modal" data-target="#snapshot_policy_form">
          <span class="glyphicon glyphicon-time" aria-hidden="true"></span>&nbsp;
          <?php print $policy_name ? t('Edit Policy') : t('Create Policy'); ?>
        </button>
      </p>
      <?php print $policy_form; ?>

    </div>

    <div class="col-md-8">
      <h5>Recent Policy Events</h5>
      <?php print $policy_log_table; ?>
    </div>

    <div class="col-md-4">
      <div>
        <p>
          <small class="text-muted">
            The automated snapshots policy helps you to set your server protection
            policy. Once you create a policy, a snapshot of your server will be
            created accordingly.
          </small>
        </p>

        <p>
          <small class="text-warning">
            This policy will never remove manually created snapshots. Only snapshots
            created by this policy will be rotated. Here is how this works. Let's say
            you create a policy for maximum of 20 snapshots, your server set to the
            maximum of 25 snapshots and you have created 15 manual snapshots. In this
            example, the policy you create will be able to create and rotate 5 snapshots
            automatically.
          </small>
        </p>
      </div>
    </div>

  </div>
</section>
