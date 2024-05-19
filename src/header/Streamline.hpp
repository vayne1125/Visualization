#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "./constants.hpp"

using namespace std;
class Streamline{
public:
    Streamline(string file,double h, double density, double gap, int pointsThreshold1, int pointsThreshold2);
    ~Streamline();
    void read_vec(string file);
    void draw();
    void test();
    float maxMagnitude, minMagnitude;
    void create_1dtexture(const vector<vector<float>>& RGBA);
    glm::vec2 get_resolution();
private:
    unsigned int VAO;
    // 讀入的資料
    vector<vector<glm::dvec2>> data;
    glm::vec2 resolution;

    glm::dvec2 calc_vec_interpolation(glm::dvec2 pos);
    void cal_streamline(double , double , double, int , int);
    void set_VAO();
    void create_1dtexture();
    unsigned int texture1DID;
    int vertexCnt = 0;
    vector<double> lines;
};