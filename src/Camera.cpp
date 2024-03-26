
#include "./header/Camera.hpp"
void Camera::ProcessMouseScroll(float yoffset){
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 130.0f)
        zoom = 130.0f;
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, target, up);
}

Camera::Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float fovy){
    std::cout << "Camera.cpp\n";
    up = upup;
    target = tar;
    position = pos;
    zoom = fovy;
}