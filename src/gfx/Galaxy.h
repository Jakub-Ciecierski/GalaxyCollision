/* 
 * File:   Galaxy.h
 * Author: ciecierskij
 *
 * Created on July 18, 2015, 3:51 PM
 */

#ifndef GALAXY_H
#define	GALAXY_H

#include "Star.h"

class Galaxy {
    
public:
    Galaxy();
    Galaxy(Star** disk, Star** bulge, Star** halo,
            int disksize, int bulgeSize, int haloSize);
    
    Galaxy(const Galaxy& orig);
    virtual ~Galaxy();
    
    
    int diskSize;
    int bulgeSize;
    int haloSize;
    
    Star** disk;
    Star** bulge;
    Star** halo;

    int TotalSize();
private:

};

#endif	/* GALAXY_H */

