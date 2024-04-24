#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
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
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float _r);

    void set_projection_method(PROJECTION_METHODS projectionMethod);
    void set_screen_wh(int width,int heigth);
    glm::mat4 get_projection_matrix();
    glm::mat4 get_view_matrix();

    void ProcessMouseScroll(float yoffset);

private:
};
