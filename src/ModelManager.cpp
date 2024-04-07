#include "./header/ModelManager.hpp"
ModelManager::ModelManager(const string& modelName, int isoLevel){
    this -> rotate = glm::vec3(0,0,0);
    this -> autoRY = 1;
    this -> fixedRY = glm::mat4(1.0f);
    init(modelName, isoLevel);
}
void ModelManager::init(const string& modelName, int isoLevel){
    if(modelName == "carp")
        modelID = CARP;
    else if(modelName == "engine")
        modelID = ENGINE;
    else if(modelName == "golfball")
        modelID = GOLFBALL;
    else if(modelName == "teddybear")
        modelID = TEDDYBEAR;
    
    
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
    
    add_volume(isoLevel);
    isoValueDistributed = volumeArray[0].data;

    volumnCnt = 1;
}

void ModelManager::add_volume(int isoLevel){
    if(volumeIsoValueArray[isoLevel]) return;
    volumeIsoValueArray[isoLevel] = 1;
    volumnCnt++;

    Volume voulme(infFile,rawFile,isoLevel);
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
    }
    volumeArray.clear();
    volumeIsoValueArray.assign(255,0);
}

glm::mat4 ModelManager::GetModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(1.0f, 0.0f, 1.0f));
    if(modelID == CARP)
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    else if(modelID == TEDDYBEAR){
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return model;
}

glm::mat4 ModelManager::GetFixedRYMatrix(){
    return fixedRY;
}

void ModelManager::updateFixedRY(){
    fixedRY = glm::rotate(fixedRY, glm::radians(0.75f), glm::vec3(0.0f, 1.0f, 0.0f));
}