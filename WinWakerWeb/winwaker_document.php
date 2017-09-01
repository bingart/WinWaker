<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>WinWaker Document</title>
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
		<h1 class="pagetitle">WinWaker Document</h1>
		<a href="#" id="rss-posts">RSS Feeds</a>
		<div class="post">
			<h2 class="title">Index</h2>
			<div class="entry">
				<p>
					<a href="winwaker_document.php#Minimum Requirement">Minimum Requirement</a><br/>
					<a href="winwaker_document.php#Installation">Installation</a><br/>
					<a href="winwaker_document.php#Quick Start">Quick Start</a><br/>
					<a href="winwaker_document.php#User Guide">User Guide</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#User Interface">User Interface</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#Change working mode">Change working mode</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#Context menu">Context menu</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#Configuration">Configuration</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#Add Filtered Application">Add Filtered Application</a><br/>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="winwaker_document.php#About">About</a><br/>
				</p>
			</div>
			<p class="meta"></p>
			<br>

			<a name="Minimum Requirement"></a>
			<h2 class="title">Minimum Requirement</h2>
			<div class="entry">
				<p>
					1. Windows XP or above;<br>
					2. At least 2 Mega memory;<br>
					3. At least 4 mega file storage;<br>
				</p>
			</div>
			<p class="meta"><a href="#">Back To Top</a></p>
			<br>
			
			<a name="Installation"></a>
			<h2 class="title">Installation</h2>
			<div class="entry">
				<p>
					1. Download latest stable version of WinWaker setup zip file;<br>
					2. Extract zip file into any folder;<br>
					3. Run setup exe file, WinWakerSetup-1.0.5.exe;<br>
					4. Follow the intruction of setup file as usual;<br>
				</p>
			</div>
			<p class="meta"><a href="#">Back To Top</a></p>
			<br>
			
			<a name="Quick Start"></a>
			<h2 class="title">Quick Start</h2>
			<div class="entry">
				<p>
					1. Open the WinWaker by clicking menu item or shortcut over desktop;<br>
					2. Set the working mode to Waking Up by clicking the button in main window;<br>
					3. Windows will never invoke screen saving !!!
					4. Drag a shortcut from desktop and drop into WinWaker main window, then Application Filter is enbled, the Windoes will be waken up only when the application (refered by the shortcut) is running;<br>
  				</p>
			</div>
			<p class="meta"><a href="#">Back To Top</a></p>
			<br>
			
			<a name="User Guide"></a>
			<h2 class="title">User Guide</h2>
			<div class="entry">

				<br>
				<a name="User Interface"></a>
				<h3>User Interface</h3>
				<p>
					WinWaker provide two user interface: main window and system tray icon.
					The title, tips and color of icon will be changed according to current working mode;
					When right click the main window or system tray icon, a popup context menu is displayed, the title of first menu item will be changed according to current working mode;
					The main window will be closed automatically when startup;<br>
					<br>
					<img src="images/winwaker_main_active_application_filtering.png" />
					<center><b><small>Figure 1.1: Main window with waking up mode and application filtering enabled</small></b></center>
					<br>
					<br>
					<img src="images/winwaker_main_active_no_condition.png" />
					<center><b><small>Figure 1.2: Main window with waking up mode and application filtering disabled</small></b></center>
					<br>
					<br>
					<img src="images/winwaker_main_deactive.png" />
					<center><b><small>Figure 2: Main window with sleeping mode</small></b></center>
					<br>
					<br>
					<center><img src="images/winwaker_tray_active.png" />
					<b><small>Figure 3: System tray icon with waking up mode</small></b></center>
					<br>
					<br>
					<img src="images/winwaker_tray_deactive.png" />
					<center><b><small>Figure 4: System tray icon with sleeping mode</small></b></center>
					<br>
					<br>
					<img src="images/winwaker_main_menu.png" />
					<center><b><small>Figure 5: Context menu for main window</small></b></center>
					<br>
					<br>
					<img src="images/winwaker_tray_menu.png" />
					<center><b><small>Figure 6: Context menu for system tray icon</small></b></center>
					<br>
					<br>	
  				</p>
				
				<br>
				<p class="meta"><a href="#">Back To Top</a></p>
				<a name="Change working mode"></a>
				<h3>Change working mode</h3>
				<p>
					Three ways to change working mode:<br>
					1. Click the button in main window;<br>
					2. Double click the system tray icon;<br>
					3. Right click the main window or system tray icon, pop up context menu and click the first menu item;
  				</p>

				<br>
				<p class="meta"><a href="#">Back To Top</a></p>
				<a name="Context menu"></a>
				<h3>Context menu</h3>
				<p>
					Right click the main window or system tray icon, context menu is displayed. The meaning of each menu item is:<br>
					1. Waking/Sleeping: change the working mode, the item title will be changed according to current working mode;<br>
					2. Hide/Open: hide the main window to system tray or open main window from system tray;<br>
					3. Config: open the configuration window;<br>
					4. About: open the about window;<br>
					5. Exit: exit WinWaker;
  				</p>

				<br>
				<p class="meta"><a href="#">Back To Top</a></p>
				<a name="Configuration"></a>
				<h3>Configuration</h3>
				<p>
					Within context menu, select Config item to open the configuration window.<br>
					<br>
					<img src="images/winwaker_configuration.png" />
					<center><b><small>Figure 7: Configuration window</small></b></center><br>
					<br>
					<br>
					<b>Auto Run When Start Windows:</b> if set, WinWaker will start automatically with Windows;<br>
					<b>Mouse Step:</b> the simulated mouse moving step, in pixel unit, the valid range is from 1 to 5, sugguest to set to 1 to avoid affecting the user's mouse movement;<br>
					<b>Mouse Idle Time:</b> the mouse idle time before waking up Windows, in second unit, the valid range is from 2 to 30; For instance, if set to 5 seconds, then the Windows will be waked up after mouse doesn't move for 5 seconds; The value should be less than the current Windows sleeping time;<br>
					<b>Enable Application Filtering:</b> if set, Application Filtering feature is enabled;<br>
					<b>Filtered Application List:</b> the applications to be filtered;<br>
					<b>Add:</b> open File Open Dialog to search a exe file or shortcut link file and add as application;<br>
					<b>Remove:</b> remove current selected application;<br>
					<b>Clear:</b> remove all applications;<br>
					<b>OK:</b> save and apply new configurations;<br>
					<b>Cancel:</b> discard new configurationis;<br>
					<b>Default:</b> set configuration to default value;
  				</p>

				<br>
				<p class="meta"><a href="#">Back To Top</a></p>
				<a name="Add Filtered Application"></a>
				<h3>Add Filtered Application</h3>
				<p>
					Three way to add applications:<br>
					<br>
					1. Drag shortcut or exe file and drop into main window, then the application is added and the filter application feature is enabled, it is also the quickest and simplest way, shown in Figure 8;
					<img src="images/winwaker_add_application_with_main_window.png" />
					<center><b><small>Figure 8: Add application with main window</small></b></center><br>
					<br>
					<br>
					2. Drag shortcut or exe file and drop into configuration window, then the application is added, shown in Figure 9;<br>
					<img src="images/winwaker_add_application_with_config_window.png" />
					<center><b><small>Figure 9: Add application with configuration window</small></b></center><br>
					<br>
					<br>
					3. Click the add button to open file dialog and select shortcut or exe file, shown in Figure 10;<br>
					<img src="images/winwaker_add_application_with_add_button.png" />
					<center><b><small>Figure 10: Add application with Add button</small></b></center><br>
					<br>
					<br>
					Note:<br>
					1. The duplicated application will be ignored;<br>
					2. For some specific application, such microsoft office word/excel/powerpoint, the exe file name doesn't match the shortcut definition, WinWaker invokes conversion for them;
	  			</p>
				
				<br>				
				<p class="meta"><a href="#">Back To Top</a></p>
				<a name="About"></a>
				<h3>About</h3>
				<p>
					Within context menu, select About item to open the about window. The software version and license information is provided. click button to close.<br>
					<br>
					<img src="images/winwaker_about.png" />
					<center><b><small>Figure 11: About window</small></b></center><br>
					<br>
					<br>
  				</p>

			</div>
			<p class="meta"><a href="#">Back To Top</a></p>
			
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
