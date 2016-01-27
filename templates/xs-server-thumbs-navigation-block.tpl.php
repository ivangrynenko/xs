<?php
/**
 * @file
 * xs-server-thumbs-navigation-block.tpl.php
 */
?>
<nav class="navbar navbar-default server-icons-navigation">
  <div class="container-fluid">
    <div class="navbar-header">

    <span class="server-icon server-icon-view">
      <span class="icon">
        <a href="<?php print $link_href_overview; ?>" class="<?php print $link_class_active_overview; ?>">
          <i class="fa fa-tasks fa-4x"></i> <span class="text">Overview</span>
        </a>
      </span>
    </span>

      <?php if (!empty($link_href_graphs)): ?>
        <span class="server-icon server-icon-graphs">
      <span class="icon">
        <a href="<?php print $link_href_graphs; ?>" class="<?php print $link_class_active_graphs; ?>">
          <i class="fa fa-area-chart fa-4x"></i> Graphs
        </a>
      </span>
    </span>
      <?php endif; ?>

      <span class="server-icon server-icon-snapshots">
      <span class="icon">
        <a href="<?php print $link_href_snapshots; ?>" class="<?php print $link_class_active_snapshots; ?>">
          <i class="fa fa-files-o fa-4x"></i> Snapshots
        </a>
      </span>
    </span>

      <?php if (!empty($link_href_snapshot_policy)): ?>
        <span class="server-icon server-icon-snapshots">
      <span class="icon">
        <a href="<?php print $link_href_snapshot_policy; ?>" class="<?php print $link_class_active_snapshot_policy; ?>">
          <i class="fa fa-shield fa-4x"></i> Snapshot Policy
        </a>
      </span>
    </span>
      <?php endif; ?>

    </div>
  </div>
</nav>
