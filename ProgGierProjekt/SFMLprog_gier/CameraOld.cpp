#include "CameraOld.h"

CameraOld::CameraOld() :
    cameraPosition(getRandomCameraPosition()),
    cameraFront(glm::vec3(0.0f, 0.0f, 0.0f)),
    cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    //prepare model view and projection
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPosition, cameraFront, cameraUp);

    projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    std::cout << "\nCamera random position: \n" << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;

}
CameraOld::~CameraOld(){}

glm::vec3 CameraOld::getRandomCameraPosition() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.0f, 5.0f);
    std::uniform_real_distribution<float> yDist(0.0f, 5.0f);
    std::uniform_real_distribution<float> zDist(0.0f, 5.0f);

    float x = xDist(gen);
    float y = yDist(gen);
    float z = zDist(gen); 

    return glm::vec3(x, y, z);
}
