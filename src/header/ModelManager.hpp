#include "./Volume.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class ModelManager{
public:
    vector<Volume> volumeArray;
    ModelManager(const string& infFile,const string& rawFile);
    void init(const string& infFile,const string& rawFile);
    void add_volume(float isoLevel);
    void updateFixedRY();

    glm::mat4 GetModelMatrix();
    glm::mat4 GetFixedRYMatrix();
    vector<float> isoValueDistributed;
    glm::vec3 rotate;
    bool autoRY;
    // glm::mat4 fixedRY;

private:
    string infFile, rawFile;
    glm::mat4 fixedRY;
    void update();
};