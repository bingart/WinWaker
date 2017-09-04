#ifndef INTERFACE_H
#define INTERFACE_H

#include <Windows.h>

#define WM_MOUSE_NOTIFY (WM_APP + 0)
#define WM_CONFIG_NOTIFY (WM_APP + 1)
#define WM_ABOUT_NOTIFY (WM_APP + 2)
#define WM_TIP_NOTIFY (WM_APP + 3)

#define DAEMON_WINDOW_NAME ("WinWakerDaemon")
#define WM_SHUTDOWN (WM_APP + 100)
#define WM_REOPEN (WM_APP + 101)

#endif