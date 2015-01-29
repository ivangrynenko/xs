<?php

	$url = "http://www.whmcs.com/index.php";

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_TIMEOUT, 10);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	$data = curl_exec($ch);
	
	echo "CURL Connection: ";
	
	if (curl_error($ch)) {
		echo "ERROR ".curl_error($ch)."<br><br>";
	} else {
		echo "SUCCESS<br><br>";
	}
	
	curl_close($ch);
	
	echo "Data:<br><textarea rows=\"20\" cols=\"120\">$data</textarea>";
	
?>