#include "./Volume.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
enum MODEL_TYPE{
    CARP, ENGINE, GOLFBALL, TEDDYBEAR
};
using namespace std;
class ModelManager{
public:
    vector<bool> volumeIsoValueArray; 
    vector<Volume> volumeArray;
    ModelManager(const string& modelName, int isoLevel);
    void init(const string& modelName,int isoLevel);
    void add_volume(int isoLevel);
    void updateFixedRY();
    void delete_volume(int isoLevel);
    void delete_all_volume();
    glm::mat4 GetModelMatrix();
    glm::mat4 GetFixedRYMatrix();
    vector<float> isoValueDistributed;
    glm::vec3 rotate;
    bool autoRY;
    int volumnCnt;
private:
    int modelID;
    string infFile, rawFile;
    glm::mat4 fixedRY;
    void update();
};