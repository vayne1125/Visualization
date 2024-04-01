#include "./header/Volume.hpp"
extern int edgeTable[256]; 
extern int triTable[256][16];
Volume::Volume(){
    cout << "Volume.cpp\n";
}
Volume::Volume(string infFile,string rawFile, float isoLevel){
    cout << "Volume.cpp\n";
    this -> isoValue = isoLevel;
    read_inf(infFile);
    read_raw(rawFile);
    cal_gradient();
    calc_mesh(isoLevel);
    set_VAO();
    cout << "vertexCnt: " << vertexCnt << "\n";
}
void Volume::set_VAO(){
    //VAO VBO
    unsigned int VBO;
    // gen
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);     // 現在使用的VAO是誰

    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰

    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    mesh.clear();
    isoValueGrid.clear();
    gradient.clear();
    glBindVertexArray(0);
}
void Volume::draw(){
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    glBindVertexArray(0);
}
void Volume::read_raw(string file){
    std::ifstream inputFile(file, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return ;
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);

    inputFile.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    inputFile.close();

    isoValueGrid.resize(resolution[0],vector<vector<float>>(resolution[1],vector<float>(resolution[2])));

    data.resize(256);
    int index = 0;
    for(int i=0;i<resolution[0];i++){
        for(int j=0;j<resolution[1];j++){
            for(int k=0;k<resolution[2];k++){
                isoValueGrid[i][j][k] = buffer[index++];
                data[isoValueGrid[i][j][k]]++;
            }
        }
    }
}
void Volume::read_inf(string file){
    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cerr << "Unable to open file." << std::endl;
        return;
    }
    string line;
    while (getline(inputFile, line)){
        istringstream iss(line);
        string key, value;
        getline(iss, key, '=');
        getline(iss, value);
        if(key=="Resolution"){
            char separator;
            istringstream iss(value);
            iss >> resolution.z >> separator >> resolution.y >> separator >> resolution.x;
        }
    }
    cout << "resolution: " << resolution[0] << " " <<  resolution[1] << " "<<  resolution[2] << "\n";
}
void Volume::calc_mesh(float isoLevel){
    glm::vec3 grid[8];
    float val[8];
    glm::vec3 grad[8];
    float offsetX = resolution.x/2,  offsetY = resolution.y/2, offsetZ = resolution.z/2;
    // glm::vec3 normal;
    for(int i=0;i<resolution.x-1;i++){
        for(int j=0;j<resolution.y-1;j++){
            for(int k=0;k<resolution.z-1;k++){
                grid[0] = glm::vec3(i  , j  , k  );
                grid[1] = glm::vec3(i  , j  , k+1);
                grid[2] = glm::vec3(i  , j+1, k+1);
                grid[3] = glm::vec3(i  , j+1, k  );
                grid[4] = glm::vec3(i+1, j  , k  );
                grid[5] = glm::vec3(i+1, j  , k+1);
                grid[6] = glm::vec3(i+1, j+1, k+1);
                grid[7] = glm::vec3(i+1, j+1, k  );

                // grid[0] = glm::vec3(i  , j+1, k  );
                // grid[1] = glm::vec3(i+1, j+1, k  );
                // grid[2] = glm::vec3(i+1, j+1, k+1);
                // grid[3] = glm::vec3(i  , j+1, k+1);
                // grid[4] = glm::vec3(i  , j  , k  );
                // grid[5] = glm::vec3(i+1, j  , k  );
                // grid[6] = glm::vec3(i+1, j  , k+1);
                // grid[7] = glm::vec3(i  , j  , k+1);

                int tableIndex = 0;
                for(int i=0,tp=1;i<8;i++,tp<<=1){
                    val[i] = isoValueGrid[grid[i].x][grid[i].y][grid[i].z];
                    grad[i] = gradient[grid[i].x][grid[i].y][grid[i].z];
                    if (val[i] < isoLevel) 
                        tableIndex |= tp;
                }
                
                if (edgeTable[tableIndex] == 0) continue;

                glm::vec3 vertexList[12];
                glm::vec3 gradientList[12];
                int vid1[12] = {0,1,2,3,4,5,6,7,0,1,2,3};
                int vid2[12] = {1,2,3,0,5,6,7,4,4,5,6,7};

                for(int i=0,tp=1;i<12;i++,tp<<=1){
                    if (edgeTable[tableIndex] & tp){
                        vertexList[i] = calc_interpolation(isoLevel,grid[vid1[i]],grid[vid2[i]],val[vid1[i]],val[vid2[i]]);
                        // 用梯度法找法向量
                        gradientList[i] = calc_interpolation(isoLevel,grad[vid1[i]],grad[vid2[i]],val[vid1[i]],val[vid2[i]]);
                    }
                }
                // float offsetX =0,  offsetY = 0, offsetZ = 0;


                /* Create the triangle */
                // glm::vec3 nor;
                for (int i=0; triTable[tableIndex][i] != -1; i++) {
                    mesh.push_back(vertexList[triTable[tableIndex][i]].x-offsetX);
                    mesh.push_back(vertexList[triTable[tableIndex][i]].y-offsetY);
                    mesh.push_back(vertexList[triTable[tableIndex][i]].z-offsetZ);

                    // glm::vec3 normal;
                    // if(i%3 == 0)
                    // {
                    //     glm::vec3 a = vertexList[triTable[tableIndex][i+1]] - vertexList[triTable[tableIndex][i]];
                    //     glm::vec3 b = vertexList[triTable[tableIndex][i+2]] - vertexList[triTable[tableIndex][i]];
                    //     normal = glm::normalize(cross(a,b));
                    // }
                    // mesh.push_back(normal.x);
                    // mesh.push_back(normal.y);
                    // mesh.push_back(normal.z);
                    mesh.push_back(gradientList[triTable[tableIndex][i]].x);
                    mesh.push_back(gradientList[triTable[tableIndex][i]].y);
                    mesh.push_back(gradientList[triTable[tableIndex][i]].z);
                }
            }
        }
    }
    vertexCnt = mesh.size()/6;
}
glm::vec3 Volume::calc_interpolation(float isoLevel,glm::vec3 p1, glm::vec3 p2, float valp1, float valp2){
    float mu;
    glm::vec3 p;
    // if (abs(isoLevel-valp1) < 0.00001)
    //     return(p1);
    // if (abs(isoLevel-valp2) < 0.00001)
    //     return(p2);
    // if (abs(valp1-valp2) < 0.00001)
    //     return(p1);
    mu = (isoLevel - valp1) / (valp2 - valp1);
    // p = p1 + mu * (p2 - p1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);
    return p;
}
void Volume::delete_VAO(){
    cout << "IsoValue: " << (int)isoValue << " -> ";
    cout << "delete\n";
    glDeleteVertexArrays(1, &VAO);
}
void Volume::cal_gradient(){
    gradient.resize(resolution[0],vector<vector<glm::vec3>>(resolution[1],vector<glm::vec3>(resolution[2])));
    int N,M,K;
    N = resolution[0];
    M = resolution[1];
    K = resolution[2];
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            for(int k=0;k<K;k++){

                glm::vec3 grad;

                if(i==0){ // 前微
                    grad.x = isoValueGrid[i+1][j][k] - isoValueGrid[i][j][k];
                }else if(i==N-1){ // 後微
                    grad.x = isoValueGrid[i][j][k] - isoValueGrid[i-1][j][k];
                }else{ // 中微
                    grad.x = isoValueGrid[i+1][j][k] - isoValueGrid[i-1][j][k];
                    grad.x /= 2;
                }

                if(j==0){ // 前微
                    grad.y = isoValueGrid[i][j+1][k] - isoValueGrid[i][j][k];
                }else if(j==M-1){ // 後微
                    grad.y = isoValueGrid[i][j][k] - isoValueGrid[i][j-1][k];
                }else{ // 中微
                    grad.y = isoValueGrid[i][j+1][k] - isoValueGrid[i][j-1][k];
                    grad.y /= 2;
                }

                if(k==0){ // 前微
                    grad.z = isoValueGrid[i][j][k+1] - isoValueGrid[i][j][k];
                }else if(k==K-1){ // 後微
                    grad.z = isoValueGrid[i][j][k] - isoValueGrid[i][j][k-1];
                }else{ // 中微
                    grad.z = isoValueGrid[i][j][k+1] - isoValueGrid[i][j][k-1];
                    grad.z /= 2;
                }

                gradient[i][j][k] = grad;
            }
        }
    }
}