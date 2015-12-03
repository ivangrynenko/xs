<?php

require("../dbconnect.php");

if ($pid) {
    header("Location: ../cart.php?pid=".(int)$pid."&carttpl=ajaxcart");
    exit;
}

header("Location: ../cart.php?carttpl=ajaxcart");
exit;

?>