#ifndef ENGINE1_LIB_LOAD_SHADER_FILE_H_
#define ENGINE1_LIB_LOAD_SHADER_FILE_H_

#include <GL/glew.h>
#include <string>

namespace Engine1 {
    std::string loadShaderFile(const char *filePath);
}

#endif  // ENGINE1_LIB_LOAD_SHADER_FILE_H_