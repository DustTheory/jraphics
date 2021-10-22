#include "../lib/player_controls.h"

void PlayerController::Handle(){
    float speed = 0.05;
    if(GetKeyState(GLFW_KEY_W) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(0, 0, speed);
    }

    if(GetKeyState(GLFW_KEY_S) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(0, 0, -speed);
    }

    if(GetKeyState(GLFW_KEY_A) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(speed, 0, 0);
    }

    if(GetKeyState(GLFW_KEY_D) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(-speed, 0, 0);
    }

    if(GetKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(0, -speed, 0);
    }

    if(GetKeyState(GLFW_KEY_SPACE) == GLFW_PRESS){
        player_.player_camera_.MoveRelativeToLook(0, speed, 0);
    }
}

PlayerController::PlayerController(Player &player) : player_(player){}