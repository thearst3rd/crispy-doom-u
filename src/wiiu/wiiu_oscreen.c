//
// Copyright(C) 2025 strejf79
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
// Wii U OSScreen management implementation
//

#ifdef __WIIU__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "doomtype.h"
#include <coreinit/screen.h>
#include <coreinit/cache.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

#include "wiiu_oscreen.h"

// Global variables for OSScreen buffer management
void* g_tvBuffer = NULL;
void* g_drcBuffer = NULL;
int g_tvBufferSize = 0;
int g_drcBufferSize = 0;
boolean g_osScreenShutdownPending = false;

void WiiU_OSScreenInit(void)
{
    // Initialize OSScreen
    OSScreenInit();
    
    // Allocate buffers
    g_tvBufferSize = OSScreenGetBufferSizeEx(SCREEN_TV);
    g_drcBufferSize = OSScreenGetBufferSizeEx(SCREEN_DRC);
    
    g_tvBuffer = memalign(0x100, g_tvBufferSize);
    g_drcBuffer = memalign(0x100, g_drcBufferSize);
    
    if (!g_tvBuffer || !g_drcBuffer)
    {
        // Cleanup on failure
        if (g_tvBuffer)
            free(g_tvBuffer);
        if (g_drcBuffer)
            free(g_drcBuffer);
        g_tvBuffer = NULL;
        g_drcBuffer = NULL;
        OSScreenShutdown();
        return;
    }
    
    // Set up buffers
    OSScreenSetBufferEx(SCREEN_TV, g_tvBuffer);
    OSScreenSetBufferEx(SCREEN_DRC, g_drcBuffer);
    OSScreenEnableEx(SCREEN_TV, true);
    OSScreenEnableEx(SCREEN_DRC, true);
    
    // Reset shutdown flag
    g_osScreenShutdownPending = false;
}

void WiiU_OSScreenPresentCleanFrame(void)
{
    if (!g_tvBuffer || !g_drcBuffer)
        return;
        
    // Present a clean black frame before SDL handover
    OSScreenClearBufferEx(SCREEN_TV, 0x00000000);
    OSScreenClearBufferEx(SCREEN_DRC, 0x00000000);
    DCFlushRange(g_tvBuffer, g_tvBufferSize);
    DCFlushRange(g_drcBuffer, g_drcBufferSize);
    OSScreenFlipBuffersEx(SCREEN_TV);
    OSScreenFlipBuffersEx(SCREEN_DRC);
    
    // Give the display a frame to pick up the cleared buffers
    OSSleepTicks(OSMillisecondsToTicks(16));
}

void WiiU_OSScreenMarkForShutdown(void)
{
    // Mark for shutdown but don't actually shutdown yet
    // This will be completed after SDL presents its first frame
    g_osScreenShutdownPending = true;
}

void WiiU_OSScreenCompleteShutdown(void)
{
    if (!g_osScreenShutdownPending)
        return;
        
    // Now it's safe to shutdown OSScreen since SDL has taken over
    OSScreenShutdown();
    
    // Free buffers
    if (g_tvBuffer)
    {
        free(g_tvBuffer);
        g_tvBuffer = NULL;
    }
    if (g_drcBuffer)
    {
        free(g_drcBuffer);
        g_drcBuffer = NULL;
    }
    
    g_osScreenShutdownPending = false;
}

boolean WiiU_OSScreenShutdownPending(void)
{
    return g_osScreenShutdownPending;
}

#endif // __WIIU__
