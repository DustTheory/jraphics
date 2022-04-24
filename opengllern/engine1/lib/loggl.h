#include <GLFW/glfw3.h>

#ifndef ENGIN_E1_LIB_LOGGL_H_
#define ENGIN_E1_LIB_LOGGL_H

#define GL_LOG_FILE "gl.log"

bool RestartGlLog();

bool GlLog(const char* message, ...);
bool GlLogErr(const char* message, ...);

void GlfwErrorCallback(int error, const char* description);

void LogGlParams();

void PrintShaderInfoLog(GLuint shader_index);

void PrintShaderProgramInfoLog(GLuint programme);

const char* GlTypeToString(GLenum type);

void PrintAll(GLuint programme);

#endif  // ENGIN_E1_LIB_LOGGL_H_
