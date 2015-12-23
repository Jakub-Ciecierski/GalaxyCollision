/* 
 * File:   Galaxy.cpp
 * Author: ciecierskij
 * 
 * Created on July 18, 2015, 3:51 PM
 */

#include "Galaxy.h"

Galaxy::Galaxy() {
}

Galaxy::Galaxy(Star** disk, Star** bulge, Star** halo,
            int diskSize, int bulgeSize, int haloSize)
{
    this->disk = disk;
    this->bulge = bulge;
    this->halo = halo;
    
    this->diskSize = diskSize;
    this->bulgeSize = bulgeSize;
    this->haloSize = haloSize;
}

Galaxy::Galaxy(const Galaxy& orig) {
}

Galaxy::~Galaxy() {
    delete disk;
    delete bulge;
    delete halo;
}

int Galaxy::TotalSize()
{
	return diskSize + bulgeSize + haloSize;
}
