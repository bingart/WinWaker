<?php

if(isset($_GET['token'])) {
	$token = $_GET['token'];
	$expect = "winwaker";
	if (strcmp($token, $expect) !== 0) {
		die;
	}
} else {
	die;
}

$cdir = scandir('.');
$count = 0;
foreach ($cdir as $key => $value) {
	$len = strlen ($value);
	if ($len == 18) {
		$suffix = substr($value, 14, 4);
		if (strcmp($suffix, ".log") == 0) {
			echo $value;
			echo "<br />";
			$count++;
		}
	}
}
echo 'OK, count='.$count;

?>