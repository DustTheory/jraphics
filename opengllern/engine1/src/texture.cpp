#include "../lib/texture.h"

#include "stb_image.h"

#include <GLFW/glfw3.h>

engine1::Texture::Texture(GLenum texture_target, const std::string &filename){
    texture_target_ = texture_target;
    texture_filename_ = filename;
}

bool engine1::Texture::Load(){
    stbi_set_flip_vertically_on_load(1);
    int width = 0;
    int height = 0;
    int bpp = 0;
    unsigned char* image_data = stbi_load(texture_filename_.c_str(), &width, &height, &bpp, 0);
    if(image_data == nullptr){
        GlLog("Failed to load texture from %s - %s", texture_filename_.c_str(), stbi_failure_reason());
        return false;
    }
    GlLog("Successfully loaded texture %s", texture_filename_.c_str());

    glGenTextures(1, &texture_object_);
    glBindTexture(texture_target_, texture_object_);
    glTexImage2D(texture_target_, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    
    glTexParameterf(texture_target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(texture_target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(texture_target_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(texture_target_, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(texture_target_, 0);
    return true;
}

void engine1::Texture::Bind(GLenum texture_unit) const {
    glActiveTexture(texture_unit);
    glBindTexture(texture_target_, texture_object_);
}