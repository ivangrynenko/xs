<?php
  if (!empty($_GET['aff'])) {
    header('Location: https://clientarea.redy.host/aff.php?aff=' . $_GET['aff']);
  }
  else {
    header('Location: https://www.redy.host');
  }
?>
