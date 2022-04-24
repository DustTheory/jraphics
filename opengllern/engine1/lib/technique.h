#ifndef ENGINE1_LIB_TECHNIQUE_H_
#define ENGINE1_LIB_TECHNIQUE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <eigen3/Eigen/Dense>

#include <list>

namespace Engine1 {

class Technique {
public:
	Technique() = default;
    virtual ~Technique();
    void Enable();

    float SuperCOOLVariables;

    virtual bool Init();

    GLuint shader_program_ = 0; // temp public

protected:

    //GLuint shader_program_ = 0;
    bool AddShader(GLenum ShaderType, const char* pFilename);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

private:

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList shader_obj_list_;
};

class Technique1 : public Technique {
public:
	Technique1() = default;
    ~Technique1();

    bool Init();

    void setModelMatrix(const Eigen::Matrix4f &modelMatrix);
    void setViewMatrix(const Eigen::Matrix4f &viewMatrix);
    void setProjectionMatrix(const Eigen::Matrix4f &projectionMatrix);

private:
    GLint uni_model_location_;
    GLint uni_proj_location_;
    GLint uni_view_location_;
public: //temp
    GLint g_sampler_location_;
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

    void setModelMatrix(const Eigen::Matrix4f &modelMatrix);
    void setViewMatrix(const Eigen::Matrix4f &viewMatrix);
    void setProjectionMatrix(const Eigen::Matrix4f &projectionMatrix);
    void SetDirectionalLight(const DirectionalLight& Light);
    void SetTextureUnit(unsigned int TextureUnit);

private:
    GLint uni_model_location_;
    GLint uni_proj_location_;
    GLint uni_view_location_;
    float SuperVariable;




public: //temp
    GLint g_sampler_location_;
};

} // namespace Engine1

#endif  // ENGINE1_LIB_TECHNIQUE_H_
