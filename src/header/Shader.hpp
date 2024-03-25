#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class Shader{
public:
    Shader(const string &vertexPath, const string &fragmentPath);
    Shader();
    unsigned int ID;
    // activate the shader
    void use();
    // utility uniform functions
    void set_uniform(const string &name, bool value) const;
    void set_uniform(const string &name, int value) const;
    void set_uniform(const string &name, float value) const;
    void set_uniform(const string &name, glm::vec3 value) const;
    void set_uniform(const string &name, glm::vec4 value) const;
    void set_uniform(const string &name, glm::mat4 value) const;
    
private:
    void init(string vertFilePath, string fragFilePath);
    void checkCompileErrors(unsigned int shader, std::string type);
};