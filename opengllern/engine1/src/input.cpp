#include "../lib/input.h"

using Engine1::InputRouter;
using Engine1::InputHandler;

InputRouter::InputRouter(GLFWwindow* window){
      SetWindow(window);
}

void InputRouter::SetWindow(GLFWwindow *window){
      if(!window)
            throw "Window mustn't be NULL";
      window_ = window;
}

void InputRouter::RoutePoll(){
     for(auto& handler : handlers_)
            handler->Handle();
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
      return glfwGetCursorPos(window_, &pos_x, &pos_y);
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
      return GetRouter()->GetCursorPosition(pos_x, pos_y);
}
