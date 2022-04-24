#include <GLFW/glfw3.h>

#include "input.h"

#include <string>
#include <vector>

#ifndef ENGINE1_LIB_OPEN_GL_WINDOW_H_
#define ENGINE1_LIB_OPEN_GL_WINDOW_H_

using Engine1::InputRouter;

namespace Engine1 {

class OpenGlWindow {
public:
    OpenGlWindow();

    std::string GetWindowTitle() const;
    int GetWidth() const;
    int GetHeight() const;
    bool IsFullscreen() const;
    int GetMultisamplingLevel() const;

    GLFWmonitor* GetMonitor() const;
    GLFWwindow* GetWindow() const;
    const GLFWvidmode* GetMode() const;
    const GLFWvidmode* GetAllModes(int &count) const;

    void SetFullScreen( bool fullscreen );
    void SetMultisamplingLevel(int multisampling_level);
    void SetCursorMode(int cursor_mode);

    InputRouter& GetInputRouter(){
        return input_router_;
    }

    ~OpenGlWindow();

private:

    std::string window_title_ = "OpenGl Window";
    int width_;
    int height_;
    int multisampling_level_ = 4;
    bool fullscreen_ = true;

    GLFWwindow* window_ = nullptr;
    GLFWmonitor* monitor_ = nullptr;
    const GLFWvidmode *mode_ = nullptr;

    std::vector<InputRouter*> input_handlers_;

    InputRouter input_router_;

    static void CallbackResize(GLFWwindow* window, int cx, int cy);
    void CreateWindow();
};

} // namespace Engine1

#endif  // ENGINE1_LIB_OPEN_GL_WINDOW_H_

