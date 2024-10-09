#include "./header/TextureManager.hpp"

void Texture::use(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::Texture(const string& fname){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int image_width, image_height, nrChannels;
    unsigned char* image_data;
    image_data = stbi_load(fname.c_str(), &image_width, &image_height, &nrChannels, 0);
    cout << fname << " " << image_width << " " <<image_width << " " << nrChannels << "\n";
    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }
}

Texture::Texture(int r,int g,int b){
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char color[4] = { r,g,b,255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
}


TextureManager::TextureManager(){
    white = new Texture(255,255,255);

    string dir = ASSET_DIR;
    #if defined(__linux__) || defined(__APPLE__)
        dir += "Image/";
    #else
        dir += "Image\\";
    #endif

    test = new Texture(dir + "yellow_marble.jpg");

    cheese = new Texture(dir + "cheese.jpg");
    rainbowCloud = new Texture(dir + "rainbow_cloud.png");
    yellowMarble = new Texture(dir + "yellow_marble.jpg");


}

void TextureManager::useByID(TEXTURE texID){
    if(texID == TEXTURE::WHITE){
        white->use();
    }else if(texID == TEXTURE::TEST){
        test->use();
    }else if(texID == TEXTURE::CHEESE){
        cheese->use();
    }else if(texID == TEXTURE::RAINBOW_CLOUD){
        rainbowCloud->use();
    }else if(texID == TEXTURE::YELLOW_MARBLE){
        yellowMarble->use();
    }else cout << "error: TextureManager::useByID unknowen TEXTURE\n";
}