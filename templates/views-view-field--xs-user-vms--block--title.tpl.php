<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
?>
<i class="fa fa-link"></i>
<?php print l($row->node_title, 'node/' . $row->nid); ?>

<?php
$vm_uuid = xs_get_vm_uuid_by_node($row->nid);
try {
  $xs_vm = new XsVm($vm_uuid);
}
catch (Exception $e) {
  xs_log($e);

  return;
}

if (empty($xs_vm)) {
  return;
}
$vm = $xs_vm->getData();

$disk_size = $xs_vm->getDiskSpace($vm['VBDs']);
$disk_size = $disk_size / 1024 / 1024 / 1024;

$vm_metrics = $xs_vm->getMetrics();

$vm_cpus = $vm_metrics['VCPUs_number'];

// Memory in GB.
$memory = $vm_metrics['memory_actual'] / 1024 / 1024 / 1024;
$memory = $memory > 1 ? (int) ceil($memory) : $memory;

if ($xs_vm->getStatus() == 'Running') {
  $time_delta = REQUEST_TIME - $vm_metrics['start_time']->timestamp;
  $server_running_time = xs_seconds2human($time_delta);
  $server_uptime = $server_running_time['days'] . ' days, ' . $server_running_time['hours'] . ' hours, ' . $server_running_time['minutes'] . ' minutes';
}

?>
<div class="text-muted">
  <span>
    <?php print format_plural($vm_cpus, '@count CPU', '@count CPUs', array('@count' => $vm_cpus)); ?>
  </span> /
  <span>
    <?php print $memory; ?> GB RAM
  </span> /
  <span>
    <?php print $disk_size; ?> GB HDD
  </span>
</div>
<?php if (!empty($server_uptime)): ?>
  <div class="text-success">
    Up <?php print $server_uptime; ?>
    <i data-toggle="tooltip" title="Started <?php print date("d M Y h:iA", $vm_metrics['start_time']->timestamp); ?>" class="fa fa-info"></i>
  </div>
<?php endif; ?>


