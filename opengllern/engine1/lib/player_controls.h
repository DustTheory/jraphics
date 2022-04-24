#ifndef ENGIN_E1_LIB_PLAYER_CONTROLS_H
#define ENGIN_E1_LIB_PLAYER_CONTROLS_H

#include "./input.h"
#include "./player.h"

class PlayerController : public engine1::InputHandler {
 public:
   virtual void Handle();
   explicit PlayerController(Player &player);
 private:
    Player& player_;
};

#endif  // ENGIN_E1_LIB_PLAYER_CONTROLS_H_
