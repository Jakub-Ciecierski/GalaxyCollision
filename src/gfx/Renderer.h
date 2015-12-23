#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "vector_types.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../../include/glm/glm/glm.hpp"
#include "../../include/glm/glm/gtc/matrix_transform.hpp"

#include "../../include/SDL2/include/SDL.h"
#include "../../include/SDL2/include/SDL_opengl.h"

#include "ParticlesCreator.h"
#include "shapes/Sphere.h"
#include "shapes/Cube.h"
#include "shapes/Triangle.h"
#include "util/ShaderLoader.h"
#include "Camera.h"
#include "Star.h"
#include "Galaxy.h"
#include "../util/GalaxyLoader.h"
#include "util/BMPLoader.h"

#include "../cuda/GalaxyCollision.cuh"

int GFXInit(int argc, char** argv);

int initWindow();

int initEvents();

int initProgram();

int initSampleObjects();
