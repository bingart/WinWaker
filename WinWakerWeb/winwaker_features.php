<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Features</title>
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
		<h1 class="pagetitle">WinWaker Features</h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title">Overview</h2>
			<div class="entry">
				<p>
					<b>Working Mode</b>: waking up (simulate mouse event) or sleeping (do nothing);<br>
					<b>Application Filtering</b>: wake up windows only when specific filtered application is running;<br>
					<b>Auto Run When Windows start</b>: WinWaker will start automatically with Windows;<br>
					<b>Automatically Hiding Main Window</b>: Automatically hide main window into system tray;<br>
					<b>Mouse Step</b>: The step of simulated mouse movement is configurable;<br>
					<b>Mouse Idle Time</b>: The mouse idle time before waking up Windows;<br>
					<b>Application Filtering</b>: Filtering running application, only wake up Windows when specific application is running;<br>
					<b>Drag and Drop Application</b>: Drag shortcut and Drop into mina window or filtered application listbox in configuration dialog to add a filtered application;<br>
					<b>Online Software Updating</b>: Updating WinWaker online if new version is released;<br>
				</p>
			</div>
			<p class="meta"></p>

			<h2 class="title">Application Filtering</h2>
			<div class="entry">
				<p>
					Application Filtering is the most convenient feature of WinWaker, it provides user more flexibility to control how to wake up Windows;<br>
					The usage of application filtering is simple, only two steps: a. define a list of filtered application; b. enabled application filtering.<br>
					WinWaker checks each application in the list, if at least one application is running, WinWaker will wake up Windows. 
					For instance, define PowerPoint as filtered application, if working mode is set to Waking up and application filtering is enabled, WinWaker will not wake up Windows until PowerPoint is started, in other word, "waking up Windows only with PowerPoint";<br>
					<br>
					In summary, WinWaker will wake up Windows only if all of following conditions are met:<br>
					1. Working mode is set to waking up;<br>
					2. Applicatioin Filtering condition is OK when one of following three cases is matched:<br>
					&nbsp;&nbsp;&nbsp;&nbsp;a. Application filtering is disabled; OR<br>
					&nbsp;&nbsp;&nbsp;&nbsp;b. Application filtering is enabled AND at least one application is running; OR<br>
					&nbsp;&nbsp;&nbsp;&nbsp;c. Application filtering is enabled AND application list is empty; (no filtered application, application filtering feature is disabled automatically);<br>
					<br>
					For detail information about how to define application list, please see <a href="winwaker_document.php">Document</a> - <a href="winwaker_document.php#Add Filtered Application">Add Filtered Application</a>
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
