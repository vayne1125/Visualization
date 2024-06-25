#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "./constants.hpp"
#include "std_image.h"

enum TEXTURE{
    WHITE, TEST, CHEESE, RAINBOW_CLOUD, YELLOW_MARBLE    
};

class Texture{
public:
	void use();
	Texture(const string& fname);
    Texture(int r,int g,int b);
private:
	unsigned int textureID;
};


class TextureManager{
public: 
    TextureManager();
    void useByID(TEXTURE);
private:
    Texture* white;
    Texture* test, *cheese, *rainbowCloud, *yellowMarble;
};