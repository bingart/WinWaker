<?
	$filePath = "download/WinWakerSetup_1.0.5.zip";
	$fileSize = filesize($filePath);
	$file = fopen($filePath, "r");
	
	header("Pragma: public" );
	header("Expires: 0" ); 
	header("Accept-Ranges: bytes");
	header("Content-type: application/zip");
	header("Content-Length: ".$fileSize);
	header("Content-Disposition: attachment; filename=WinWakerSetup_1.0.5.zip");

	while (!feof($handle))
	{
		if ($fileSize > 1024)
		{
			$readSize = 1024;
		}
		else
		{
			$readSize = $fileSize;
		}
		
		if ($readSize > 0)
		{
			$string = fread($file, $readSize);
			echo $string;
		}
		
		$fileSize = $fileSize - $readSize;
		
		usleep(100);
	}

	fclose($file);

	exit;
?>