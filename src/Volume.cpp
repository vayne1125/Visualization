#include "./header/Volume.hpp"
// #include "./header/constants.hpp"
extern int edgeTable[256]; 
extern int triTable[256][16];
Volume::Volume(){
    cout << "Volume.cpp\n";
}
Volume::Volume(int method,string infFile,string rawFile, float isoLevel){
    cout << "METHODS::ISO_SURFACE: Volume.cpp\n";
    this -> method = method;
    
    this -> isoValue = isoLevel;
    read_inf(infFile);

    if(rawDataType == RAW_DATA_TYPE::UNSIGNED_CHAR)
        read_raw<unsigned char>(rawFile);
    else if(rawDataType == RAW_DATA_TYPE::UNSIGNED_SHORT)
        read_raw<unsigned short>(rawFile);
    
    cal_gradient();
    if(this->method == METHODS::ISO_SURFACE){
        calc_mesh(isoLevel);
    }
    set_VAO();
    cout << "vertexCnt: " << vertexCnt << "\n";
}

Volume::Volume(int method,string infFile,string rawFile){
    cout << "METHODS::VOLUME_RENDERING: Volume.cpp\n";
    this -> method = method;
    read_inf(infFile);

    if(rawDataType == RAW_DATA_TYPE::UNSIGNED_CHAR)
        read_raw<unsigned char>(rawFile);
    else if(rawDataType == RAW_DATA_TYPE::UNSIGNED_SHORT)
        read_raw<unsigned short>(rawFile);

    cal_gradient();
    create_3dtexture();
    create_1dtexture();
    cal_slice(512);
    // set_VAO();
    cout << "vertexCnt: " << vertexCnt << "\n";
}
void Volume::cal_slice(int sliceNum){
    this -> slice_VAO.assign(6,0);
    mesh.clear();
    
    // int sliceNum = 4096; //切片數
    this -> vertexCnt = 6 * sliceNum;
    float offsetX = resolution.x/2,  offsetY = resolution.y/2, offsetZ = resolution.z/2;
    int xMin = 0 - offsetX, xMax = resolution.x - offsetX;
    int yMin = 0 - offsetY, yMax = resolution.y - offsetY;
    int zMin = 0 - offsetZ, zMax = resolution.z - offsetZ;
    float x,y,z,s;
    // 延 z 軸切 back_to_front
    for(int i=sliceNum-1;i>=0;i--){
        z = (float)i/(float)sliceNum*(float)resolution.z - offsetZ;
        s = (z-zMin)/(zMax-zMin);
        // P0
        mesh.push_back(xMin); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(0); mesh.push_back(s);

        // P1
        mesh.push_back(xMax); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(0); mesh.push_back(s);

        // P2
        mesh.push_back(xMax); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(1); mesh.push_back(s);

        // P2
        mesh.push_back(xMax); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(1); mesh.push_back(s);

        // P3
        mesh.push_back(xMin); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(1); mesh.push_back(s);

        // P0
        mesh.push_back(xMin); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(0); mesh.push_back(s);
    }

    unsigned int VBO;
    glGenVertexArrays(1, &slice_VAO[0]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[0]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();

    // 延 z 軸切 front_to_back
    for(int i=0;i<sliceNum;i++){
        z = (float)i/(float)sliceNum*(float)resolution.z - offsetZ;
        s = (z-zMin)/(zMax-zMin);
        // P0
        mesh.push_back(xMin); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(0); mesh.push_back(s);

        // P1
        mesh.push_back(xMax); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(0); mesh.push_back(s);

        // P2
        mesh.push_back(xMax); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(1); mesh.push_back(s);

        // P2
        mesh.push_back(xMax); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(1); mesh.push_back(1); mesh.push_back(s);

        // P3
        mesh.push_back(xMin); mesh.push_back(yMax); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(1); mesh.push_back(s);

        // P0
        mesh.push_back(xMin); mesh.push_back(yMin); mesh.push_back(z);
        mesh.push_back(0); mesh.push_back(0); mesh.push_back(s);
    }
    glGenVertexArrays(1, &slice_VAO[1]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[1]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();
//-----------------------------------------------------------
// x
//-----------------------------------------------------------
    // 延 x 軸切 back_to_front
    for(int i=sliceNum-1;i>=0;i--){
        x = (float)i/(float)sliceNum*(float)resolution.x - offsetX;
        s = (x-xMin)/(xMax-xMin);
        // P0
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(0);

        // P1
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(0);

        // P2
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(1);

        // P2
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(1);

        // P3
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(1);

        // P0
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(0);
    }

    glGenVertexArrays(1, &slice_VAO[2]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[2]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();

    // 延 x 軸切 front_to_back
    for(int i=0;i<sliceNum;i++){
        x = (float)i/(float)sliceNum*(float)resolution.x - offsetX;
        s = (x-xMin)/(xMax-xMin);
        // P0
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(0);

        // P1
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(0);

        // P2
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(1);

        // P2
        mesh.push_back(x); mesh.push_back(yMax); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(1); mesh.push_back(1);

        // P3
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMax);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(1);

        // P0
        mesh.push_back(x); mesh.push_back(yMin); mesh.push_back(zMin);
        mesh.push_back(s); mesh.push_back(0); mesh.push_back(0);
    }
    glGenVertexArrays(1, &slice_VAO[3]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[3]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();

//-----------------------------------------------------------
// y
//-----------------------------------------------------------
    // 延 y 軸切 back_to_front
    for(int i=sliceNum-1;i>=0;i--){
        y = (float)i/(float)sliceNum*(float)resolution.y - offsetY;
        s = (y-yMin)/(yMax-yMin);
        // P0
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(0);

        // P1
        mesh.push_back(yMax); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(0);

        // P2
        mesh.push_back(yMax); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(1);

        // P2
        mesh.push_back(xMax); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(1);

        // P3
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(1);

        // P0
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(0);
    }

    glGenVertexArrays(1, &slice_VAO[4]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[4]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();

    // 延 y 軸切 front_to_back
    for(int i=0;i<sliceNum;i++){
        y = (float)i/(float)sliceNum*(float)resolution.y - offsetY;
        s = (y-yMin)/(yMax-yMin);
        // P0
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(0);

        // P1
        mesh.push_back(xMax); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(0);

        // P2
        mesh.push_back(xMax); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(1);

        // P2
        mesh.push_back(xMax); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(1); mesh.push_back(s); mesh.push_back(1);

        // P3
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMax);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(1);

        // P0
        mesh.push_back(xMin); mesh.push_back(y); mesh.push_back(zMin);
        mesh.push_back(0); mesh.push_back(s); mesh.push_back(0);
    }
    glGenVertexArrays(1, &slice_VAO[5]); 
    glGenBuffers(1, &VBO);
    glBindVertexArray(slice_VAO[5]);     // 現在使用的VAO是誰
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // 現在使用的VBO是誰
    glBufferData(GL_ARRAY_BUFFER,mesh.size() * sizeof(mesh[0]), mesh.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    mesh.clear();
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
    if(this->method == METHODS::ISO_SURFACE){
        // glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    }else if(this->method == METHODS::VOLUME_RENDERING){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, this->texture3DID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_1D, this->texture1DID);
        glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glBindVertexArray(this->VAO);
        // glDisable(GL_CULL_FACE);
        // glCullFace(GL_NONE);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        // glFrontFace(GL_CCW);
    }
    // glBindVertexArray(this->VAO);
    // glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    glBindVertexArray(0);
}
void Volume::draw(int v){
    // v決定了切片方向
    if(method != METHODS::VOLUME_RENDERING) cout << "Volume::draw: method dismatch!\n";
    glBindVertexArray(this->slice_VAO[v]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, this->texture3DID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(0);
}
template<typename T> void Volume::read_raw(string file){
    std::ifstream inputFile(file, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return ;
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<T> buffer(fileSize);

    inputFile.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    inputFile.close();

    isoValueGrid.resize(resolution[0],vector<vector<float>>(resolution[1],vector<float>(resolution[2])));

    data.resize(256);
    int index = 0;
    for(int i=0;i<resolution[0];i++){
        for(int j=0;j<resolution[1];j++){
            for(int k=0;k<resolution[2];k++){
                isoValueGrid[i][j][k] = buffer[index++];
                this->minIsoValue = min(this->minIsoValue,(int)isoValueGrid[i][j][k]);
                this->maxIsoValue = max(this->maxIsoValue,(int)isoValueGrid[i][j][k]);
                if(isoValueGrid[i][j][k] > 0)
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
        string key, value, tp;
        getline(iss, key, '=');
        getline(iss, value);
        char separator;
        
        if(key=="Resolution"){
            istringstream iss(value);
            iss >> resolution.z >> separator >> resolution.y >> separator >> resolution.x;
        }
        if(key == "Type"){
            istringstream iss(value);
            iss >> tp;
            if(tp == "unsigned_char")
                rawDataType = RAW_DATA_TYPE::UNSIGNED_CHAR;
            else if(tp == "unsigned_short")
                rawDataType = RAW_DATA_TYPE::UNSIGNED_SHORT;
            else {
                cerr << "Volume.cpp: read_inf: error raw data type!\n";
            }
        }
    }
    cout << "resolution: " << resolution[0] << " " <<  resolution[1] << " "<<  resolution[2] << "\n";
    cout << "data type: " << this->rawDataType << "\n";
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
    mu = (isoLevel - valp1) / (valp2 - valp1);
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
void Volume::delete_slice_VAO(){
    cout << "slice vao delete\n";
    for(int i=0;i<slice_VAO.size();i++){
        glDeleteVertexArrays(1, &this-> slice_VAO[i]);
    }
}
void Volume::create_3dtexture(){
    // vector<vector<vector<vector<unsigned char>>>> texture3DData;
    // texture3DData.assign(this->resolution.x,vector<vector<vector<unsigned char>>>(this->resolution.y,vector<vector<unsigned char>>(this->resolution.z,vector<unsigned char>(4,0))));
    

    int N,M,K;
    N = resolution[0];
    M = resolution[1];
    K = resolution[2];
    
    unsigned char (*texture3DData)[4] = new unsigned char[N * M * K][4];
    
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            for(int k=0;k<K;k++){
                float len = glm::length(this->gradient[i][j][k]);
                this -> maxMag = max(maxMag,len);
                this -> minMag = min(minMag,len);
                glm::vec3 grad;
                if(len) grad = glm::normalize(this->gradient[i][j][k]);
                else grad = this->gradient[i][j][k];
                texture3DData[k*N*M + j*N + i][0] = (grad[0]+1)/2*255;
                texture3DData[k*N*M + j*N + i][1] = (grad[1]+1)/2*255;
                texture3DData[k*N*M + j*N + i][2] = (grad[2]+1)/2*255;
                texture3DData[k*N*M + j*N + i][3] = this->isoValueGrid[i][j][k];//((this->isoValueGrid[i][j][k] - this->minIsoValue)/(float)(this->maxIsoValue - this->minIsoValue))*255;
                // if(texture3DData[k*N*M + j*N + i][3] == 0) cout << "0";
                // cout << grad[0] << " " <<grad[1] << " " << grad[2] << ' ';
            }
        }
    }
    glGenTextures(1, &this->texture3DID);
    glBindTexture(GL_TEXTURE_3D, this->texture3DID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexImage3D(
        GL_TEXTURE_3D,
        0,
        GL_RGBA,
        this->resolution.x,
        this->resolution.y,
        this->resolution.z,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texture3DData
    );
    isoValueGrid.clear();
    gradient.clear();
}
void Volume::create_1dtexture(){
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
            // float tpR = hk + 1/3.0, tpG = hk, tpB = hk-1/3.0;
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
            texture1DData[i][0] = RGB.r*255;
            texture1DData[i][1] = RGB.g*255;
            texture1DData[i][2] = RGB.b*255;
            texture1DData[i][3] = 0.05*255;
        }
    }
    glGenTextures(1, &this->texture1DID);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
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
void Volume::create_1dtexture(const vector<vector<float>>& RGBA){
    unsigned char (*texture1DData)[4] = new unsigned char[256][4];
    for(int i=0;i<256;i++){
        texture1DData[i][0] = RGBA[0][i]*255;
        texture1DData[i][1] = RGBA[1][i]*255;
        texture1DData[i][2] = RGBA[2][i]*255;
        texture1DData[i][3] = RGBA[3][i]*255;
    }
    glGenTextures(1, &this->texture1DID);
    glBindTexture(GL_TEXTURE_1D, this->texture1DID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
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
                // if(grad[0])
                //     cout << grad[0] << " " <<grad[1] << " " << grad[2] << ' ';
            }
        }
    }
}