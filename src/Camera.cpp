
#include "./header/Camera.hpp"
void Camera::ProcessMouseScroll(float yoffset){
    if(projectionMethod == PROJECTION_METHODS::PERSPECTIVE){
        zoom -= (float)yoffset * sensitivity;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 130.0f)
            zoom = 130.0f;
    }else if(projectionMethod == PROJECTION_METHODS::ORTHO){
        this -> offset -= yoffset * sensitivity;
        // std::cout << offset << "\n";
        if(this -> offset <= -256) this -> offset = -255;
    }else{
        std::cout << "ERROR: PROJECTION_METHODS not found!\n";
    }
}
void Camera::set_sensitivity(float s){
    this -> sensitivity = s;
}
void Camera::set_ortho_offset(float of){
    this -> offset = of;
}
void Camera::ProcessKeyDown2D(int key){
    // std::cout << "in2\n";

    float delta = 0.5;
    if(key == GLFW_KEY_A){
        position.x += delta;
        target.x += delta;
    }else if(key == GLFW_KEY_D){
        position.x -= delta;
        target.x -= delta;
    }else if(key == GLFW_KEY_W){
        position.y += delta;
        target.y += delta;
    }else if(key == GLFW_KEY_S){
        position.y -= delta;
        target.y -= delta;
    }
}
void Camera::ProcessKeyDown3D(int key){
        std::cout << "in\n";
    float w = 10;
    if(key == GLFW_KEY_A){
        polarAngle += w * 0.1f;
        // std::cout << azimuthAngle << "\n";
    }else if(key == GLFW_KEY_D){
        polarAngle -= w * 0.1f;
    }else if(key == GLFW_KEY_W){
        azimuthAngle += w * 0.1f;
    }else if(key == GLFW_KEY_S){
        azimuthAngle -= w * 0.1f;
    }

//  std::cout << azimuthAngle << "\n";
    if(azimuthAngle < -89.0) azimuthAngle = -89.0;
    if(azimuthAngle > 89.0) azimuthAngle = 89.0;

    if(polarAngle > 360.0 ) polarAngle -= 360.0;
    update();
}
void Camera::update(){
    position.x = radialDistance*cos(glm::radians(azimuthAngle)) * sin(glm::radians(polarAngle));
    position.y = radialDistance*sin(glm::radians(azimuthAngle));
    position.z =  radialDistance*cos(glm::radians(azimuthAngle)) * cos(glm::radians(polarAngle));
}
void Camera::reset(){
    polarAngle = 180.0f;
    azimuthAngle = 0.0f;
    offset = 0.0;
    sensitivity = 5;
    position.x = position.y = 0;
    target.x = target.y = 0;
    update();
}
void Camera::reset(METHODS method){
    if(method == METHODS::SAMMON_MAPPING){
        polarAngle = 180.0f;
        azimuthAngle = 0.0f;
        position.x = position.y = 0;
        target.x = target.y = 0;
        offset = -254;
        sensitivity = 1;
    }else 
        reset();
    update();
}
void Camera::set_position(glm::vec3 pos){

    this->position = pos;
    // return glm::lookAt(position, target, up);
}
glm::mat4 Camera::get_view_matrix(){
    
    return glm::lookAt(position, target, up);
}
glm::mat4 Camera::get_projection_matrix(){
    if(this->projectionMethod == PROJECTION_METHODS::PERSPECTIVE){
        return glm::perspective(glm::radians(zoom),(float)screenW / (float)screenH, 0.001f, 10000.0f);
    }else if(this->projectionMethod == PROJECTION_METHODS::ORTHO){
        if(screenW > screenH)
            return glm::ortho( 
                -256 - offset                              ,  256 + offset, 
                (-256 - offset)*((float) screenH / screenW), (256 + offset) * ((float) screenH / screenW),
                0.0001f                                    , 10000.0f);
        else
            return glm::ortho( 
                (-256 - offset)*((float) screenW / screenH) , (256 + offset)*((float) screenW / screenH), 
                -256 - offset                               ,  256 + offset, 
                0.0001f                                     , 10000.0f);
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