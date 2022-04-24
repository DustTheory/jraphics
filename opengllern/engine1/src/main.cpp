#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <eigen3/Eigen/Dense>

#include <assert.h>
#include <iostream>
#include <chrono>
#include <memory>

#include "../lib/loggl.h"
#include "../lib/load_shader_file.h"
#include "../lib/mat_math.h"
#include "../lib/open_gl_window.h"
#include "../lib/texture.h"
#include "../lib/technique.h"

#include "../lib/player.h"
#include "../lib/player_controls.h"


using namespace engine1;

void InitLogging(){
    assert(RestartGlLog());
    RestartGlLog();
    GlLog("starting GLFW\n%s\n", glfwGetVersionString());
    glfwSetErrorCallback(GlfwErrorCallback);
}

void InitGlfw(){
    // Start GLFW3
    if(glfwInit() == 0) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(1);
    }

    // Hint opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   
}

void InitGlew(){
    glewExperimental = GL_TRUE;
    glewInit();
}

int main()
{
    InitLogging();
    InitGlfw();
    OpenGlWindow open_gl_window; 
    open_gl_window.SetCursorMode(GLFW_CURSOR_DISABLED);
    InitGlew();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vertex_vbo;
    glGenBuffers(1, &vertex_vbo);

    GLfloat vertices[] = {
       -0.5F, -0.5F, -0.5F, 0.0F, 0.0F,
       -0.5F, -0.5F, 0.5F, 0.0F, 0.0F,
       -0.5F, 0.5F, -0.5F, 0.0F, 1.0F,
       -0.5F, 0.5F, 0.5F, 0.0F, 1.0F,
       0.5F, -0.5F, -0.5F, 1.0F, 0.0F,
       0.5F, -0.5F, 0.5F, 1.0F, 0.0F,
       0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
       0.5F, 0.5F, 0.5F, 1.0F, 1.0F,
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 2, 4,
        2, 6, 4,

        0, 1, 2,
        2, 1, 3,

        3, 1, 5,
        3, 5, 7,
       
        6, 7, 5,
        6, 5, 4,
       
        0, 4, 1,
        1, 4, 5,

        3, 7, 6,
        3, 6, 2,
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    Technique1 test;
    test.Init();

    test.Enable();

    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    GLint pos_attrib = glGetAttribLocation(test.shader_program_, "vertex_position");
    GLint texture_coords_attrib = glGetAttribLocation(test.shader_program_, "vertex_texture_coordinates");

    Texture* texture = new Texture(GL_TEXTURE_2D, "./assets/textures/bricks.jpg");
    if(!texture->Load()) {
        return 1;
}

    auto t_start = std::chrono::high_resolution_clock::now();

    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK); 
    glFrontFace(GL_CCW);

    Player player;
        player.player_camera_.SetAspectRatio(
        static_cast<float>(open_gl_window.GetWidth())/
        static_cast<float>(open_gl_window.GetHeight())
    );
    open_gl_window.GetInputRouter().AddHandler(std::make_shared<PlayerController>(player));
    glEnable(GL_DEPTH_TEST);  
    
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    
    while(glfwWindowShouldClose(open_gl_window.GetWindow()) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        test.Enable();

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        test.setModelMatrix(model);
        test.setViewMatrix(player.player_camera_.GetViewTransformMatrix());
        test.setProjectionMatrix(player.player_camera_.GetProjectionTransformMatrix());

        glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        texture->Bind(GL_TEXTURE0);
        glUniform1i(test.g_sampler_location_, 0);

        glEnableVertexAttribArray(pos_attrib);
        glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
        
        glEnableVertexAttribArray(texture_coords_attrib);
        glVertexAttribPointer(texture_coords_attrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));


        glDrawElements(GL_TRIANGLES, sizeof(elements)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        open_gl_window.GetInputRouter().RoutePoll();
        glfwSwapBuffers(open_gl_window.GetWindow());

        glDisableVertexAttribArray(pos_attrib);
        glDisableVertexAttribArray(texture_coords_attrib);

        if(glfwGetKey(open_gl_window.GetWindow(), GLFW_KEY_ESCAPE) != 0) {
            glfwSetWindowShouldClose(open_gl_window.GetWindow(), 1);
        }

    }

    glfwTerminate();


    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vertex_vbo);

    glDeleteVertexArrays(1, &vao);    
    delete texture;

    return 0;
}
