#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float zoom;
    float polarAngle,azimuthAngle;
    float mouseSensitivity;
    float r;
    Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 upup, float _r);

    glm::mat4 GetViewMatrix();

    void ProcessMouseMovement(float xoffset, float yoffsete);

    void ProcessMouseScroll(float yoffset);

private:
    void update();
};
