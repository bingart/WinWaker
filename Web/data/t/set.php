<?php

$now = date("YmdHis");
$file_name = './'.$now.'.log';

$url = $_POST['url'];
$url = base64_decode ($url);
$delay = $_POST['delay'];
$action = $_POST['action'];

$arr = array('u' => $url, 'd' => $delay, 'a' => $action);
$json = json_encode($arr);

$fp = fopen($file_name, 'w');
fwrite($fp, $json);
fclose($fp);

echo 'OK'.$file_name;

?>