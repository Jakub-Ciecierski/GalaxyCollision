#pragma once

#include <iostream>
#include <string.h>

#include "shapes/Sphere.h"
#include "shapes/Point.h"
#include <GL/freeglut.h>

using namespace std;

#define GLX_MW 0
#define GLX_AND 1

#define GLX_DISK 0
#define GLX_BULGE 1
#define GLX_HALO 2

class Star
{
private:

    int galaxyType;
    int starType;

    float const width = 10.0f;

    float speed = 0.005f;
    int oldTimeSinceStart = 0;

    Sphere* sphere;
    float x, y, z;
public:
    Point* point;
    float mass;
    
    float vx, vy, vz;

    Star();

    Star(float mass, float x, float y, float z,
        float vx, float vy, float vz);

    Star(float mass, float x, float y, float z,
            float vx, float vy, float vz, GLuint Texture, GLuint TextureID);

    void setGalaxyType(int galaxyType);
    void setStarType(int starType);

    void Update(float ax, float ay, float az);
    void Update(float ax, float ay, float az, float maxVelocity);

    Sphere* GetSphere();
    
    Point* GetPoint();
    
    float GetVelocityLength();
    
    friend ostream& operator<<(ostream& os, Star& star);
};

