<?php
	$db_host = "127.0.0.1";
	$db_name = "addb";
	$db_user = "aduser";
	$db_password = "ad";

	header("Cache-Control: no-cache, must-revalidate");
	header("Content-type: text/plain; charset=utf-8");
	
	/* connect db */
	$link = mysql_connect($db_host, $db_user, $db_password)
		or die("Could not connect : " . mysql_error()); 
	mysql_select_db($db_name) or die("Could not select database");
	mysql_query("SET NAMES 'UTF8'");	
	mysql_query("set autocommit=1");	
?>
 