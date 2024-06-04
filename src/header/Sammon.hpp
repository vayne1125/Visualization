#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "./constants.hpp"

using namespace std;
class Sammon{
public:
    Sammon(string file,int N);
    Sammon();
    ~Sammon();
    void draw(MODE);
    void test();
    int dataNum, dimension;
private:
    unsigned int VAO, oriPoints_VAO;
    vector<unsigned int> animation_VAO;
    // 讀入的資料
    vector<vector<double>> data;
    void set_VAO();
    void read_data(string file);
    void calc_2d_point(int N);
    int vertexCnt = 0;
    vector<double> points, oriPoints;
};