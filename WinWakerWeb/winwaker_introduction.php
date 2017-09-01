<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Introduction</title>
<?php
	include "meta.php";
?>
<link href="default.css" rel="stylesheet" type="text/css" media="screen" />
</head>

<body>

<?php
	include "header.php";
?>

<!-- start menu -->
<div id="menu">
	<ul>
		<li><a href="index.php">Home</a></li>
		<li class="current_page_item"><a href="services.php">Services</a></li>
		<li><a href="about.php">About</a></li>
		<li><a href="contact.php">Contact</a></li>
	</ul>
</div>
<!-- end menu -->

<!-- start page -->
<div id="page">
	<!-- start content -->
	<div id="content">
		<h1 class="pagetitle">WinWaker Introduction</h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title"></h2>
			<div class="entry">
				<p>
					<b>WinWaker</b> is a free software that keeps waking up Windows to avoid unexpected screen saving or locking.
					It will be a helpful assistance for PPT presentation or other scenarios that requires Windows screen being ALWAYS ON.<br>
					<b>WinWaker</b> simulates user mouse movement and send to Windows, and stop Windows to active its screen saving or locking.<br>
					<b>WinWaker</b> allows you to define accurate conditions to wake up Windows, Waking up Windows only when specific application runs.
				</p>
			</div>
			<p class="meta"></p>
		</div>
	</div>
	<!-- end content -->
	
	<?php
		include "sidebar.php";
	?>
	
	<div style="clear: both;">&nbsp;</div>
</div>
<!-- end page -->

<?php
	include "footer.php";
?>

</body>

</html>
