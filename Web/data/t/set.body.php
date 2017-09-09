<?php

$now = date("YmdHis");
$file_name = './'.$now.'.log';

var_dump($_POST);

$json = file_get_contents('php://input');
$fp = fopen($file_name, 'w');
fwrite($fp, $json);
fclose($fp);

echo 'OK'.$file_name;

?>