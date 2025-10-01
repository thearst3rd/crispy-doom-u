//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Wii U platform exit handling.
//


#include "wiiu_exit.h"
#include <proc_ui/procui.h>
#include <sysapp/launch.h>

// Global flag for clean exit request from in-game quit menu
boolean g_request_app_exit = false;

// Function to request clean exit from in-game quit menu
void platform_request_exit(void)
{
    g_request_app_exit = true;
}

// Manual ProcUI cleanup to ensure proper shutdown
void cleanup_procui(void)
{
    if (ProcUIIsRunning()) {
        if (!ProcUIInShutdown()) {
            SYSLaunchMenu();
            boolean still_running = true;
            while (still_running) {
                ProcUIStatus status = ProcUIProcessMessages(true);
                switch (status) {
                    case PROCUI_STATUS_EXITING:
                        still_running = false;
                        break;
                    case PROCUI_STATUS_IN_FOREGROUND:
                        break;
                    case PROCUI_STATUS_IN_BACKGROUND:
                        break;
                    case PROCUI_STATUS_RELEASE_FOREGROUND:
                        ProcUIDrawDoneRelease();
                        break;
                }
            }
        }
        ProcUIShutdown();
    }
}
