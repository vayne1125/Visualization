
#include "./header/Camera.hpp"
void Camera::ProcessMouseScroll(float yoffset){
    if(projectionMethod == PROJECTION_METHODS::PERSPECTIVE){
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 130.0f)
            zoom = 130.0f;
    }else if(projectionMethod == PROJECTION_METHODS::ORTHO){
        this -> offset -= yoffset*5;
        if(this -> offset <= -256) this -> offset = -256;
    }else{
        std::cout << "ERROR: PROJECTION_METHODS not found!\n";
    }
}
glm::mat4 Camera::get_view_matrix(){
    return glm::lookAt(position, target, up);
}
glm::mat4 Camera::get_projection_matrix(){
    if(this->projectionMethod == PROJECTION_METHODS::PERSPECTIVE){
        return glm::perspective(glm::radians(zoom),(float)screenW / (float)screenH, 0.001f, 10000.0f);
    }else if(this->projectionMethod == PROJECTION_METHODS::ORTHO){
        if(screenW > screenH)
            return glm::ortho( -256 - offset, 256 + offset, (-256 - offset)*((float) screenH / screenW), (256 + offset) * ((float) screenH / screenW), 0.001f, 10000.0f);
        else
            return glm::ortho( (-256 - offset)*((float) screenW / screenH), (256 + offset)*((float) screenW / screenH), -256 - offset, 256 + offset, 0.001f, 10000.0f);
    }
}
void Camera::set_screen_wh(int width,int heigth){
    this->screenH = heigth;
    this->screenW = width;
}
void Camera::set_projection_method(PROJECTION_METHODS projectionMethod){
    this->projectionMethod = projectionMethod;
}
Camera::Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float fovy){
    std::cout << "Camera.cpp\n";
    up = upup;
    target = tar;
    position = pos;
    zoom = fovy;
    screenW = 800;
    screenH = 600;
}