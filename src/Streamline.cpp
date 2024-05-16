#include "./header/Streamline.hpp"
Streamline::Streamline(string file,double h, double density, double gap, int pointsThreshold1, int pointsThreshold2){
    cout << "Streamline.cpp\n";

    string dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/asset/Vector/";
    #else
        dir = "D:\\school\\Visualization\\src\\asset\\Vector\\";
    #endif

    read_vec(dir + file);

    // double h, double density, double pointsThreshold1, double pointsThreshold2
    cal_streamline(h, density, gap,pointsThreshold1, pointsThreshold2);
    set_VAO();
    create_1dtexture();
}
Streamline::~Streamline(){
    cout << "free Streamline\n";
    glDeleteVertexArrays(1, &this->VAO);
}
void Streamline::draw(){
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    
    glDrawArrays(GL_LINES, 0, vertexCnt);
    glBindVertexArray(0);
}
void Streamline::test(){
    float points[] = {
        -50.0f,  50.0f, // top-left
        50.0f,  50.0f,  // top-right
        50.0f, -50.0f,  // bottom-right
        -50.0f, -50.0f  // bottom-left
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
}
void Streamline::read_vec(string file){
    std::ifstream inputFile(file);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return ;
    }

    inputFile >> resolution.x >> resolution.y ;
    cout << "res:" << resolution.x << " " << resolution.y << "\n"; 
    
    data.resize(resolution.x,vector<glm::dvec2>(resolution.y));
    
    for(int i=0;i<resolution[0];i++){
        for(int j=0;j<resolution[1];j++){
            inputFile >> data[i][j].x >> data[i][j].y;
        }
    }
}
glm::dvec2 Streamline::calc_vec_interpolation(glm::dvec2 pos){
    double dx = pos.x - (int)pos.x;
    double dy = pos.y - (int)pos.y;

    int x = pos.x;
    int y = pos.y;

    double xx = data[x][y].x * (1-dx) * (1-dy) + 
                data[x+1][y].x * dx * (1-dy) + 
                data[x][y+1].x * (1-dx) * dy + 
                data[x+1][y+1].x * dx * dy;

    double yy = data[x][y].y * (1-dx) * (1-dy) + 
                data[x+1][y].y * dx * (1-dy) + 
                data[x][y+1].y * (1-dx) * dy + 
                data[x+1][y+1].y * dx * dy;

    return glm::dvec2(xx,yy);
}
// 只畫點數量在 pointsThreshold1 ~ pointsThreshold2 之間的
// h: streamline 每次計算點的位移量
// density: 疏密度，越大線越密
void Streamline::cal_streamline(double h, double density, double gap, int pointsThreshold1, int pointsThreshold2){
    vector<vector<bool>> vis(this->resolution.x * density,vector<bool>(this->resolution.y * density));
    int newResolutionX = this->resolution.x * density;
    int newResolutionY = this->resolution.y * density;
    double offsetX = resolution.x/2.0, offsetY = resolution.y/2.0;
    maxMagnitude = 0;
    minMagnitude = 0x3f;
    for(double i=0; i < this->resolution.x; i += gap){
        for(double j=0; j < this->resolution.y; j+= gap){
            glm::dvec2 pos(i,j);
            vector<pair<int,int>> visPoint;
            vector<float> magnitude;
            int cnt = 0;
            // 最多 pointsThreshold2 次
            for(int k=0; k<pointsThreshold2; k++){
                if(pos.x < 0 || pos.x >= resolution.x - 1 || pos.y < 0 || pos.y >= resolution.y - 1) break;
                if(pos.x * density > newResolutionX || pos.y * density > newResolutionY) break;
                if(vis[int(pos.x * density)][int(pos.y * density)]) break;
                visPoint.push_back({int(pos.x * density),int(pos.y * density)});
                
                // RK2-method
                glm::dvec2 K1 = calc_vec_interpolation(pos);
                glm::dvec2 midPos = pos + h * glm::normalize(K1);

                if(midPos.x < 0 || midPos.x >= resolution.x - 1 || midPos.y < 0 || midPos.y >= resolution.y - 1) break;
                
                glm::dvec2 K2 = calc_vec_interpolation(midPos);
                glm::dvec2 newPos = pos + (h/2.0)*(glm::normalize(K1)+glm::normalize(K2));

                if(newPos.x < 0 || newPos.x >= resolution.x - 1 || newPos.y < 0 || newPos.y >= resolution.y - 1) break;
                if(newPos.x * density > newResolutionX || newPos.y * density > newResolutionY) break;
                if(vis[int(newPos.x * density)][int(newPos.y * density)]) break;

                lines.push_back(pos.x - offsetX); 
                lines.push_back(pos.y - offsetY);
                lines.push_back(glm::length(K1));

                magnitude.push_back(glm::length(K1));

                lines.push_back(newPos.x - offsetX); 
                lines.push_back(newPos.y - offsetY);
                K1 = calc_vec_interpolation(newPos);
                lines.push_back(glm::length(K1));

                magnitude.push_back(glm::length(K1));
                
                cnt++;
                pos = newPos;
            }         

            if(cnt < pointsThreshold1){
                for(int k=0; k < cnt * 2 * 3; k++){
                    lines.pop_back();
                }
            }else{
                for(auto k: visPoint){
                    vis[k.first][k.second] = 1;
                }
                for(auto k: magnitude){
                    maxMagnitude = fmax(k,maxMagnitude);
                    minMagnitude = fmin(k,minMagnitude);
                }
            }
        }
    }
    // cout << minMagnitude << " " << maxMagnitude << "\n";
    vertexCnt = lines.size()/3;
    cout << "vertexCnt: " << vertexCnt << "\n";
}
void Streamline::set_VAO(){
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,lines.size() * sizeof(lines[0]), lines.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,  GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)(2 * sizeof(double)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}
void Streamline::create_1dtexture(){
    unsigned char (*texture1DData)[4] = new unsigned char[256][4];
    for(int i=0;i<256;i++){
        {
            // HSV to RGB
            float h =  256-i;
            float s =  1.0;
            float l =  0.5;

            float q = l + s - (l*s);
            float p = 2 * l - q;
            float hk = h/360.0;
            glm::vec3 tpRGB = glm::vec3(hk + 1/3.0, hk, hk-1/3.0);
            glm::vec3 RGB;

            for(int i=0;i<3;i++){
                if(tpRGB[i] < 0) tpRGB[i] += 1.0;
                else if(tpRGB[i] > 1) tpRGB[i] -= 1.0; 
            }
            for(int i=0;i<3;i++){
                if(tpRGB[i] < 1/6.0) RGB[i] = p+(q-p)*6*tpRGB[i];
                else if(1/6.0 <= tpRGB[i] && tpRGB[i] < 1/2.0) RGB[i] = q;
                else if(1/2.0 <= tpRGB[i] && tpRGB[i] < 2/3.0) RGB[i] = p + (q-p)*6*(2/3.0 - tpRGB[i]);
                else RGB[i] = p;
            }
            texture1DData[i][0] = 255;//RGB.r*255;
            texture1DData[i][1] = 255;//RGB.g*255;
            texture1DData[i][2] = 255;//RGB.b*255;
            texture1DData[i][3] = 255;
        }
    }
    glGenTextures(1, &this->texture1DID);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_RGBA,
        256,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texture1DData
    );
}
void Streamline::create_1dtexture(const vector<vector<float>>& RGBA){
    unsigned char (*texture1DData)[4] = new unsigned char[256][4];
    for(int i=0;i<256;i++){
        texture1DData[i][0] = RGBA[0][i]*255;
        texture1DData[i][1] = RGBA[1][i]*255;
        texture1DData[i][2] = RGBA[2][i]*255;
        texture1DData[i][3] = 255;
    }
    glGenTextures(1, &this->texture1DID);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_RGBA,
        256,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texture1DData
    );
}