#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp> //value_ptr

#include <eigen3/Eigen/Dense>
#include "../lib/loggl.h"
#include "../lib/loadShaderFile.h"
#include "../lib/matMath.h"

#include <assert.h>
#include <iostream>
#include <chrono>

int g_gl_width = 1000;
int g_gl_height = 600;

struct vec3 {
    float x, y, z;
};

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    g_gl_width = width;
    g_gl_height = height;
}

void castRay(double mouse_x, double mouse_y) {
    float x = (2.0f * mouse_x) / g_gl_width - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / g_gl_height;
    float z = 1.0f;
    vec3 ray_nds{x, y, z};
    std::cout << x << ' ' << y << ' ' << z << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
       double xpos, ypos;
       //getting cursor position
       glfwGetCursorPos(window, &xpos, &ypos);
       castRay(xpos, ypos);
    }
}

// glm::vec4 test(glm::vec4 v){
//     glm::mat4 trans = glm::mat4(1.0f);
//     trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//     return trans * v;
// }
int main()
{

    assert(restart_gl_log());
    restart_gl_log();
    gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    glfwSetErrorCallback(glfw_error_callback);

    // Start GLFW3
    if(!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }


    // Hint opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create window
    GLFWwindow* window = glfwCreateWindow(g_gl_width, g_gl_height, "OpenGL", nullptr, nullptr); // Windowed

    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vertex_vbo;
    glGenBuffers(1, &vertex_vbo);

    GLfloat vertices[] = {
       0.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f,
       0.0f, 1.0f, 0.0f,
       0.0f, 1.0f, 1.0f,
       1.0f, 0.0f, 0.0f,
       1.0f, 0.0f, 1.0f,
       1.0f, 1.0f, 0.0f,
       1.0f, 1.0f, 1.0f
    };

    for(int i = 0; i < sizeof(vertices)/sizeof(GLfloat); i++)
        vertices[i] -= 0.5;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colors_vbo;
    glGenBuffers(1, &colors_vbo);

    float colors[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f,
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f,
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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

    std::string vertexShaderStr = loadShaderFile("./src/shaders/vertexshader.vert");
    const char *vertexShaderSrc = vertexShaderStr.c_str();

    std::string fragmentShaderStr = loadShaderFile("./src/shaders/fragmentshader.frag");
    const char * fragmentShaderSrc = fragmentShaderStr.c_str();

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);


    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "frag_color");

    glLinkProgram(shaderProgram);
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    GLint uniView = glGetUniformLocation(shaderProgram, "view");


    glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    GLint colAttrib = glGetAttribLocation(shaderProgram, "vertex_color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    GLint posAttrib = glGetAttribLocation(shaderProgram, "vertex_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);


    auto t_start = std::chrono::high_resolution_clock::now();

    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK); 
    glFrontFace(GL_CW);

    Eigen::Matrix4f view = genViewMatrix({0.0, 2.0, -3.0}, {0.0f, 1.0f, 1.0f}, {0, 1.0, 0});
    Eigen::Matrix4f projection = genProjectionMatrix(90, (float)g_gl_width/(float)g_gl_height, 1.0f, 10.0f);
        
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

        //  model *= genScaleMatrix(0.5, 0.5, 0.5);
        //  model *= genTranslateMatrix(0, 0, 0.0);
        model *= genRotateMatrix(time, {0.0f, 1.0, 0.0f});

        glUniformMatrix4fv(uniModel, 1, GL_FALSE,  model.data());
        glUniformMatrix4fv(uniView, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, projection.data());


        glDrawElements(GL_TRIANGLES, sizeof(elements)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    glfwTerminate();
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &colors_vbo);

    glDeleteVertexArrays(1, &vao);    

    return 0;
}