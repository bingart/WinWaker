<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker FAQ</title>
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
		<h1 class="pagetitle">WinWaker FAQ</h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title"></h2>
			<div class="entry">
				<p>
					<strong>1. Does WinWaker monitor the windows mouse event or keyboard event?</strong><br>
					<strong>A.</strong> WinWaker does NOT install any mouse or keyboard hook to monitor windows messages;
					As the step of simulated mouse event could be decreased to 1 pixel, WinWaker does NOT affect your usual working.
					You don't almost feel the existence of WinWaker.
				</p>
				<p>
					<strong>2. How does WinWaker simulate mouse event?</strong><br>
					<strong>A.</strong> WinWaker simulate only the mouse moving event.
					For instance, if step is set to 1, then after a configurable interval time, a mouse move event (1 pixel at X direction and 1 pixel at Y direction) is sent to Windows;
					After the second interval time, the second mouse move event (-1 pixel at X direction and -1 pixel at Y direction) is sent to Windows, repeat the above process again and again.
					And so on ... As a result, WinWaker does NOT affect the user mouse movement essentially.
				</p>
			  <p>
					<strong>3. Why drag and drop a shortcut and nothing happened?</strong><br>
					<strong>A.</strong> WinWaker supports two types of files for draging and dropping, the short cut (lnk) and exe files.
					if drag and drop a short cut file into main window or configuration window and nothing hanppened, this must be the case that the target or short cut file is not exe file.
					The final item type of filtered application list is execute file, the short cut is just the link of execute file.
					Please check the properties of short cut file, right click the short cut and select properties and select Shortcut tab page and check Target entry value, check if it is exe file.
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
