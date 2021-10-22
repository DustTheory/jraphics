

#ifndef ENGINE1_LIB_LOGGL_H_
#define ENGINE1_LIB_LOGGL_H_

#define GL_LOG_FILE "logs/gl.log"

bool RestartGlLog();

bool GlLog(const char* message, ...);
bool GlLogErr(const char* message, ...);

void GlfwErrorCallback(int error, const char* description);

void LogGlParams();

void PrintShaderInfoLog(GLuint shader_index);

void PrintShaderProgramInfoLog(GLuint programme);

const char* GlTypeToString(GLenum type);

void PrintAll(GLuint programme);

#endif  // ENGINE1_LIB_LOGGL_H_