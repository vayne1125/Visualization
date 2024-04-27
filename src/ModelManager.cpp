#include "./header/ModelManager.hpp"
ModelManager::ModelManager(int method,const string& modelName, int isoLevel){
    this -> rotate = glm::vec3(0,0,0);
    this -> autoRY = 0;
    this -> fixedRY = glm::mat4(1.0f);
    this -> rotateY = 0;
    this -> method = method;
    this -> openPhong = 0;
    init(method, modelName, isoLevel);
}
ModelManager::ModelManager(int method,const string& modelName){
    cout << "METHODS::VOLUME_RENDERING: ModelManager.cpp\n";
    this -> rotate = glm::vec3(0,0,0);
    this -> autoRY = 0;
    this -> rotateY = 0;
    this -> fixedRY = glm::mat4(1.0f);
    this -> method = method;
    this -> openPhong = 0;
    init(method, modelName);
}
void ModelManager::init(int method, const string& modelName, int isoLevel){    
    this->method = method;
    if(modelName == "carp")
        modelID = MODEL_TYPE::CARP;
    else if(modelName == "engine")
        modelID = MODEL_TYPE::ENGINE;
    else if(modelName == "golfball")
        modelID = MODEL_TYPE::GOLFBALL;
    else if(modelName == "teddybear")
        modelID = MODEL_TYPE::TEDDYBEAR;
    
    
    string dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/asset/";
    #else
        dir = "D:\\school\\Visualization\\src\\asset\\";
    #endif
    
    this -> infFile = dir + modelName + ".inf";
    this -> rawFile = dir + modelName + ".raw";
    
    delete_all_volume();
    isoValueDistributed.clear();
    if(this->method == METHODS::ISO_SURFACE){
        if(isoLevel == -1){
            cout << "ERROR: isoLevel = -1\n";
        }
        add_volume(isoLevel);
        isoValueDistributed = volumeArray[0].data;
    }else if(this->method == METHODS::VOLUME_RENDERING){
        Volume voulme(this->method, infFile,rawFile);
        volumeArray.push_back(voulme);
        isoValueDistributed = volumeArray[0].data;
    }
    volumnCnt = 1;
}

void ModelManager::add_volume(int isoLevel){
    if(volumeIsoValueArray[isoLevel]) return;
    volumeIsoValueArray[isoLevel] = 1;
    volumnCnt++;

    Volume voulme(this->method, infFile,rawFile,isoLevel);
    // voulme.data.clear();

    volumeArray.push_back(voulme);
}

void ModelManager::delete_volume(int isoLevel){
    if(!volumeIsoValueArray[isoLevel]) return;
    volumnCnt--;
    for(auto it = volumeArray.begin(); it != volumeArray.end(); ++it){
        if(abs(it->isoValue - isoLevel) < 0.0001){
            it->delete_VAO();
            volumeArray.erase(it);
            break;
        }
    }
    volumeIsoValueArray[isoLevel] = 0;
}

void ModelManager::delete_all_volume(){
    for(auto it = volumeArray.begin(); it != volumeArray.end(); ++it){
        it->delete_VAO();
        it->delete_slice_VAO();
    }
    volumeArray.clear();
    volumeIsoValueArray.assign(255,0);
}

glm::mat4 ModelManager::get_model_matrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(1.0f, 0.0f, 1.0f));
    if(modelID == MODEL_TYPE::CARP)
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    else if(modelID == MODEL_TYPE::TEDDYBEAR){
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return model;
}
glm::mat4 ModelManager::get_fixedRY_matrix(){
    return fixedRY;
}
float ModelManager::getRotationY(){
    return rotateY;
}
void ModelManager::updateFixedRY(){
    if(this -> autoRY){
        rotateY += 0.75;
        if(rotateY >= 360)
            rotateY -= 360;
    }
    // glm::mat4(1);
    fixedRY = glm::rotate(glm::mat4(1.0f), glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
    // fixedRY = glm::rotate(fixedRY, glm::radians(0.75f), glm::vec3(0.0f, 1.0f, 0.0f));
}