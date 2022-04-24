#ifndef ENGIN_E1_LIB_TECHNIQUE_H_
#define ENGIN_E1_LIB_TECHNIQUE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <eigen3/Eigen/Dense>

#include <list>

namespace engine1 {

class Technique {
public:
    Technique() = default;
    virtual ~Technique();
    void Enable() const;

    virtual bool Init();

    GLuint shader_program = 0; // temp public

protected:

    //GLuint shader_program_ = 0;
    bool AddShader(GLenum shader_type, const char* p_filename);
    bool Finalize();
    GLint GetUniformLocation(const char* p_uniform_name) const;

private:

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList shader_obj_list_;
};

class Technique1 : public Technique {
public:
    Technique1() = default;
    ~Technique1();

    bool Init();

    void SetModelMatrix(const Eigen::Matrix4f &model_matrix) const;
    void SetViewMatrix(const Eigen::Matrix4f &view_matrix) const;
    void SetProjectionMatrix(const Eigen::Matrix4f &projection_matrix) const;

private:
    GLint uni_model_location_;
    GLint uni_proj_location_;
    GLint uni_view_location_;
public: //temp
    GLint g_sampler_location;
};

struct DirectionalLight {
    Eigen::Vector3f color;
    float ambient_intensity;
};

class SimpleLigthingTechnique : public Technique {
public:
    SimpleLigthingTechnique() = default;
    ~SimpleLigthingTechnique();

    bool Init();

    void SetModelMatrix(const Eigen::Matrix4f &model_matrix) const;
    void SetViewMatrix(const Eigen::Matrix4f &view_matrix);
    void SetProjectionMatrix(const Eigen::Matrix4f &projection_matrix);
    void SetDirectionalLight(const DirectionalLight& light);
    void SetTextureUnit(unsigned int texture_unit);

    float supervarable_fas;

private:
    GLint uni_model_location_;
    GLint uni_proj_location_;
    GLint uni_view_location_;
    float SuperVariable_;




public: //temp
    GLint g_sampler_location;
};

} // namespace Engine1

#endif  // ENGIN_E1_LIB_TECHNIQUE_H_
