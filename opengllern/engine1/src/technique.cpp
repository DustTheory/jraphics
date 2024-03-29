#include "../lib/technique.h"
#include "../lib/loggl.h"

#include <string>
#include <fstream>

using engine1::Technique;
using engine1::Technique1;

#define INVALID_UNIFORM_LOCATION 0xffffffff

bool ReadTextFile(const char *file_path, std::string &dest);

bool Technique::Init(){
    shader_program = glCreateProgram();

    if (shader_program == 0) {
        GlLogErr("Error creating shader program\n");
        return false;
    }

    return true;
}

bool Technique::AddShader(GLenum shader_type, const char* shader_source_filepath){
    std::string shader_source;
    if (!ReadTextFile(shader_source_filepath, shader_source)) {
        GlLogErr("Failed to load shader file: %s", shader_source_filepath);
        return false;
    }

    GLuint shader_obj = glCreateShader(shader_type);
    if(shader_obj == 0){
        GlLogErr("Failed to create shader with type %d with source: %s", shader_type, shader_source_filepath);
        return false;
    }
    shader_obj_list_.push_back(shader_obj);


    const GLchar* shaders_sources[1];
    shaders_sources[0] = shader_source.c_str();
    GLint shader_source_lengths[1] = { static_cast<GLint>(shader_source.size()) };
    glShaderSource(shader_obj, 1, shaders_sources, shader_source_lengths);
    glCompileShader(shader_obj);

    GLint shader_compile_success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &shader_compile_success);

    if(!static_cast<bool>(shader_compile_success)){
        GLchar shader_information_log[1024];
        glGetShaderInfoLog(shader_obj, 1024, NULL, shader_information_log);
        GlLogErr("Failed to compile %s. Shader info log tail: \n %s\n\n");
        return false;
    }

    glAttachShader(shader_program, shader_obj);
    return true;
}

bool Technique::Finalize(){

    char log_buffer[1024];

    GLint shader_program_link_success;
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_program_link_success);

    if(shader_program_link_success == 0){
        glGetProgramInfoLog(shader_program, sizeof(log_buffer), NULL, log_buffer);
        GlLogErr("Failed to link shader program: \n %s\n\n", log_buffer);
        return false;
    }

    GLint shader_program_validation_success;
    glValidateProgram(shader_program);
    glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &shader_program_validation_success);
    if (shader_program_validation_success == 0) {
        glGetProgramInfoLog(shader_program, sizeof(log_buffer), NULL, log_buffer);
        fprintf(stderr, "Shader program validation failed: \n %s\n\n", log_buffer);
    }

    for (GLuint shader_program_obj : shader_obj_list_) {
        glDeleteShader(shader_program_obj);
}
    shader_obj_list_.clear();

    return shader_program_validation_success != 0;
}

void Technique::Enable() const{
    glUseProgram(shader_program);
}

GLint Technique::GetUniformLocation(const char* uniform_name) const
{
    GLuint location = glGetUniformLocation(shader_program, uniform_name);

    if (location == 0xffffffff) {
        GlLogErr("Warning! Unable to get the location of uniform '%s'\n", uniform_name);
    }

    return location;
}

Technique::~Technique(){}

bool ReadTextFile(const char *file_path, std::string &dest){
    std::ifstream file_stream(file_path, std::ios::in);
    if(!file_stream.is_open()) { 
        return false;
}

    std::string line;
    while(!file_stream.eof()) {
        std::getline(file_stream, line);
        dest.append(line + "\n");
    }
    
    file_stream.close();
    return true;
}


bool Technique1::Init(){
    if(!Technique::Init()) {
        return false;
}
    
    if(!AddShader(GL_VERTEX_SHADER, "./assets/shaders/vertexshader.vert")) {
        return false;
}
    
    if(!AddShader(GL_FRAGMENT_SHADER, "./assets/shaders/fragmentshader.frag")) {
        return false;
}

    if(!Finalize()) {
        return false;
}

    uni_view_location_ = GetUniformLocation("view");
    uni_proj_location_ = GetUniformLocation("proj");
    uni_model_location_ = GetUniformLocation("model");
    g_sampler_location = GetUniformLocation("g_sampler");

    if( uni_model_location_ == INVALID_UNIFORM_LOCATION ||
        uni_proj_location_ == INVALID_UNIFORM_LOCATION ||
        uni_view_location_ == INVALID_UNIFORM_LOCATION ||
        g_sampler_location == INVALID_UNIFORM_LOCATION
    ){
        GlLogErr("Invalid uniform location");
        return false;
    }

    return true;
}

void Technique1::SetModelMatrix(const Eigen::Matrix4f &model_matrix) const{
    glUniformMatrix4fv(uni_model_location_, 1, GL_FALSE,  model_matrix.data());
}

void Technique1::SetViewMatrix(const Eigen::Matrix4f &view_matrix) const{
    glUniformMatrix4fv(uni_view_location_, 1, GL_FALSE,  view_matrix.data());
}

void Technique1::SetProjectionMatrix(const Eigen::Matrix4f &projection_matrix) const{
    glUniformMatrix4fv(uni_proj_location_, 1, GL_FALSE,  projection_matrix.data());
}

Technique1::~Technique1(){
    glDeleteProgram(shader_program);
}
