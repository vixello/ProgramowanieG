#pragma once
#include "glm/glm.hpp" // vec2, vec3, mat4, radians
#include "glm/ext.hpp" // perspective, translate, rotate
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <random>
#include <iostream>

class Camera
{
public:
    Camera();
    ~Camera();

    glm::vec3 getRandomCameraPosition();
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
};

