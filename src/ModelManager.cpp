#include "./header/ModelManager.hpp"
ModelManager::ModelManager(const string& infFile,const string& rawFile, int isoLevel){
    this -> rotate = glm::vec3(0,0,0);
    this -> autoRY = 1;
    this -> fixedRY = glm::mat4(1.0f);
    volumeIsoValueArray.assign(255,0);
    init(infFile,rawFile, isoLevel);
}
void ModelManager::init(const string& infFile,const string& rawFile, int isoLevel){
    this -> infFile = infFile;
    this -> rawFile = rawFile;
    isoValueDistributed.clear();
    volumeArray.clear();
    add_volume(isoLevel);
    isoValueDistributed = volumeArray[0].data;
}
void ModelManager::add_volume(int isoLevel){
    if(volumeIsoValueArray[isoLevel]) return;
    volumeIsoValueArray[isoLevel] = 1;
    
    Volume voulme(infFile,rawFile,isoLevel);
    // voulme.data.clear();

    volumeArray.push_back(voulme);
}

void ModelManager::delete_volume(int isoLevel){
    if(!volumeIsoValueArray[isoLevel]) return;
    for(auto it = volumeArray.begin(); it != volumeArray.end(); ++it){
        if(abs(it->isoValue - isoLevel) < 0.0001){
            it->delete_VAO();
            volumeArray.erase(it);
            break;
        }
    }
    volumeIsoValueArray[isoLevel] = 0;
}

glm::mat4 ModelManager::GetModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(1.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    return model;
}

glm::mat4 ModelManager::GetFixedRYMatrix(){
    return fixedRY;
}

void ModelManager::updateFixedRY(){
    fixedRY = glm::rotate(fixedRY, glm::radians(0.75f), glm::vec3(0.0f, 1.0f, 0.0f));
}