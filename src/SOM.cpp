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
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // 給初值
    nodeSz = pair(16,16);
    nodeInitPos = NODE_INIT_POS::GRID;
    renderTo = RENDER_TO::LINES;
    fittingMesh = FITTING_MESH::CYLINDER;
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
        minX = fmin(minX, data[i][0]);
        maxX = fmax(maxX, data[i][0]);
        
        minY = fmin(minY, data[i][1]);
        maxY = fmax(maxY, data[i][1]);

        minZ = fmin(minZ, data[i][2]);
        maxZ = fmax(maxZ, data[i][2]);
    }
    // cout << data[dataNum-1].x << " "<< data[dataNum-1].y << " " << data[dataNum-1].z<<"\n";
    // 移到中間
    float centerX = minX + (maxX - minX) / 2, centerY =  minY + (maxY - minY) / 2, centerZ =  minZ + (maxZ - minZ) / 2;
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

    glm::vec3 range((maxX - minX), (maxY - minY), (maxZ - minZ));
    // cout << range.x << " " << range.y << " " << range.z << "\n";
    srand( time(NULL) );

    // random init weight
    if(nodeInitPos == NODE_INIT_POS::RANDOM){
        for(int i=0;i < this->nodeSz.first; i++){
            for(int j=0; j < this->nodeSz.second; j++){
                node[i][j] = glm::vec3((rand() % int(range.x) - range.x/2.0), (rand() % int(range.y) - range.y/2.0), (rand() % int(range.z) - range.z/2.0));
            }
        }
    }else if(nodeInitPos == NODE_INIT_POS::GRID){
        if(fittingMesh == FITTING_MESH::CYLINDER){
            float r = range.z / 2.0;
            float h = range.y;
            for(int i=0;i < this->nodeSz.first; i++){
                for(int j=0; j < this->nodeSz.second; j++){
                    // 高度
                    float y = (j / ((float)nodeSz.second - 1)) * h - range.y/2.0;

                    // 圓周
                    float theta = (i / (float)nodeSz.first) * 2 * PI;
                    float x = r * sin(theta);
                    float z = r * cos(theta);
                    node[i][j] = glm::vec3(x,y,z);
                }
            }
        }else if(fittingMesh == FITTING_MESH::PLANE){
            for(int i=0;i < this->nodeSz.first; i++){
                for(int j=0; j < this->nodeSz.second; j++){
                    node[i][j] = glm::vec3(range.x * ((float)i/nodeSz.first) - range.x/2.0, range.y  * ((float)j/nodeSz.second) - range.y/2.0, 0);
                }
            }
        }
    }else cout << "SOM.cpp: init_node(): NODE_INIT_POS error!\n";
}

void SOM::init(){
    // init node weight
    init_node();

    // init varible
    iterationsCnt = 0;
    learningRate = startLearningRate;
    startNeighbourhoodRadius = max(nodeSz.first,nodeSz.second)/4.0;
    neighbourhoodRadius = startNeighbourhoodRadius;
    this -> isDone = false;
    
    // test();
    set_VAO();
}

void SOM::train(int perIter = 1000){
    while(perIter--){
        // 每次隨機選一個資料(P)做計算
        int PIndex = ((float)rand()/ (RAND_MAX))* (dataNum-1);
        if(PIndex == dataNum){
            isDone = 1;
            cout << "SOM.cpp: error: PIndex over vec size\n";
            break;
        }
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
                double dis;
                if(fittingMesh == FITTING_MESH::CYLINDER) dis = sqrt(min((i-x)*(i-x) + (j-y)*(j-y),  (nodeSz.first - abs(i - x))*(nodeSz.first - abs(i - x)) + (j-y)*(j-y)));
                else if(fittingMesh == FITTING_MESH::PLANE) dis = sqrt((i-x)*(i-x) + (j-y)*(j-y));
                else cout << "ERROR::SOM.cpp: update_neighbor: unknowen fittingMesh\n";
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
    if(renderTo == RENDER_TO::LINES){
        vector<float> lines;
        if(fittingMesh == FITTING_MESH::CYLINDER){
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
        }else if(fittingMesh == FITTING_MESH::PLANE){
            for(int i=0; i<nodeSz.first; i++){
                for(int j=0; j<nodeSz.second; j++){
                    if(i != nodeSz.first-1){
                        lines.push_back(node[i][j].x);
                        lines.push_back(node[i][j].y);
                        lines.push_back(node[i][j].z);

                        lines.push_back(node[(i+1)% nodeSz.first][j].x);
                        lines.push_back(node[(i+1)% nodeSz.first][j].y);
                        lines.push_back(node[(i+1)% nodeSz.first][j].z);
                    }
                    // 往上
                    if(j != nodeSz.second-1) {
                        lines.push_back(node[i][j].x);
                        lines.push_back(node[i][j].y);
                        lines.push_back(node[i][j].z);

                        lines.push_back(node[i][(j+1)% nodeSz.second].x);
                        lines.push_back(node[i][(j+1)% nodeSz.second].y);
                        lines.push_back(node[i][(j+1)% nodeSz.second].z);
                    }
                }
            }
        }
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lines[0]) * lines.size(), lines.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(lines[0]), 0);

        vertexCnt = lines.size()/3;
    }else if(renderTo == RENDER_TO::SURFACES){
        vector<float> surfaces;
        int szx , szy ;
        if(fittingMesh == FITTING_MESH::CYLINDER){
            szx = nodeSz.first, szy = nodeSz.second;
        }else if(fittingMesh == FITTING_MESH::PLANE){
            szx = nodeSz.first - 1, szy = nodeSz.second;
        }
        for(int i=0; i<szx; i++){
            for(int j=1; j<szy; j++){
                /*-----------------
                    p1--p4                                          
                    |\   |
                    | \  |
                    |  \ |
                    |___\|
                    p2   p3
                ------------------*/    
                glm::vec3 p1 = node[i][j], p2 = node[i][j-1], p3 = node[(i+1)% nodeSz.first][j-1], p4 = node[(i+1)% nodeSz.first][j];
                glm::vec2 p1_st = glm::vec2(i,j), p2_st = glm::vec2(i,j-1), p3_st = glm::vec2((i+1),j-1), p4_st = glm::vec2((i+1),j);
                
                // p123
                glm::vec3 u = p2 - p1;
                glm::vec3 v = p3 - p1;
                glm::vec3 normal = glm::normalize(cross(u,v));
                
                // p1
                surfaces.push_back(p1.x);
                surfaces.push_back(p1.y);
                surfaces.push_back(p1.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p1_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p1_st[1]/nodeSz.second);

                // p2
                surfaces.push_back(p2.x);
                surfaces.push_back(p2.y);
                surfaces.push_back(p2.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p2_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p2_st[1]/nodeSz.second);

                // p3
                surfaces.push_back(p3.x);
                surfaces.push_back(p3.y);
                surfaces.push_back(p3.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p3_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p3_st[1]/nodeSz.second);


                // p134
                u = p3 - p1;
                v = p4 - p1;
                normal = glm::normalize(cross(u,v));
                
                // p1
                surfaces.push_back(p1.x);
                surfaces.push_back(p1.y);
                surfaces.push_back(p1.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p1_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p1_st[1]/nodeSz.second);

                // p3
                surfaces.push_back(p3.x);
                surfaces.push_back(p3.y);
                surfaces.push_back(p3.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p3_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p3_st[1]/nodeSz.second);

                // p4
                surfaces.push_back(p4.x);
                surfaces.push_back(p4.y);
                surfaces.push_back(p4.z);
                surfaces.push_back(normal.x);
                surfaces.push_back(normal.y);
                surfaces.push_back(normal.z);
                surfaces.push_back((float)p4_st[0]/nodeSz.first);
                surfaces.push_back(1 - (float)p4_st[1]/nodeSz.second);
            }
        }
        
            
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(surfaces[0]) * surfaces.size(), surfaces.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(surfaces[0]), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(surfaces[0]), (void*)(3 * sizeof(surfaces[0])));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(surfaces[0]), (void*)(6 * sizeof(surfaces[0])));
        vertexCnt = surfaces.size()/8;
    }else{
        cout << "SOM.cpp: set_VAO(): error: unknowen value \'renderTo\'\n";
    }
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

void SOM::setRenderTo(RENDER_TO r){
    renderTo = r;
    set_VAO();
}

void SOM::setFittingMesh(FITTING_MESH fm){
    fittingMesh = fm;
}

void SOM::draw(){

    glBindVertexArray(this->VAO);

    if(renderTo == RENDER_TO::LINES){
        glDrawArrays(GL_LINES, 0, vertexCnt);
    }else if(renderTo == RENDER_TO::SURFACES){
        glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    }else{
        cout << "SOM.cpp: set_VAO(): error: unknowen value \'renderTo\'\n";
    }
    glBindVertexArray(0);
}

int SOM::getIterations(){
    return iterations;
}

int SOM::getIterationsCnt(){
    return iterationsCnt;
}

float SOM::getLearningRate(){
    return learningRate;
}

float SOM::getStartLearningRate(){
    return startLearningRate;
}

float SOM::getStartNeighbourhoodRadius(){
    return startNeighbourhoodRadius;
}

float SOM::getNeighbourhoodRadius(){
    return neighbourhoodRadius;
}