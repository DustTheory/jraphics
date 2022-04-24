#ifndef ENGIN_E1_LIB_INPUT_H_
#define ENGIN_E1_LIB_INPUT_H_

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

namespace engine1 {

class InputRouter;
class InputHandler;



class InputHandler {
public:
    InputHandler() = default;
  virtual void Handle() = 0;
  void SetRouter(engine1::InputRouter* input_router);

private:
  engine1::InputRouter* input_router_;


protected:
  engine1::InputRouter* GetRouter();

  int GetKeyState(int key);
  int GetMouseButtonState(int button);
  void GetCursorPosition(double& pos_x, double& pos_y);
  void GetCursorBounds(int& pos_x, int& pos_y);

  friend class engine1::InputRouter;
};

class InputRouter
{
public:
  InputRouter(){}
  explicit InputRouter(GLFWwindow* window);
  
  void SetWindow(GLFWwindow *window);
  void RoutePoll();
  void AddHandler(std::shared_ptr<engine1::InputHandler> handler);

private:
  GLFWwindow* window_;
  std::vector<std::shared_ptr<engine1::InputHandler> > handlers_;

 protected:
  int GetKeyState(int key);
  int GetMouseButtonState(int button);
  void GetCursorPosition(double& pos_x, double& pos_y);
  void GetCursorBounds(int& pos_x, int& pos_y);

  friend class engine1::InputHandler;

};


}; // namespace Engine1

#endif  // ENGIN_E1_LIB_INPUT_H_

