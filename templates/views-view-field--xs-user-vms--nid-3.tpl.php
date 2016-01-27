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
<?php
$vm_uuid = xs_get_vm_uuid_by_node($row->nid);
try {
  $xs_vm = new XsVm($vm_uuid);
}
catch (Exception $e) {
  xs_log($e);
}

if (!empty($xs_vm)) {
  $vm = $xs_vm->getData();
}

$allowed_operations = !empty($vm['allowed_operations']) ? $vm['allowed_operations'] : array('start');

module_load_include('inc', 'xs', 'includes/xs.actions');
$links = xs_actions_get_links($row->nid, $allowed_operations, $_GET['q']);
$action_links_dropdown = theme('xs_links_dropdown', array(
  'links' => $links,
  'label' => t('Server Actions')
));
?>
<?php print $action_links_dropdown; ?>
