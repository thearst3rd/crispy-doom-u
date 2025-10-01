//
// Copyright(C) 2021 Terry Hearst
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
//
//	Wii U Launcher/WAD picker implementation
//

#include "wiiu_launcher.h"

#ifdef __WIIU__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

#include <vpad/input.h>
#include <coreinit/screen.h>
#include <coreinit/cache.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>
#include <proc_ui/procui.h>
#include <sysapp/launch.h>
#include <gx2/state.h>

#include "config.h"

#include "i_system.h"
#include "m_argv.h"

#include "wiiu_launcher_main.h"
#include "wiiu_launcher_nowads.h"
#include "wiiu_controller.h"
#include "wiiu_oscreen.h"

// Global variables
int launcherRunning = 1; // 0 means go to game, -1 means quit
launcherState state = LAUNCHER_MAIN;

extern char **foundWads;
extern int *selectedWads;
extern int selectedWadsCount;

void launcherUpdate()
{
    switch (state)
    {
        case LAUNCHER_MAIN: launcherMainUpdate(); break;
        case LAUNCHER_NOWADS: launcherNoWadsUpdate(); break;
        default:
            // NOP...
            break;
    }
}

void launcherDraw(OSScreenID screenID)
{
    char buf[60];
    switch (state)
    {
        case LAUNCHER_MAIN: launcherMainDraw(screenID); break;
        case LAUNCHER_NOWADS: launcherNoWadsDraw(screenID); break;
        default:
            sprintf(buf, "ERROR: Unknown launcher state: %d", state);
            OSScreenPutFontEx(screenID, 0, 0, buf);
            OSScreenPutFontEx(screenID, 0, 1, "Press Home to exit");
            break;
    }
}

void generateArgcArgv()
{
    // Build up "myargc" and "myargv" from stuff user selected
    char *origArgv0 = myargv[0];
    if (selectedWadsCount > 1)
        myargc = selectedWadsCount + 3;
    else if (selectedWadsCount == 1)
        myargc = 3;
    else
        myargc = 1;

    myargv = malloc(myargc * sizeof(char *));
    int currArg = 0;
    myargv[currArg++] = strdup(origArgv0);

    if (selectedWadsCount > 0)
    {
        myargv[currArg++] = strdup("-iwad");
        myargv[currArg++] = strdup(foundWads[selectedWads[0]]);

        if (selectedWadsCount > 1)
        {
            myargv[currArg++] = strdup("-file");
            for (int i = 1; i < selectedWadsCount; i++)
            {
                myargv[currArg++] = strdup(foundWads[selectedWads[i]]);
            }
        }
    }
}

void launcherRun()
{
    // Init launcher using new OSScreen management system
    WiiU_OSScreenInit();

    // Init launcher states
    launcherMainInit();
    launcherNoWadsInit();

    // Launcher loop - no ProcUI to avoid crashes
    while (launcherRunning > 0)
    {
        // Poll input
        WiiU_PollJoystick();
        launcherUpdate();

        // Draw screen
        OSScreenClearBufferEx(SCREEN_TV, 0x00000000);
        OSScreenClearBufferEx(SCREEN_DRC, 0x00000000);

        launcherDraw(SCREEN_TV);
        launcherDraw(SCREEN_DRC);

        DCFlushRange(g_tvBuffer, g_tvBufferSize);
        DCFlushRange(g_drcBuffer, g_drcBufferSize);

        OSScreenFlipBuffersEx(SCREEN_TV);
        OSScreenFlipBuffersEx(SCREEN_DRC);
        
        // Small delay to prevent excessive CPU usage
        OSSleepTicks(OSMillisecondsToTicks(16)); // ~60 FPS
    }

    // Launcher exit is handled by the main loop
    // No need for custom app running flag

    // Present clean frame and mark OSScreen for shutdown
    // Actual shutdown will be completed after SDL presents its first frame
    WiiU_OSScreenPresentCleanFrame();
    WiiU_OSScreenMarkForShutdown();

    if (launcherRunning >= 0)
    {
        generateArgcArgv();
    }

    // Cleanup launcher states
    launcherMainCleanup();
    launcherNoWadsCleanup();
}

#endif // __WIIU__
