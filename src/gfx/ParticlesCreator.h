/*
 * ParticlesCreator.h
 *
 *  Created on: Jul 22, 2015
 *      Author: jakub
 */

#ifndef PARTICLESCREATOR_H_
#define PARTICLESCREATOR_H_

#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../../include/glm/glm/glm.hpp"
#include "../../include/glm/glm/gtc/matrix_transform.hpp"

class ParticlesCreator {
private:

	GLuint vertex_buffer;
	GLuint position_buffer;
	GLuint color_buffer;

	void initVBO();
	void initGFXArrays();

public:

	int particleCount;

	GLfloat* g_position_size_data;
	GLubyte* g_color_data;

	ParticlesCreator(int particleCount);
	virtual ~ParticlesCreator();

	void BindVBO();
	void UnBindVBO();
};

#endif /* PARTICLESCREATOR_H_ */
