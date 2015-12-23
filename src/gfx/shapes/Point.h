#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include "../../../include/glm/glm/glm.hpp"
#include "../../../include/glm/glm/gtc/matrix_transform.hpp"
#include <vector>

class Point
{
private:


    glm::mat4 Model;
    int counter = 0;
    
    void init();

    void initVertices();
    void initColor();
    void initTexture();
    void initIndices();

public:
    
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint uvbuffer;
    GLuint elementbuffer;

    GLuint Texture;
    GLuint TextureID;

    int indicesSize = 1;
    
    float X;
    float Y;
    float Z;

    float R;
    float G;
    float B;

    float ScaleX;
    float ScaleY;
    float ScaleZ;
    
    Point();
    Point(float x, float y, float z);
    Point(float x, float y, float z, GLuint texture, GLuint textureID);

    void UpdateColor(float r, float g, float b);
    void UpdateColor();
    
    void Render();
    void RenderTexture();
    void RenderTextureIndices();
    
    glm::mat4 GetModelMatrix();
};

