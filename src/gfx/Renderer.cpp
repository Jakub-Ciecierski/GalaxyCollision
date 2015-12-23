#include "Renderer.h"

const int windowWidth = 1024;
const int windowHeight = 768;

/*********************/
/***** VARIABLES *****/
/*********************/

glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

GLuint programID;
GLuint MatrixID;

GLuint Texture;
GLuint TextureID;

GLuint CameraRight_worldspace_ID;
GLuint CameraUp_worldspace_ID;
GLuint ViewProjMatrixID;


Triangle* triangle;
Cube* cube;
Sphere* sphere;
Sphere* sphere1;
Camera* camera;

Galaxy** galaxies;

ParticlesCreator* particles;

//  The number of frames
int frameCount = 0;
//  Number of frames per second
float fps = 0;

//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;
int counter = 0;

/*********************/
/***** FUNCtIONS *****/
/*********************/

void bindVBO(GLuint vertexBuffer, GLuint uvbuffer,
        GLuint elementbuffer, GLuint colorbuffer, int indiciesSize);
void unbindVBO();

void render(void);
void keyboard(unsigned char c, int x, int y);
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMove(int x, int y);
void calculateFPS();

void updateGalaxies();
float4* getGalaxiesDescription(int count, float* maxVelocity);

void render(void)
{
    camera->Update();
    calculateFPS();

    //system("clear");
    //printf("FPS: %lf\n", fps);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    updateGalaxies();

    //Sphere* i_sphere = _galaxies[0]->bulge[0]->GetSphere();
    Point* i_sphere = galaxies[0]->bulge[0]->GetPoint();
    // Bind VBO etc
    int indiciesSize = i_sphere->indicesSize;

    GLuint colorbuffer = i_sphere->colorbuffer;

    bindVBO(i_sphere->vertexbuffer, i_sphere->uvbuffer,
            i_sphere->elementbuffer, i_sphere->colorbuffer, i_sphere->indicesSize);

    // compute this part of MVP only once
    glm::mat4 tmp_MVP = camera->ProjectionMatrix * camera->ViewMatrix;

    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        int size;
        Star** stars;

        stars = galaxies[i]->disk;
        size = galaxies[i]->diskSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            MVP = tmp_MVP * star->GetPoint()->GetModelMatrix();
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            star->point->UpdateColor();
            
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, star->point->colorbuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glDrawElements(GL_POINTS, indiciesSize, GL_UNSIGNED_SHORT, (void*)0 );

            glDisableVertexAttribArray(1);
        }

        stars = galaxies[i]->bulge;
        size = galaxies[i]->bulgeSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            star->point->UpdateColor();

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, star->point->colorbuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            MVP = tmp_MVP * star->GetPoint()->GetModelMatrix();
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            glDrawElements(GL_POINTS, indiciesSize, GL_UNSIGNED_SHORT, (void*)0 );

            glDisableVertexAttribArray(1);
        }

        stars = galaxies[i]->halo;
        size = galaxies[i]->haloSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            star->point->UpdateColor();

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, star->point->colorbuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            MVP = tmp_MVP * star->GetPoint()->GetModelMatrix();
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            glDrawElements(GL_POINTS, indiciesSize, GL_UNSIGNED_SHORT, (void*)0 );

            glDisableVertexAttribArray(1);
        }
    }

    unbindVBO();

    glutSwapBuffers();

    glutPostRedisplay();
}

void updateGalaxies()
{
    int count = galaxies[0]->TotalSize() + galaxies[1]->TotalSize();

    float maxVelocity = 0;

    float4* bodyDescription = getGalaxiesDescription(count, &maxVelocity);
    float3* acceleration = (float3*)malloc(sizeof(float3) * count);

    galaxyCollisionInit(bodyDescription, acceleration, count);

    int index = 0;
    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        int size;
        Star** stars;

        stars = galaxies[i]->disk;
        size = galaxies[i]->diskSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            float3 acc = acceleration[index++];
            star->Update(acc.x, acc.y, acc.z, maxVelocity);
        }

        stars = galaxies[i]->bulge;
        size = galaxies[i]->bulgeSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            float3 acc = acceleration[index++];
            star->Update(acc.x, acc.y, acc.z, maxVelocity);
        }

        stars = galaxies[i]->halo;
        size = galaxies[i]->haloSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            float3 acc = acceleration[index++];
            star->Update(acc.x, acc.y, acc.z, maxVelocity);
        }
    }

    free(bodyDescription);
    free(acceleration);
}

float4* getGalaxiesDescription(int count, float* maxVelocity)
{
    //int count = galaxies[0]->TotalSize() + galaxies[1]->TotalSize();

    //int sizef3 = count * sizeof(float3);
    int sizef4 = count * sizeof(float4);

    //fprintf(stderr, "[Cuda] Bodies Count: %d \n", count);

    float4* bodyDescription = (float4*)malloc(sizef4);
    //float3* acceleration = (float3*)malloc(sizef3);

    int index = 0;

    *maxVelocity = galaxies[0]->bulge[0]->GetVelocityLength();

    // Init the body description - position and mass
    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        int size;
        Star** stars;

        stars = galaxies[i]->disk;
        size = galaxies[i]->diskSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            bodyDescription[index].x = star->point->X;
            bodyDescription[index].y = star->point->Y;
            bodyDescription[index].z = star->point->Z;
            bodyDescription[index].w = star->mass;
            index++;

            if(*maxVelocity < star->GetVelocityLength()){
                *maxVelocity = star->GetVelocityLength();
            }
        }

        stars = galaxies[i]->bulge;
        size = galaxies[i]->bulgeSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            bodyDescription[index].x = star->point->X;
            bodyDescription[index].y = star->point->Y;
            bodyDescription[index].z = star->point->Z;
            bodyDescription[index].w = star->mass;
            index++;

            if(*maxVelocity < star->GetVelocityLength()){
                *maxVelocity = star->GetVelocityLength();
            }
        }

        stars = galaxies[i]->halo;
        size = galaxies[i]->haloSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];

            bodyDescription[index].x = star->point->X;
            bodyDescription[index].y = star->point->Y;
            bodyDescription[index].z = star->point->Z;
            bodyDescription[index].w = star->mass;
            index++;

            if(*maxVelocity < star->GetVelocityLength()){
                *maxVelocity = star->GetVelocityLength();
            }
        }
    }
    return bodyDescription;
}

///////////////////////////////// <PARTICLE STUFF> /////////////////////////////////////////

void render3(void)
{
    camera->Update();
    calculateFPS();

    system("clear");
    printf("FPS: %lf\n", fps);

	int particleIndex = 0;

	//fprintf(stderr, "render() 1\n");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 CameraPosition(glm::inverse(camera->ViewMatrix)[3]);
    glm::mat4 ViewProjectionMatrix = camera->ProjectionMatrix * camera->ViewMatrix;


    for (int i = 0; i < GALAXY_COUNT; i++)
    {
        int size;
        Star** stars;

        stars = galaxies[i]->disk;
        size = galaxies[i]->diskSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];
            particles->g_position_size_data[4*particleIndex+0] = star->point->X;
            particles->g_position_size_data[4*particleIndex+1] = star->point->Y;
            particles->g_position_size_data[4*particleIndex+2] = star->point->Z;

            particles->g_position_size_data[4*particleIndex+3] = 1.0f; // size

            particles->g_color_data[4*particleIndex+0] = 1.0f; // R
            particles->g_color_data[4*particleIndex+1] = 1.0f; // G
            particles->g_color_data[4*particleIndex+2] = 1.0f; // B

            particles->g_color_data[4*particleIndex+3] = 1.0f; // alpha

    		particleIndex++;
        }

        stars = galaxies[i]->bulge;
        size = galaxies[i]->bulgeSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];
            particles->g_position_size_data[4*particleIndex+0] = star->point->X;
            particles->g_position_size_data[4*particleIndex+1] = star->point->Y;
            particles->g_position_size_data[4*particleIndex+2] = star->point->Z;

            particles->g_position_size_data[4*particleIndex+3] = 1.0f;

            particles->g_color_data[4*particleIndex+0] = 1.0f; // R
            particles->g_color_data[4*particleIndex+1] = 1.0f; // G
            particles->g_color_data[4*particleIndex+2] = 1.0f; // B

            particles->g_color_data[4*particleIndex+3] = 1.0f; // alpha

    		particleIndex++;
        }

        stars = galaxies[i]->halo;
        size = galaxies[i]->haloSize;
        for (int j = 0; j < size; j++)
        {
            Star* star = stars[j];
            particles->g_position_size_data[4*particleIndex+0] = star->point->X;
            particles->g_position_size_data[4*particleIndex+1] = star->point->Y;
            particles->g_position_size_data[4*particleIndex+2] = star->point->Z;

            particles->g_position_size_data[4*particleIndex+3] = 1.0f;

            particles->g_color_data[4*particleIndex+0] = 1.0f; // R
            particles->g_color_data[4*particleIndex+1] = 1.0f; // G
            particles->g_color_data[4*particleIndex+2] = 1.0f; // B

            particles->g_color_data[4*particleIndex+3] = 1.0f; // alpha

    		particleIndex++;
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);

    glUniform3f(CameraRight_worldspace_ID, camera->ViewMatrix[0][0],
    					camera->ViewMatrix[1][0], camera->ViewMatrix[2][0]);
    glUniform3f(CameraUp_worldspace_ID , camera->ViewMatrix[0][1],
    		camera->ViewMatrix[1][1], camera->ViewMatrix[2][1]);

    glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

    //fprintf(stderr, "before drawarrays\n");

    particles->BindVBO();

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles->particleCount);

    particles->UnBindVBO();

    glutSwapBuffers();

    glutPostRedisplay();
}

void createParticles(int particleCount)
{
	fprintf(stderr, "createParticles 1\n");
	particles = new ParticlesCreator(particleCount);
	fprintf(stderr, "createParticles 2\n");
}

///////////////////////////////// </PARTICLE STUFF> /////////////////////////////////////////

void bindVBO(GLuint vertexbuffer, GLuint uvbuffer,
        GLuint elementbuffer, GLuint colorbuffer, int indiciesSize)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "textureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
	);
    /*
    if(colorbuffer != 0){
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
            );
    }
    else {
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
    }
    */
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
}

void unbindVBO()
{
    glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
}

void keyboard(unsigned char c, int x, int y)
{
    float slower = 25.0f;
    switch (c){
    case 'a':
        camera->position -= camera->right * camera->speed / slower;
        break;
    case 'd':
        camera->position += camera->right * camera->speed / slower;
        break;
    case 'w':
        camera->position += camera->direction * camera->speed / slower;
        break;
    case 's':
        camera->position -= camera->direction * camera->speed / slower;
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key){
    case GLUT_KEY_LEFT:
        camera->position -= camera->right * camera->speed;
        break;
    case GLUT_KEY_RIGHT:
        camera->position += camera->right * camera->speed;
        break;
    case GLUT_KEY_UP:
        camera->position += camera->direction * camera->speed;
        break;
    case GLUT_KEY_DOWN:
        camera->position -= camera->direction * camera->speed;
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {

        // when the button is released
        if (state == GLUT_UP) {
            //angle += deltaAngle;
            camera->xOrigin = -1;
            camera->yOrigin = -1;
        }
        else  {// state = GLUT_DOWN
            camera->xOrigin = x;
            camera->yOrigin = y;
        }
    }
}

void mouseMove(int x, int y)
{
    // this will only be true when the left button is down
    if (camera->xOrigin >= 0 && camera->yOrigin >= 0) {

        camera->horizontalAngle += (x - camera->xOrigin) * camera->mouseSpeed;
        camera->verticalAngle += (y - camera->yOrigin) * camera->mouseSpeed;

        camera->xOrigin = x;
        camera->yOrigin = y;
    }
}

void calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if (timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
}

int GFXInit(int argc, char** argv)
{
    glutInit(&argc, argv);

    initWindow();

    initEvents();

    initProgram();

    initSampleObjects();

    //galaxies = gllParse(argv[1], Texture, TextureID);
    galaxies = glxParseGalaxies("resources");

    createParticles(galaxies[0]->TotalSize() + galaxies[1]->TotalSize());

    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    glutMainLoop();

    delete[] galaxies;

    return EXIT_SUCCESS;
}

int initWindow()
{
    // GLUT Display
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Galaxy Collision");

    // GLEW init
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return -1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Black screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return 0;
}

int initEvents()
{
    // Glut function poitners
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);

    return 0;
}

int initProgram()
{
	CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
	CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(programID, "VP");

    // VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    fprintf(stderr, "Current WD: %s\n", get_current_dir_name());

    // Load shaders
    /*
    programID = LoadShaders("src/gfx/shaders/ParticleVertexShader.vert",
        		"src/gfx/shaders/ParticleFragmentShader.frag");
    */

    programID = LoadShaders("src/gfx/shaders/VertexShader.vert",
    		"src/gfx/shaders/FragmentShader.frag");


    /*
    programID = LoadShaders("src/gfx/shaders/TextureVertexShader.vert",
            "src/gfx/shaders/TextureFragmentShader.frag");
*/
    glUseProgram(programID);

    // Get a handle for our "MVP" uniform.
    MatrixID = glGetUniformLocation(programID, "MVP");

    // Load the texture using any two methods
    Texture = LoadBMP("resources/uvtemplate.bmp");
    //GLuint Texture = loadDDS("uvtemplate.DDS");

    // Get a handle for our "myTextureSampler" uniform
    TextureID  = glGetUniformLocation(programID, "textureSampler");

    fprintf(stderr, "ProgramID: %d, Texture: %d, TextureID: %d\n", programID, Texture, TextureID);

    return 0;
}

int initSampleObjects()
{
    // Objects
    //sphere = new Sphere(0, 0, 0, 0.002f);
    sphere = new Sphere(0, 0, 0);
    //sphere = new Sphere(0.5f, 0.5f, 0.5f, 0.002f);
    cube = new Cube();
    camera = new Camera(0.0f, 0.0f, 1.0f);

    return 0;
}
