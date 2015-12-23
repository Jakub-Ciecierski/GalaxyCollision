#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Macros.h"
#include "../gfx/Renderer.h"
#include "../gfx/Star.h"
#include "../gfx/Galaxy.h"

#define GLX_SKIP 1

#define GALAXY_COUNT 2

#define DISK_COUNT 16384
#define MAX_MW_DISK_COUNT 16384 / 5
#define MAX_AND_DISK_COUNT 16384 / 5

// old
#define MAX_DISK_COUNT DISK_COUNT / GLX_SKIP

#define BULGE_COUNT 8192
#define MAX_MW_BULGE_COUNT 8192 / 5
#define MAX_AND_BULGE_COUNT 8192 / 5

// old
#define MAX_BULGE_COUNT BULGE_COUNT / GLX_SKIP

#define HALO_COUNT 16384
#define MAX_MW_HALO_COUNT 0//16384
#define MAX_AND_HALO_COUNT 0//16384

// old
#define MAX_HALO_COUNT HALO_COUNT / GLX_SKIP

#define MAX_STAR_COUNT MAX_DISK_COUNT + MAX_BULGE_COUNT + MAX_HALO_COUNT

#define DISK_EXT ".disk"
#define BULGE_EXT ".bulge"
#define HALO_EXT ".halo"

#define MW_FILENAME "MW"
#define AND_FILENAME "AND"

#define FILENAME_SIZE 512

using namespace std;

Star** getStars(char* filepath, int maxSize, int glx_name, int glx_type, int* size);
Galaxy** glxParseGalaxies(const char* dir);

void printSampleStars(Galaxy** galaxies);
