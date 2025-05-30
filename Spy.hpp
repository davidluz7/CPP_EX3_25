// doodluz7@gmail.com
#pragma once
#include "Player.hpp"
#include <vector> // חובה בשביל std::vector
#include <string>

namespace coup {

    class Spy : public Player {
    private:

    public:
        Spy(Game& game, const std::string& name);

        void tax() override;
        std::string role() const override;

        void block_arrest(Player& target);                 // יכולת ייחודית: חסימת arrest
        
    };
}