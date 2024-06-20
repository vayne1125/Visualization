#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

#include "./constants.hpp"

class Camera{
public:
    glm::vec3 position, startPos;
    glm::vec3 target, startTar;
    glm::vec3 up;
    PROJECTION_METHODS projectionMethod;
    float zoom;
    float offset = 0.0;
    float sensitivity = 5;
    int screenW, screenH;
    float polarAngle = 0.0f;
    float azimuthAngle = 0.0f;
    float radialDistance = 300.0f;
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float _r);

    void set_ortho_offset(float of);
    void set_sensitivity(float s);

    void set_projection_method(PROJECTION_METHODS projectionMethod);
    void set_screen_wh(int width,int heigth);
    glm::mat4 get_projection_matrix();
    glm::mat4 get_view_matrix();
    void set_position(glm::vec3 pos);
    void ProcessMouseScroll(float yoffset);
    void ProcessKeyDown3D(int key);
    void ProcessKeyDown2D(int key);
    void reset();
    void reset(METHODS method);
    void update();
private:
};
