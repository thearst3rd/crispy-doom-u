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
//	Wii U Launcher - Main screen
//

#include "launcherMain.h"

#ifdef __WIIU__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "wiiu-config.h"
#include "launcher.h"

extern int launcherRunning;
extern launcherState state;

// Scanned WADs
char **foundWads = NULL;
int foundWadsCount = 0;
int selectedWadIndex = 0;

void addWad(char *wadname)
{
    foundWadsCount++;
    foundWads = realloc(foundWads, foundWadsCount * sizeof(char *));
    // Insert alphabetically
    bool added = false;
    for (int i = 0; i < foundWadsCount - 1; i++)
    {
        if (strcmp(wadname, foundWads[i]) < 0)
        {
            // Shift all other entries down
            for (int j = foundWadsCount - 2; j >= i; j--)
            {
                foundWads[j + 1] = foundWads[j];
            }
            foundWads[i] = wadname;
            added = true;
            break;
        }
    }
    // If we made it all the way to the end and it still hasn't been added,
    // added, make it the last thing in the list
    if (!added)
    {
        foundWads[foundWadsCount - 1] = wadname;
    }
}

char *joinDirs(char *dirname, char *filename)
{
    size_t dirlen = strlen(dirname);
    if (dirlen == 0)
        return strdup(filename);
    // + 2 for slash and null terminator
    size_t joinedLen = dirlen + strlen(filename) + 2;
    char *joined = (char *) malloc(joinedLen * sizeof(char));
    strcpy(joined, dirname);
    joined[dirlen] = '/';
    strcpy(joined + dirlen + 1, filename);
    return joined;
}

void addWadsInDir(char *dirname, char *relDirname)
{
    struct dirent *files;
    DIR *dir = opendir(dirname);
    if (dir != NULL)
    {
        while ((files = readdir(dir)) != NULL)
        {
            char *filename = joinDirs(dirname, files->d_name);
            char *relFilename = joinDirs(relDirname, files->d_name);

            // Check if file or directory
            struct stat path_stat;
            stat(filename, &path_stat);
            if (S_ISREG(path_stat.st_mode))
            {
                addWad(relFilename);
            }
            else
            {
                if (S_ISDIR(path_stat.st_mode))
                {
                    addWadsInDir(filename, relFilename);
                }
                free(relFilename);
            }
            free(filename);
        }
        closedir(dir);
    }
}

void launcherMainInit()
{
    foundWads = NULL;
    foundWadsCount = 0;
    // Scan for WADs
    addWadsInDir(HOMEBREW_APP_PATH "/wads", "");

    if (foundWadsCount == 0)
        state = LAUNCHER_NOWADS;
}

void launcherMainUpdate(VPADStatus status)
{
    if (status.trigger & (VPAD_BUTTON_UP | VPAD_STICK_L_EMULATION_UP))
        selectedWadIndex--;
    if (status.trigger & (VPAD_BUTTON_DOWN | VPAD_STICK_L_EMULATION_DOWN))
        selectedWadIndex++;

    if (selectedWadIndex < 0)
        selectedWadIndex = foundWadsCount - 1;
    else if (selectedWadIndex >= foundWadsCount)
        selectedWadIndex = 0;

    if (status.trigger & VPAD_BUTTON_PLUS)
        launcherRunning = 0;
}

void launcherMainDraw(OSScreenID screenID)
{
    OSScreenPutFontEx(screenID, 0, 0, "Crispy Doom");

    OSScreenPutFontEx(screenID, 0, 2, "Press up and down to select a WAD");
    OSScreenPutFontEx(screenID, 0, 3, "Press + to start playing");
    for (int i = 0; i < foundWadsCount; i++)
    {
        int y = i + 5;
        OSScreenPutFontEx(screenID, 5, y, foundWads[i]);
        if (selectedWadIndex == i)
            OSScreenPutFontEx(screenID, 2, y, ">>");
    }
}

void launcherMainCleanup()
{
    if (foundWadsCount > 0)
    {
        // Free found WADs
        for (int i = 0; i < foundWadsCount; i++)
        {
            free(foundWads[i]);
        }
        free(foundWads);
    }
}

#endif // __WIIU__
