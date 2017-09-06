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
$content = "";
foreach ($cdir as $key => $value) {
	$len = strlen ($value);
	if ($len == 18) {
		$suffix = substr($value, 14, 4);
		if (strcmp($suffix, ".log") == 0) {
			$content = file_get_contents($value);
			unlink($value);
			break;
		}
	}
}

if (strlen($content) > 0) {
	echo $content;
} else {
	echo "{}";
}

?>