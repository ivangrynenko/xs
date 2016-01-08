<?php
/**
 * @file xs-rrd-output.tpl.php
 * Template file to output performance graphs.
 */
?>
<div class="col-md-12">
  <?php if (!empty($graph['cpu'])): ?>
    <?php foreach ($graph['cpu'] as $image): ?>
      <div class="col-md-6">
        <h3>CPU performance</h3>
        <img src="<?php print $image; ?>"/>
      </div>
    <?php endforeach; ?>
  <?php endif; ?>

  <?php if (!empty($graph['ram'])): ?>
    <?php foreach ($graph['ram'] as $image): ?>
      <div class="col-md-6">
        <h3>Memory usage</h3>
        <img src="<?php print $image; ?>"/>
      </div>
    <?php endforeach; ?>
  <?php endif; ?>
</div>

<div class="col-md-12">
  <?php if (!empty($graph['network'])): ?>
    <?php foreach ($graph['network'] as $image): ?>
      <div class="col-md-6">
        <h3>Network usage</h3>
        <img src="<?php print $image; ?>"/>
      </div>
    <?php endforeach; ?>
  <?php endif; ?>

  <?php if (!empty($graph['disk_rw'])): ?>
    <?php foreach ($graph['disk_rw'] as $image): ?>
      <div class="col-md-6">
        <h3>Disk Read/Write usage</h3>
        <img src="<?php print $image; ?>"/>
      </div>
    <?php endforeach; ?>
  <?php endif; ?>
</div>

<div class="col-md-12">
  <p class="bg-warning text-muted">
    Performance data updated <?php print $last_updated; ?> ago.<br />
    <?php print $ttl_message; ?>
  </p>
</div>
