#ifndef ENGINE1_LIB_INPUT_H_
#define ENGINE1_LIB_INPUT_H_

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>

namespace Engine1 {

class InputRouter;
class InputHandler;



class InputHandler {
public:
	InputHandler() = default;
  virtual void Handle() = 0;
  void SetRouter(Engine1::InputRouter* input_router);

private:
  Engine1::InputRouter* input_router_;


protected:
  Engine1::InputRouter* GetRouter();

  int GetKeyState(int key);
  int GetMouseButtonState(int button);
  void GetCursorPosition(double& pos_x, double& pos_y);
  void GetCursorBounds(int& pos_x, int& pos_y);

  friend class Engine1::InputRouter;
};

class InputRouter
{
public:
  InputRouter(){}
  explicit InputRouter(GLFWwindow* window);
  
  void SetWindow(GLFWwindow *window);
  void RoutePoll();
  void AddHandler(std::shared_ptr<Engine1::InputHandler> handler);

private:
  GLFWwindow* window_;
  std::vector<std::shared_ptr<Engine1::InputHandler> > handlers_;

 protected:
  int GetKeyState(int key);
  int GetMouseButtonState(int button);
  void GetCursorPosition(double& pos_x, double& pos_y);
  void GetCursorBounds(int& pos_x, int& pos_y);

  friend class Engine1::InputHandler;

};


}; // namespace Engine1

#endif  // ENGINE1_LIB_INPUT_H_

