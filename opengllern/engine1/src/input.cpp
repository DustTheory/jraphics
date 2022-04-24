#include "../lib/input.h"

using engine1::InputRouter;
using engine1::InputHandler;

InputRouter::InputRouter(GLFWwindow* window){
      SetWindow(window);
}

void InputRouter::SetWindow(GLFWwindow *window){
      if(window == nullptr) {
            throw "Window mustn't be NULL";
}
      window_ = window;
}

void InputRouter::RoutePoll(){
     for(auto& handler : handlers_) {
            handler->Handle();
}
}


void InputRouter::AddHandler(std::shared_ptr<InputHandler> handler){
      handler->SetRouter(this);
      handlers_.push_back(handler);
}

int InputRouter::GetKeyState(int key){
      return glfwGetKey(window_, key);
}

int InputRouter::GetMouseButtonState(int button){
      return glfwGetMouseButton(window_, button);
}

void InputRouter::GetCursorPosition(double& pos_x, double& pos_y){
      glfwGetCursorPos(window_, &pos_x, &pos_y);
}

void InputRouter::GetCursorBounds(int& x_bound, int& y_bound){
      glfwGetWindowSize(window_, &x_bound, &y_bound);
}

InputRouter* InputHandler::GetRouter(){
      return input_router_;
}

void InputHandler::SetRouter(InputRouter* input_router){
      input_router_ = input_router;
}


int InputHandler::GetKeyState(int key){
      return GetRouter()->GetKeyState(key);
}

int InputHandler::GetMouseButtonState(int button){
      return GetRouter()->GetMouseButtonState(button);
}

void InputHandler::GetCursorPosition(double& pos_x, double& pos_y){
      GetRouter()->GetCursorPosition(pos_x, pos_y);
}

void InputHandler::GetCursorBounds(int& x_bound, int& y_bound){
      GetRouter()->GetCursorBounds(x_bound, y_bound);
}
