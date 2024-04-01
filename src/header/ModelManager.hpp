#include "./Volume.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class ModelManager{
public:
    vector<bool> volumeIsoValueArray; 
    vector<Volume> volumeArray;
    ModelManager(const string& infFile,const string& rawFile, int isoLevel);
    void init(const string& infFile,const string& rawFile,int isoLevel);
    void add_volume(int isoLevel);
    void updateFixedRY();
    void delete_volume(int isoLevel);
    glm::mat4 GetModelMatrix();
    glm::mat4 GetFixedRYMatrix();
    vector<float> isoValueDistributed;
    glm::vec3 rotate;
    bool autoRY;

private:
    string infFile, rawFile;
    glm::mat4 fixedRY;
    void update();
};