#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "./constants.hpp"
enum RAW_DATA_TYPE{
    UNSIGNED_SHORT, UNSIGNED_CHAR
};
using namespace std;
class Volume{
public:
    Volume();
    void delete_VAO();
    void delete_slice_VAO();
    Volume(int method,string infFile, string rawFile, float isoLevel);
    Volume(int method,string infFile, string rawFile);
    void create_1dtexture(const vector<vector<float>>& RGBA);
    void cal_slice(int sliceNum);
    void draw();
    // void draw(glm::vec3 w);
    void draw(int v);
    vector<float> data;
    float maxMag = -1, minMag = 0x3f;
    float isoValue;
private:
    void read_inf(string file);
    template<typename T> void read_raw(string file);
    void calc_mesh(float isoLevel);
    void cal_gradient();
    void create_3dtexture();
    void create_1dtexture();
    void set_VAO();
    glm::vec3 calc_interpolation(float isoLevel,glm::vec3 p1, glm::vec3 p2, float valp1, float valp2);


    vector<vector<vector<float>>> isoValueGrid;
    vector<vector<vector<glm::vec3>>> gradient;
    glm::vec3 resolution;
    int rawDataType = 0;
    vector<float> mesh;
    int vertexCnt = 0;
    int minIsoValue = 0x3f, maxIsoValue = -1;
    unsigned int texture3DID, texture1DID;
    unsigned int VAO;
    vector<unsigned int> slice_VAO;
    int method;
};