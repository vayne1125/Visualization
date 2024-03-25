
#include "./header/Camera.hpp"
void Camera::ProcessMouseScroll(float yoffset){
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 100.0f)
        zoom = 100.0f;
}
void Camera::ProcessMouseMovement(float xoffset, float yoffset){
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    polarAngle += xoffset;

    azimuthAngle += yoffset;
 
    update();
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, target, up);
}

Camera::Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float _r = 150){
    up = upup;
    target = tar;
    position = pos;
    r = _r;
    zoom = 100;
    polarAngle = 0, azimuthAngle = 0;
    mouseSensitivity = 0.01f;
    update();
}

void Camera::update(){
    position.x = r*sin(polarAngle)*cos(azimuthAngle);
    position.y = r*sin(polarAngle)*sin(azimuthAngle);
    position.z = r*cos(polarAngle);
}