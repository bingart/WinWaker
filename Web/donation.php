<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Donation</title>
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
		<li><a href="services.php">Services</a></li>
		<li><a href="about.php">About</a></li>
		<li class="current_page_item"><a href="contact.php">Contact</a></li>
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
			<h2 class="title">Donation</h2>
			<p class="byline"><small>Posted on Apr 26th 2015, by <a href="mailto: admin@winwaker.org">admin</a></small></p>
			<div class="entry">
				<p>
					As a free software workshop, we will try to generate as much free and helpful softwares with the best of my ability.<br>
					If WinWaker is real helpful, please send donation to :<br>
					paypal account: <b>admin@winwaker.org</b><br>
					alipay account: <b>admin@winwaker.org</b><br>
					You could also click our <a href="ad.php">advertisement page</a> to provide us more visibility.<br>
					Thanks for your support and generousness !!!
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
