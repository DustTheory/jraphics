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

std::string engine1::LoadShaderFile(const char *file_path) {
    std::string content;
    std::ifstream file_stream(file_path, std::ios::in);

    if(!file_stream.is_open()) {
        std::cerr << "Could not read file " << file_path << ". File does not exist." << std::endl;
        return "";
    }

    std::string line;
    while(!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}
