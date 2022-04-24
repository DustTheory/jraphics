#include "../lib/player_controls.h"

#include <iostream>

void PlayerController::Handle(){
    float speed = 0.05;
    if(GetKeyState(GLFW_KEY_W) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(0, 0, speed);
    }

    if(GetKeyState(GLFW_KEY_S) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(0, 0, -speed);
    }

    if(GetKeyState(GLFW_KEY_A) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(speed, 0, 0);
    }

    if(GetKeyState(GLFW_KEY_D) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(-speed, 0, 0);
    }

    if(GetKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(0, -speed, 0);
    }

    if(GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS){
        player_.player_camera.MoveRelativeToLook(0, speed, 0);
    }

    double x_pos;
    double y_pos;
    int x_bound;
    int y_bound;
    GetCursorBounds(x_bound, y_bound);
    GetCursorPosition(x_pos, y_pos);
    static double old_x_pos = x_pos;
    static double old_y_pos = y_pos;

    double delta_x = x_pos - old_x_pos;
    double delta_y = y_pos - old_y_pos;

    float horizontal_angle = (delta_x/static_cast<double>(x_bound)) * (M_PI/4);
    float vertical_angle = (delta_y/static_cast<double>(y_bound)) * (M_PI/4); 
    player_.player_camera.Rotate(0, horizontal_angle, -vertical_angle);

    old_y_pos = y_pos, old_x_pos = x_pos;
}

PlayerController::PlayerController(Player &player) : player_(player){}
