<?php
	include "db.php";

	$ip = $_SERVER["REMOTE_ADDR"];
	$report_key = $_POST['report_key'];

	$query = "INSERT INTO `tb_install` (`ip`, `install_time`, `install_name`) VALUES ('$ip', NOW(), '$report_key')";
	$result = mysql_query($query);
	if ($result) {
		$ok = true;
	} else {
		$ok = false;
	}

	if ($ok) {
		echo "ok";
	} else {
		echo "ko";
	}
?> 