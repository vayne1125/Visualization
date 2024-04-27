#define GLM_ENABLE_EXPERIMENTAL
#include "./Volume.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "./constants.hpp"
enum MODEL_TYPE{
    CARP, ENGINE, GOLFBALL, TEDDYBEAR
};
using namespace std;
class ModelManager{
public:
    vector<bool> volumeIsoValueArray; 
    vector<Volume> volumeArray;
    ModelManager(int method,const string& modelName, int isoLevel);
    ModelManager(int method,const string& modelName);
    
    void init(int method, const string& modelName,int isoLevel = -1);
    void add_volume(int isoLevel);
    void updateFixedRY();
    void delete_volume(int isoLevel);
    void delete_all_volume();
    float getRotationY();
    glm::mat4 get_model_matrix();
    glm::mat4 get_fixedRY_matrix();
    vector<float> isoValueDistributed;
    glm::vec3 rotate;
    bool autoRY;
    int openPhong;
    float rotateY;
    int volumnCnt;
    int method;
private:
    int modelID;
    string infFile, rawFile;
    glm::mat4 fixedRY;
    void update();
};