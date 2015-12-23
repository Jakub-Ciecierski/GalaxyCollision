#pragma once

#include <GL/glew.h>
#include "../../../include/glm/glm/glm.hpp"
//#include <glm/glm.hpp>

class Cube
{
private:
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    GLuint colorbuffer;

public:
    Cube();

    void Render();
};
