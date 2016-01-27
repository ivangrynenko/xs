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
<div class="btn-group btn-group-xs">
  <button aria-expanded="false" aria-haspopup="true" data-toggle="dropdown" class="btn btn-success dropdown-toggle" type="button">
    <i class="fa fa-link"></i> Links
    <span class="caret"></span>
  </button>
  <ul aria-labelledby="dLabel" class="dropdown-menu">
    <li>
      <div class="dropdown-links"><i class="fa fa-server"></i>
        <a class="xs-glyphicon-link" href="/node/<?php print $field->original_value; ?>">Overview</a>
      </div>
    </li>

    <li class="divider" role="separator"></li>

    <li>
      <div class="dropdown-links"><i class="fa fa-history"></i>
        <a class="xs-glyphicon-link" href="/node/<?php print $field->original_value; ?>/xs-snapshots">Snapshots</a>
      </div>
    </li>

    <li class="divider" role="separator"></li>

    <li>
      <div class="dropdown-links">
        <i class="fa fa-shield"></i>
        <a class="xs-glyphicon-link" href="/node/<?php print $field->original_value; ?>/xs-snapshot-policy">Snapshot Policy</a>
      </div>
    </li>

    <li class="divider" role="separator"></li>

    <li>
      <div class="dropdown-links"><i class="fa fa-area-chart"></i>
        <a class="xs-glyphicon-link" href="/node/<?php print $field->original_value; ?>/xs-performance">Graphs</a>
      </div>
    </li>
  </ul>
</div>
