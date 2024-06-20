#include "./header/SOM.hpp"

SOM::SOM(string file){
    cout << "SOM.cpp\n";
    
    string dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/asset/Surface/";
    #else
        dir = "D:\\school\\Visualization\\src\\asset\\Surface\\";
    #endif

    read_file(dir + file);

    // init VAO VBO
    glGenBuffers(1, &points_VBO);
    glGenBuffers(1, &lines_VBO);
    glGenVertexArrays(1, &points_VAO);
    glGenVertexArrays(1, &lines_VAO);

    // 給初值
    nodeSz = pair(16,16);
    nodeInitPos = NODE_INIT_POS::GRID;

    // init train varible & VAO、VBO
    init();
}

SOM::~SOM(){

}

void SOM::read_file(string file){
     std::ifstream inputFile(file);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return ;
    }
    inputFile >> dataNum >> dimension ;
    
    data.clear();
    data.resize(dataNum);
    minX = 0x3f,maxX = -0x3f, minY = 0x3f,maxY = -0x3f, minZ = 0x3f,maxZ = -0x3f;
    for(int i=0;i<dataNum;i++){
        inputFile >> data[i][0] >> data[i][1] >> data[i][2];
        minX = min(minX, (int)data[i][0]);
        maxX = max(maxX, (int)data[i][0]);
        
        minY = min(minY, (int)data[i][1]);
        maxY = max(maxY, (int)data[i][1]);

        minZ = min(minZ, (int)data[i][2]);
        maxZ = max(maxZ, (int)data[i][2]);
    }
    // cout << data[dataNum-1].x << " "<< data[dataNum-1].y << " " << data[dataNum-1].z<<"\n";
    // 移到中間
    int centerX = minX + (maxX - minX) / 2, centerY =  minY + (maxY - minY) / 2, centerZ =  minZ + (maxZ - minZ) / 2;
    for(int i=0;i<dataNum;i++){
        data[i][0] -= centerX;
        data[i][1] -= centerY;
        data[i][2] -= centerZ;
    }

    cout << "dataNum/dimension = " << dataNum << "/" << dimension << "\n"; 
}

void SOM::init_node(){
    node.clear();
    node.resize(nodeSz.first,vector<glm::vec3>(nodeSz.second));

    glm::vec3 center(minX + (maxX - minX) / 2,minY + (maxY - minY) / 2,minZ + (maxZ - minZ) / 2);
    glm::vec3 range((maxX - minX), (maxY - minY), (maxZ - minZ));

    srand( time(NULL) );

    // random init weight
    for(int i=0;i < this->nodeSz.first; i++){
        for(int j=0; j < this->nodeSz.second; j++){
            if(nodeInitPos == NODE_INIT_POS::RANDOM)
                node[i][j] = glm::vec3((rand() % int(range.x) - center.x), (rand() % int(range.y) - center.y), (rand() % int(range.z) - center.z));
            else if(nodeInitPos == NODE_INIT_POS::GRID)
                node[i][j] = glm::vec3(range.x * ((float)i/nodeSz.first) - center.x, range.y  * ((float)j/nodeSz.second) - center.y, 0);
            else cout << "SOM.cpp: init_node(): NODE_INIT_POS error!\n";
            
            // node[i][j] = glm::vec3((rand() % int(range.x)), (rand() % int(range.y)), (rand() % int(range.z)));
            // node[i][j] = glm::vec3((rand() % int(range.x) - center.x), (rand() % int(range.y) - center.y), 0);

        }
    }
}

void SOM::init(){
    // init node weight
    init_node();

    // init varible
    iterationsCnt = 0;
    learningRate = startLearningRate;
    startNeighbourhoodRadius = max(nodeSz.first,nodeSz.second)/4.0;
    this -> isDone = false;
    
    // test();
    set_VAO();
}

void SOM::train(int perIter = 1000){
    while(perIter--){
        // 每次隨機選一個資料(P)做計算
        int PIndex = (float)rand() / RAND_MAX * dataNum;

        // 計算 winner node(Best Matching Unit/BMU)
        pair<int,int> BMUIndex = find_BMU(PIndex);

        // update neighbor weight
        update_neighbor(PIndex, BMUIndex);

        this -> iterationsCnt++;
        if(this -> iterationsCnt % 5000 == 0) set_VAO();
        if(this -> iterationsCnt >= this -> iterations) {
            this -> isDone = true;
        }
    }
    // cout << iterationsCnt << " ";
    // set_VAO();
    
    // 修改 lr [lr = startLr * e ^ -(it/maxIt)]
    learningRate = startLearningRate * exp(-(double)iterationsCnt/iterations);
    // neighbourhoodRadius  *= exp(-(double)iterationsCnt/iterations);
    neighbourhoodRadius = startNeighbourhoodRadius * (double)(iterations-iterationsCnt)/iterations * (double)(iterations-iterationsCnt)/iterations;
    neighbourhoodRadius = fmax(1,neighbourhoodRadius);
    // cout << neighbourhoodRadius << "\n";
    
    // cout << (double)(iterations-iterationsCnt)/iterations << " ";
}

pair<int,int> SOM::find_BMU(int PIndex){
    
    double minDiff = 0; 
    vector<pair<int,int>> BMU_candidate;

    for(int k=0; k < this->dimension; k++)
        minDiff += (node[0][0][k] - data[PIndex][k]) * (node[0][0][k] - data[PIndex][k]);
    

    // 計算 data 和 node(weight) 的差值 
    for(int i=0; i< nodeSz.first; i++){
        for(int j=0; j< nodeSz.second; j++){
            double diff = 0;

            // 每個 node 和 P 算
            for(int k=0; k < this->dimension; k++)
                diff += (node[i][j][k] - data[PIndex][k]) * (node[i][j][k] - data[PIndex][k]);

            if(diff == minDiff){
                BMU_candidate.push_back({i,j});
            }else if(diff < minDiff){
                BMU_candidate.clear();
                BMU_candidate.push_back({i,j});
            }

            minDiff = min(minDiff,diff);
        }
    }
    // cout << diff << "\n";
    // cout << "sz: " << BMU_candidate.size() << "\n";
    return BMU_candidate[(rand() % (int)BMU_candidate.size())];
}

void SOM::update_neighbor(int PIndex, const pair<int,int>& BMU){
    // cout << BMU.first << " " << BMU.second << "\n";
    int x = BMU.first;
    int y = BMU.second;
    for(int i=0; i<nodeSz.first ; i++){
        for(int j=0; j<nodeSz.second ; j++){
            if( i == x && j == y){
                node[i][j] += learningRate * (data[PIndex] - node[i][j]);
            }else{
                // double dis = sqrt((i-x)*(i-x) + (j-y)*(j-y));
                double dis = sqrt(min((i-x)*(i-x) + (j-y)*(j-y),  (nodeSz.first - abs(i - x))*(nodeSz.first - abs(i - x)) + (j-y)*(j-y)));
                // double disx = min((i-x), nodeSz.first - abs(i - x));
                // double disy = min((j-y), nodeSz.first - abs(j - y));
                // double dis = sqrt(disx * disx + disy * disy);
                
                // if(dis <= neighbourhoodRadius) node[i][j] += learningRate * (float)exp(-dis/2.0/neighbourhoodRadius/neighbourhoodRadius) * (data[PIndex] - node[i][j]);
                if(dis <= neighbourhoodRadius) node[i][j] += learningRate * (data[PIndex] - node[i][j]);
            }
        }
    }
}

// test render
void SOM::test(){
    for(int i=0; i<nodeSz.first; i++){
        for(int j=0; j<nodeSz.second; j++){
            node[i][j] = glm::vec3(rand()%75,i*5,j*5);
        }
    }
}

void SOM::set_VAO(){
    
    // vector<float> points;
    // for(int i=0; i<nodeSz.first; i++){
    //     for(int j=0; j<nodeSz.second; j++){
    //         points.push_back(node[i][j].x);
    //         points.push_back(node[i][j].y);
    //         points.push_back(node[i][j].z);
    //     }
    // }
    // glBindVertexArray(points_VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(), points.data(), GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(points[0]), 0);


    vector<float> lines;
    for(int i=0; i<nodeSz.first; i++){
        for(int j=0; j<nodeSz.second; j++){

            lines.push_back(node[i][j].x);
            lines.push_back(node[i][j].y);
            lines.push_back(node[i][j].z);

            lines.push_back(node[(i+1)% nodeSz.first][j].x);
            lines.push_back(node[(i+1)% nodeSz.first][j].y);
            lines.push_back(node[(i+1)% nodeSz.first][j].z);
          
            // 往上
            if(j == nodeSz.second-1) continue;
            lines.push_back(node[i][j].x);
            lines.push_back(node[i][j].y);
            lines.push_back(node[i][j].z);

            lines.push_back(node[i][(j+1)% nodeSz.second].x);
            lines.push_back(node[i][(j+1)% nodeSz.second].y);
            lines.push_back(node[i][(j+1)% nodeSz.second].z);
        }
    }

    glBindVertexArray(lines_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, lines_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines[0]) * lines.size(), lines.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(lines[0]), 0);

    lines_vertexCnt = lines.size()/3;
    // points_vertexCnt = points.size()/3;
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));
    
}

void SOM::setNodeSz(int x,int y){
    nodeSz = pair(x,y);
}

void SOM::setNodeInitPos(NODE_INIT_POS iip){
    nodeInitPos = iip;
}

void SOM::draw(){

    // glBindVertexArray(this->points_VAO);
    // glDrawArrays(GL_POINTS, 0, points_vertexCnt);
    
    glBindVertexArray(this->lines_VAO);
    glDrawArrays(GL_LINES, 0, lines_vertexCnt);
    glBindVertexArray(0);
}

int SOM::getIterations(){
    return iterations;
}

int SOM::getIterationsCnt(){
    return iterationsCnt;
}