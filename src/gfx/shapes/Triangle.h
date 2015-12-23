#pragma once

#include <GL/glew.h>
#include "../../../include/glm/glm/glm.hpp"
//#include <glm/glm.hpp>

class Triangle
{
private:
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    GLuint colorbuffer;

public:
    Triangle();

    void Render();
};

