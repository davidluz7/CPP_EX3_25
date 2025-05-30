// doodluz7@gmail.com
#ifndef GUI_HPP
#define GUI_HPP

#include "Game.hpp"
#include "Player.hpp"
#include <vector>

namespace coup {
    void run_gui(Game &game, std::vector<Player*> &players);
}

#endif