#include "../lib/open_gl_window.h"

using Engine1::OpenGlWindow;
using Engine1::InputRouter;

OpenGlWindow::OpenGlWindow(){
    monitor_ = glfwGetPrimaryMonitor();
    if(!monitor_) {
        throw "Monitor not detected";
}
    
    mode_ = glfwGetVideoMode(monitor_);
    width_ = mode_->width;
    height_ = mode_->height;

    CreateWindow();
    input_router_.SetWindow(window_);
}

std::string OpenGlWindow::GetWindowTitle() const {
    return window_title_;
}

int OpenGlWindow::GetWidth() const {
    return width_;
}

int OpenGlWindow::GetHeight() const {
    return height_;
}

GLFWmonitor* OpenGlWindow::GetMonitor() const {
    return monitor_;
}

GLFWwindow* OpenGlWindow::GetWindow() const {
    return window_;
}

const GLFWvidmode* OpenGlWindow::GetMode() const {
    return mode_;
}

const GLFWvidmode* OpenGlWindow::GetAllModes(int &count) const {
    return glfwGetVideoModes(monitor_, &count);
}

void OpenGlWindow::CreateWindow(){
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, multisampling_level_); 

    window_ =   glfwCreateWindow(width_,
                                height_,
                                window_title_.c_str(),
                                fullscreen_ ? monitor_ : NULL,
                                nullptr);

    if(window_ == NULL) {
        throw "Failed to create window";
}
    
    glfwMakeContextCurrent(window_);
}

void OpenGlWindow::SetCursorMode(int cursor_mode){
    glfwSetInputMode(window_, GLFW_CURSOR, cursor_mode);
}

OpenGlWindow::~OpenGlWindow(){

}

