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
    void draw_ellipse(int);
    void test();
    int dataNum, dimension, classNum;
private:
    unsigned int VAO, oriPoints_VAO;
    vector<unsigned int> ellipse_VAO;
    // 讀入的資料
    vector<vector<double>> data;
    void eig2(const glm::mat2& A, vector<double>& eigenvalues, std::vector<glm::dvec2>& eigenvectors);
    void set_VAO();
    void make_ellipse(const glm::dvec2&  center, const glm::dvec2&  radii, double angle, int class_);
    void read_data(string file);
    void calc_2d_point(int N);
    void calc_ellipse();
    int vertexCnt = 0, ellipseVertexCnt = 0;
    vector<double> points, oriPoints;
    vector<vector<double>> ellipsePoints;
};