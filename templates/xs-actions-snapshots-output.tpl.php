<?php
/**
 * @file
 * xs-actions-snapshots-output.tpl.php
 */

/**
 * Template file to display available server snapshots and actions form.
 *
 * @var int $snapshots_max
 *   Limit of snapshots for this server.
 * @var string $snapshots_qty
 *   Current number of snapshots existing for the server.
 * @var string $link
 *   Link to create new snapshot.
 * @var string $snapshots_table
 *   Table displaying current snapshots.
 * @var string $snapshots_actions_form
 *   Snapshots actions form.
 */
?>
<section class="node node-snapshots">
  <div class="row">

    <?php if ($error_message) : ?>
      <div class="col-md-12">
        <?php print $error_message; ?>
      </div>


    <?php else: ?>

      <?php if (empty($snapshots_max)) : ?>
        <div class="col-md-12">
          <h3>Snapshots disabled</h3>

          <p>
            No snapshots could be created for this server.
          </p>
        </div>
      <?php else: ?>
        <div class="col-md-12">

          <h3>Available server snapshots</h3>

          <p>You have <?php print $snapshots_qty; ?> snapshots available. Your server is limited to have the maximum <?php print $snapshots_max; ?> snapshots.</p>

          <h2>
            <button type="button" class="btn <?php print $link_class; ?>" data-toggle="modal" data-target="#myModal">
              <span class="glyphicon <?php print $link_glyphicon_class; ?>" aria-hidden="true"></span>&nbsp;
              <?php print t('Create a Snapshot'); ?>
            </button>
          </h2>
          <?php print $new_snapshot_form; ?>

        </div>

        <div class="col-md-12">
          <p>
            Showing the most resent snapshot at the top of the table. Snapshots are ordered by created date in descending order.
          </p>
          <?php print $snapshots_table; ?>
        </div>

        <div class="col-md-12">
          <small class="text-muted">
            <p>
              Snapshots are the fastest way to backup and revert your entire server.
              Whether you need to revert last configuration changes or had an unsuccessful
              deployment, just select the last snapshot you created and select
              'Revert' link from the Actions column.
            </p>
            <p>
              Before attempting to change server configuration, navigate to this
              page and click the <?php print t('Create a Snapshot'); ?> button.
              Just give it a title and click the <?php print XS_API_SNAPSHOT_CREATE_BUTTON_TITLE; ?> button.
            </p>
            <p>
              Snapshot Actions help: To destroy a snapshot, select 'Trash' in the Actions column.<br />
              Destroying a snapshot has no impact on your server.<br />
              To revert your Server to the selected snapshot, select 'Revert'.<br />
              Reverting a snapshot will cause your server to be shut down, it's
              disks to be reverted to the date and time of the snapshot creation.
              The server will start automatically.</p>
            <p class="note">
              Note! To avoid harmful actions to your server, a confirmation
              dialogue will appear after you select an action.
            </p>
          </small>
        </div>

      <?php endif; ?>
    <?php endif; ?>
  </div>
</section>
