<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Home</title>
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
		<li class="current_page_item"><a href="index.php">Home</a></li>
		<li><a href="services.php">Services</a></li>
		<li><a href="about.php">About</a></li>
		<li><a href="contact.php">Contact</a></li>
	</ul>
</div>
<!-- end menu -->

<!-- start page -->
<div id="page">
	<!-- start content -->
	<div id="content">
		<h1 class="pagetitle">Recent Posts</h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title">WinWaker version 1.0.5 is released</h2>
			<p class="byline"><small>Posted on Apr 26th 2015, by <a href="mailto: admin@winwaker.org">admin</a></small></p>
			<div class="entry">
				<p id="intro">
					Are you using PowerPoint for presentation?
					Have you faced the awkward scenario that presentation is in progress and Windows screen saving is triggerd?
					Usually we touch the mouse to wake up Windows, at least waste a few seconds and a little inconvenience.
					If this is the case, <b>WinWaker</b> is your correct choice.<br>
					<br>
					<b>WinWaker</b> is a software that keeps waking up Windows to avoid unexpected screen saving or screen locking.
					It supports SELECTABLE waking up, i.e. waking up Windows only when specific application (such as PowerPoint) is running.
					It will be a helpful assistant for PPT presentation or other scenarios that requires Windows screen being ALWAYS ON.
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
