

#ifndef LOG_GL_H
#define LOG_GL_H

#define GL_LOG_FILE "logs/gl.log"

bool restart_gl_log();

bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);

void glfw_error_callback(int error, const char* description);


void glfw_window_size_callback(GLFWwindow* window, int width, int height);

void log_gl_params();

void _print_shader_info_log(GLuint shader_index);

void _print_programme_info_log(GLuint programme);

const char* GL_type_to_string(GLenum type);

void print_all(GLuint programme);

#endif