#ifndef ENGINE1_LIB_PLAYER_CONTROLS_H_
#define ENGINE1_LIB_PLAYER_CONTROLS_H_

#include "./input.h"
#include "./player.h"

class PlayerController : public Engine1::InputHandler {
 public:
   virtual void Handle();
   explicit PlayerController(Player &player);
 private:
    Player& player_;
};

#endif  // ENGINE1_LIB_PLAYER_CONTROLS_H_