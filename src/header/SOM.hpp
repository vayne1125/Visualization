#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "./constants.hpp"

using namespace std;

class SOM{
public:
    bool isDone = false;
    SOM(string file);
    SOM();
    ~SOM();
    int getIterations();
    int getIterationsCnt();
    void draw();
    void train(int);
    void setNodeSz(int x, int y);
    void setNodeInitPos(NODE_INIT_POS);
    void init();
private:
    // const
    const float startLearningRate = 0.01;
    float startNeighbourhoodRadius;
    const int iterations = 500000;

    float learningRate;
    int iterationsCnt = 0;
    float neighbourhoodRadius;

    NODE_INIT_POS nodeInitPos;

    unsigned int points_VAO, lines_VAO, points_VBO, lines_VBO;
    int dataNum, dimension;
    pair<int,int> nodeSz;
    int minX, maxX , minY ,maxY , minZ ,maxZ;
    int lines_vertexCnt, points_vertexCnt;
    vector<glm::vec3> data;
    vector<vector<glm::vec3>> node;
    void read_file(string file);
    void init_node();
    void set_VAO();
    void test();
    pair<int,int> find_BMU(int);
    void update_neighbor(int, const pair<int,int>&);
};
