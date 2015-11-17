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
    <?php if (empty($snapshots_max)) : ?>
      <div class="col-md-12">
        <h3>Snapshots disabled</h3>

        <p>
          No snapshots could be created for this server.
        </p>
      </div>
    <?php else: ?>
      <div class="col-md-12">
        <h3>Available snapshots</h3>

        <p>You have available <?php print $snapshots_qty; ?> out of the maximum <?php print $snapshots_max; ?></p>

        <p><?php print $link; ?></p>

      </div>
    <div class="col-md-8">
      <?php print $snapshots_table; ?>
      </div>
    <div class="col-md-4">
      <?php print $snapshots_actions_form; ?>
    </div>
    <?php endif; ?>
  </div>
</section>
