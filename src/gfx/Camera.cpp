#include "Camera.h"

Camera::Camera()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;

    init();
}

Camera::Camera(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;

    init();
}

void Camera::init()
{
    position = glm::vec3(x, y, z);

    // horizontal angle : toward -Z
    horizontalAngle = 3.14f;

    // vertical angle : 0, look at the horizon
    verticalAngle = 0.0f;

    // Initial Field of View
    initialFoV = 45.0f;

    speed = 0.9f; // 3 units / second

    mouseSpeed = 0.001f;

    xOrigin = -1;
    yOrigin = -1;

    Update();
}

void Camera::Update()
{
	//position = glm::vec3(x, y, z);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    direction = glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector : perpendicular to both direction and right
    up = glm::cross(right, direction);

    // Get FoV
    float FoV = initialFoV;

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.0001f, 1000.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        // Camera is here
        position,
        // and looks here : at the same position, plus "direction"
        position + direction,                   
        // Head is up (set to 0,-1,0 to look upside-down)
        up
    );
}
