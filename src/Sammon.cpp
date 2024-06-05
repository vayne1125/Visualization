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
    calc_ellipse();
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
    
    data.resize(dataNum,vector<double>(dimension));
    
    for(int i=0;i<dataNum;i++){
        for(int j=0;j<dimension;j++){
            inputFile >> data[i][j];
            if(j != dimension - 1) inputFile >> c;
        }
        this -> classNum = max(this -> classNum,(int)data[i].back());
    }
    this -> classNum += 1;
    cout << "dataNum/dimension/classNum = " << dataNum << "/" << dimension << "/" << classNum << "\n"; 

}
void Sammon::draw_ellipse(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(this->ellipse_VAO);
    
    glDrawArrays(GL_POINTS, 0, 2 + 180 * classNum);
    glBindVertexArray(0);
};
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
            for(int k=0; k<dimension-1; k++){
                oldDis[i][j] += (data[ID[i]][k] - data[ID[j]][k]) * (data[ID[i]][k] - data[ID[j]][k]);
            }
            oldDis[i][j] = sqrt(oldDis[i][j]); 
            oldDis[j][i] = oldDis[i][j];
        }
    }
    double diff = 0;
    double diff_ = 0;
    double eps = 1e-6;
    double delta = 1e-6;
    double lambda = 1.0f;
    double alpha = 0.5;

    srand( time(NULL) );
    
    // random init pos
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
    // cout << "diff: "<<diff << "\n";

    while(abs(diff - diff_ ) > eps){
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
void Sammon::calc_ellipse(){
    // implement PCA method
    // 參考資料：
    // https://blog.csdn.net/qq_52331099/article/details/125084412
    // https://medium.com/ai%E5%8F%8D%E6%96%97%E5%9F%8E/preprocessing-data-%E4%B8%BB%E6%88%90%E5%88%86%E5%88%86%E6%9E%90-pca-%E5%8E%9F%E7%90%86%E8%A9%B3%E8%A7%A3-afe1fd044d4f
    int N = vertexCnt;
    vector<vector<glm::dvec2>> classData(this->classNum);
    vector<glm::dvec2> mean(this->classNum);
    for(int i=0;i<points.size();i+=3){
        int class_ = points[i+2];
        classData[class_].push_back(glm::dvec2(points[i],points[i+1]));
        mean[class_].x += points[i];
        mean[class_].y += points[i+1];
    }
    // 找均值
    for(int i=0; i<this->classNum;i++){
        mean[i].x /= classData[i].size();
        mean[i].y /= classData[i].size();
    }

    // 方差矩陣
    vector<glm::mat2> covarianceMatrix(this->classNum);
    for(int i=0;i<this->classNum;i++){
        for(auto p: classData[i]){
            covarianceMatrix[i][0][0] += (p.x - mean[i].x) * (p.x - mean[i].x);
            covarianceMatrix[i][0][1] += (p.x - mean[i].x) * (p.y - mean[i].y);
            covarianceMatrix[i][1][0] += (p.x - mean[i].x) * (p.y - mean[i].y);
            covarianceMatrix[i][1][1] += (p.y - mean[i].y) * (p.y - mean[i].y);
        }
        covarianceMatrix[i] /= (classData[i].size() - 1);
        cout << "covarianceMatrix[" << i << "]: " << covarianceMatrix[i][0][0] << " " << covarianceMatrix[i][0][1] << " "<<covarianceMatrix[i][1][1]<< "\n";
    }

    // 計算 eigen value/ vector
    vector<vector<double>> eigenvalues(this->classNum);
    vector<vector<glm::dvec2>> eigenvectors(this->classNum);
    vector<glm::dvec2> radii(this->classNum);
    float scaleFactor = 2;
    for(int i=0;i<this->classNum;i++){
        eig2(covarianceMatrix[i],eigenvalues[i],eigenvectors[i]);
        cout << "eigen value[" << i << "]: " <<eigenvalues[i][0] << " " << eigenvalues[i][1] << "\n";
        cout << "eigen vector[" << i << "]: "  <<eigenvectors[i][0][0] << "," << eigenvectors[i][0][1] << "), (" << eigenvectors[i][1][0] << ", " << eigenvectors[i][1][1] << ")"<< "\n";
        // 計算橢圓參數
        glm::dvec2 radii(scaleFactor * sqrt(eigenvalues[i][0]), scaleFactor * sqrt(eigenvalues[i][1]));
        double angle = atan2(eigenvectors[i][0][1], eigenvectors[i][0][0]);
        make_ellipse(mean[i],radii,angle,i);
    }
}
void Sammon::make_ellipse(const glm::dvec2& center, const glm::dvec2& radii, double angle,int class_){
    ellipsePoints.push_back(center.x);
    ellipsePoints.push_back(center.y);
    ellipsePoints.push_back(2);
    int numSegments = 180;
    for (int i = 0; i < numSegments; i++) {
        double theta = 2.0f * PI * i / double(numSegments);
        double x = radii.x * cos(theta);
        double y = radii.y * sin(theta);
        // 旋轉
        double xRot = x * cos(angle) - y * sin(angle);
        double yRot = x * sin(angle) + y * cos(angle);
        ellipsePoints.push_back(center.x + xRot);
        ellipsePoints.push_back(center.y + yRot);
        ellipsePoints.push_back(2);
    }
}
// 計算2x2矩陣的特徵值和特徵向量
void Sammon::eig2(const glm::mat2& A, vector<double>& eigenvalues, std::vector<glm::dvec2>& eigenvectors) {
    double a = A[0][0];
    double b = A[0][1];
    double c = A[1][0];
    double d = A[1][1];
    
    // 計算特徵值
    eigenvalues.resize(2);
    eigenvalues[0] = (double)(((a + d) + sqrt((double)((a + d)*(a + d) - 4 * (a*d - b*c)))) / 2);
    eigenvalues[1] = (double)(((a + d) - sqrt((double)((a + d)*(a + d) - 4 * (a*d - b*c)))) / 2);
    
    if(eigenvalues[0] < eigenvalues[1]){
        swap(eigenvalues[0],eigenvalues[1]);
    }

    // 計算特徵向量
    eigenvectors.resize(2);
    if (b != 0) {
        eigenvectors[0] = glm::normalize(glm::dvec2(b, eigenvalues[0] - a));
        eigenvectors[1] = glm::normalize(glm::dvec2(b, eigenvalues[1] - a));
    } else {
        eigenvectors[0] = glm::dvec2(1, 0);
        eigenvectors[1] = glm::dvec2(0, 1);
    }
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
    // VAO
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

    // oriPoints_VAO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &oriPoints_VAO);
    glBindVertexArray(oriPoints_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oriPoints[0]) * oriPoints.size(), oriPoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));
    
    oriPoints.clear();

    // ellipse_VAO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &this->ellipse_VAO);
    glBindVertexArray(this->ellipse_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ellipsePoints[0]) * ellipsePoints.size(), ellipsePoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));
    
    ellipsePoints.clear();

}
