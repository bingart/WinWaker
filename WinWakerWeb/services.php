<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Services</title>
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
		<h1 class="pagetitle"></h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title">WinWaker</h2>
			<div class="entry">
				<p>
					<strong>WinWaker</strong> is the first free software that keeps waking up Windows to avoid unexpected screen saving or locking.
					It will be a helpful assistance for PPT presentation or other scenarios that requires Windows being ALWAYS ON.
				</p>
			</div>
			<p class="meta"></p>
		</div>
		<div class="post">
			<h2 class="title">WinRave</h2>
			<div class="entry">
				<p>
					<strong>WinRave</strong> is a free software that set several alarm time and send notification to user.
					It makes us invoke some body practice to avoid occupational disease, protecting our eyes, neck and other components.
					Like <a href="http://www.workrave.org">Workrave</a>, WinRave will be the body guard for IT guys.
					This software is under developing.
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
