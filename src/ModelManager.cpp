#include "./header/ModelManager.hpp"
ModelManager::ModelManager(const string& infFile,const string& rawFile){
    this -> rotate = glm::vec3(0,0,0);
    this -> autoRY = 1;
    this -> fixedRY = glm::mat4(1.0f);
    init(infFile,rawFile);
}
void ModelManager::init(const string& infFile,const string& rawFile){
    this -> infFile = infFile;
    this -> rawFile = rawFile;
    isoValueDistributed.clear();
    volumeArray.clear();
    add_volume(200);
    isoValueDistributed = volumeArray[0].data;
}
void ModelManager::add_volume(float isoLevel){
    Volume voulme(infFile,rawFile,isoLevel);
    volumeArray.push_back(voulme);
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