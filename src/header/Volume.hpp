#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<bits/stdc++.h>
#include <glm/glm.hpp>
using namespace std;
class Volume{
public:
    Volume();
    Volume(string infFile, string rawFile, float isoLevel);
    void draw();
private:
    void read_inf(string file);
    void read_raw(string file);
    void calc_mesh(float isoLevel);
    void cal_gradient();
    void set_VAO();
    glm::vec3 calc_interpolation(float isoLevel,glm::vec3 p1, glm::vec3 p2, float valp1, float valp2);

    vector<vector<vector<float>>> isoValueGrid;
    vector<vector<vector<glm::vec3>>> gradient;
    glm::vec3 resolution;
    // iso-value 的分布
    vector<float> data;
    vector<float> mesh = {
    -0.5f, 0.0f, -0.5f, 0 , 1, 0, 
     0.5f,  0.0f, -0.5f, 0,1,0,
     0.0f,  0.0f, 0.5f, 0,1,0
};
    int vertexCnt = 0;
    unsigned int VAO;
};