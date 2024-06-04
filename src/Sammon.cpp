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
}
Sammon::Sammon(){
    cout << "Sammon.cpp -------- test --------\n";
    test();
}
Sammon::~Sammon(){

}
// Sammon::void set_cycle_VAO(){
//     float rad = 2;
//     vector<float> cycle;
//     for(int i=0;i<360;i++){
//         cycle.push_back(0.0f);
//         cycle.push_back(0.0f);

//         cycle.push_back(rad * cos(2*PI*i/n));
//         cycle.push_back(rad * sin(2*PI*i/n));

//         cycle.push_back(rad * cos(2*PI*(i+1)/n));
//         cycle.push_back(rad * sin(2*PI*(i+1)/n));
//     }
//     unsigned int VBO;
//     glGenBuffers(1, &VBO);
//     glGenVertexArrays(1, &cycle_VAO);
//     glBindVertexArray(cycle_VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
// }
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
void Sammon::draw(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(this->VAO);
    
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
    vector<glm::dvec2> pos(N, glm::dvec2(0,0));
    for(int i=0;i<N;i++){
        pos[i] = glm::dvec2(i/N,i/N);
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

                // cout <<  qi.x << " " << qi.y << " ";
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
        // cout << diff << " ";
        lambda = alpha * lambda;
    }

    for(int i=0;i<N;i++){
        points.push_back(pos[i].x);
        points.push_back(pos[i].y);
        cout << pos[i].x << " "<< pos[i].y << "\n";
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));
}
