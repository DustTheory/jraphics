#include "../lib/load_shader_file.h"

// Stolen from:
// https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../lib/loggl.h"


#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::string Engine1::loadShaderFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
