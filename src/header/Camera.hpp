#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#include "./constants.hpp"

class Camera{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    PROJECTION_METHODS projectionMethod;
    float zoom;
    float offset = 0.0;
    int screenW, screenH;
    float polarAngle = 180.0f;
    float azimuthAngle = 0.0f;
    float radialDistance = 300.0f;
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float _r);

    void set_projection_method(PROJECTION_METHODS projectionMethod);
    void set_screen_wh(int width,int heigth);
    glm::mat4 get_projection_matrix();
    glm::mat4 get_view_matrix();
    void set_position(glm::vec3 pos);
    void ProcessMouseScroll(float yoffset);
    void ProcessKeyDown(int key);
    void reset();
    void update();
private:
};
