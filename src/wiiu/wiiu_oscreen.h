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
// Wii U OSScreen management for smooth launcher-to-game transition
//

#ifndef WIIU_OSCREEN_H
#define WIIU_OSCREEN_H

#ifdef __WIIU__

#include "doomtype.h"
#include <coreinit/screen.h>
#include <coreinit/cache.h>
#include <coreinit/time.h>
#include <coreinit/thread.h>

// Global variables for OSScreen buffer management
extern void* g_tvBuffer;
extern void* g_drcBuffer;
extern int g_tvBufferSize;
extern int g_drcBufferSize;
extern boolean g_osScreenShutdownPending;

// Initialize OSScreen buffers (called from launcher)
void WiiU_OSScreenInit(void);

// Present clean black frame and prepare for SDL handover
void WiiU_OSScreenPresentCleanFrame(void);

// Mark OSScreen for shutdown (called when launcher exits)
void WiiU_OSScreenMarkForShutdown(void);

// Complete OSScreen shutdown (called after SDL presents first frame)
void WiiU_OSScreenCompleteShutdown(void);

// Check if OSScreen shutdown is pending
boolean WiiU_OSScreenShutdownPending(void);

#endif // __WIIU__

#endif // WIIU_OSCREEN_H
