#include "./header/Sammon.hpp"
Sammon::Sammon(string file,int N){
    cout << "Sammon.cpp\n";
    
    string dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/asset/";
    #else
        dir = "D:\\school\\Visualization\\src\\asset\\";
    #endif

    read_data(dir + file);
    calc_2d_point(N);
    set_VAO();

    // test();
}
Sammon::Sammon(){
    cout << "Sammon.cpp -------- test --------\n";
    test();
}
Sammon::~Sammon(){
    cout << "free Sammon\n";
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteVertexArrays(1, &this->oriPoints_VAO);
}
void Sammon::read_data(string file){
    std::ifstream inputFile(file);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return ;
    }
    char c;
    inputFile >> dataNum >> c >> dimension ;
    cout << "dataNum/dimension = " << dataNum << "/" << dimension << "\n"; 
    
    data.resize(dataNum,vector<double>(dimension));
    
    for(int i=0;i<dataNum;i++){
        for(int j=0;j<dimension;j++){
            inputFile >> data[i][j];
            if(j != dimension - 1) inputFile >> c;
        }
        // cout << data[i].back();
    }
}
void Sammon::draw(MODE mode){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(mode == MODE::BEFORE_CALC)
        glBindVertexArray(this->oriPoints_VAO);
    else if(mode == MODE::AFTER_CALC)
        glBindVertexArray(this->VAO);
    else cout << "ERROR: void Sammon.cpp::draw()\n";
    
    glDrawArrays(GL_POINTS, 0, this->vertexCnt);
    glBindVertexArray(0);
}
void Sammon::calc_2d_point(int N){
    // random N points
    vector<int> ID(this->dataNum);
    iota(ID.begin(),ID.end(),0);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(ID.begin(),ID.end(), std::default_random_engine(seed) );
    
    // implement sammon mapping
    vector<vector<double>> oldDis(N, vector<double>(N,0));
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            for(int k=0; k<dimension; k++){
                oldDis[i][j] += (data[ID[i]][k] - data[ID[j]][k]) * (data[ID[i]][k] - data[ID[j]][k]);
            }
            oldDis[i][j] = sqrt(oldDis[i][j]); 
            oldDis[j][i] = oldDis[i][j];
        }
    }
    float diff = 0x3f3f3f3f;
    float diff_ = 0;
    float eps = 1e-6;
    float delta = 1e-6;
    float lambda = 1.0f;
    float alpha = 0.3;

    srand( time(NULL) );
    
    vector<glm::dvec2> pos(N, glm::dvec2(0,0));
    for(int i=0;i<N;i++){
        pos[i] = glm::dvec2((rand()%2000 - 1000)/10000.0 * 10, (rand()%2000 - 1000)/10000.0 * 10);
    }

    oriPoints.clear();
    for(int i=0;i<N;i++){
        oriPoints.push_back(pos[i].x);
        oriPoints.push_back(pos[i].y);
        oriPoints.push_back(data[ID[i]].back());
    }

    // compute diff sum
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            diff += sqrt((pos[i].x - pos[j].x) * (pos[i].x - pos[j].x) +  (pos[i].y - pos[j].y) * (pos[i].y - pos[j].y));
        }
    }
    cout << diff << "\n";

    while(abs(diff - diff_ ) > eps){
        // cout << "in ";
        diff_ = diff;
        diff = 0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(i == j) continue;
                // compute new dis
                double dis = sqrt((pos[i].x - pos[j].x) * (pos[i].x - pos[j].x) +  (pos[i].y - pos[j].y) * (pos[i].y - pos[j].y));
                if(dis == 0.0) dis = delta;
                
                // compute descent direction
                glm::dvec2 qi = lambda * ( oldDis[i][j] - dis ) / dis * (pos[i] - pos[j]);
                glm::dvec2 qj = -qi;

                // update
                pos[i] += qi;
                pos[j] += qj;
            }
        }

        // compute diff sum
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                diff += sqrt((pos[i].x - pos[j].x) * (pos[i].x - pos[j].x) +  (pos[i].y - pos[j].y) * (pos[i].y - pos[j].y));
            }
        }
        // cout << diff << "\n";
        lambda = alpha * lambda;
    }

    float maxX = -0X3f, minX = 0x3f, maxY = -0X3f, minY = 0X3f;
    for(auto i: pos){
        maxX = fmax(maxX,i.x);
        minX = fmin(minX,i.x);
        maxY = fmax(maxY,i.y);
        minY = fmin(minY,i.y);
    }
    glm::dvec2 center = glm::dvec2(minX + (maxX-minX)/2.0,minY + (maxY-minY)/2.0);
    points.clear();
    for(int i=0;i<N;i++){
        points.push_back(pos[i].x-center.x);
        points.push_back(pos[i].y-center.y);
        points.push_back(data[ID[i]].back());
    }

    this->vertexCnt =  points.size()/3;
}

void Sammon::test(){
    double points[] = {
        -50.0, 50.0, 0.0, // top-left
        50.0,  50.0,  1.0, // top-right
        50.0, -50.0,  1.0, // bottom-right
        -50.0, -50.0, 0.0 // bottom-left
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));

    vertexCnt = 4;
}
void Sammon::set_VAO(){
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));

    points.clear();

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &oriPoints_VAO);
    glBindVertexArray(oriPoints_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oriPoints[0]) * oriPoints.size(), oriPoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));

}
