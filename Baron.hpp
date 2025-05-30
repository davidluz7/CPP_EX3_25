// doodluz7@gmail.com

#pragma once
#include "Player.hpp"

namespace coup {

    class Baron : public Player {
    public:
        Baron(Game& game, const std::string& name);

        void tax() override;
        void invest() override;
        std::string role() const override;

        void on_sanctioned();  // פעולה צדדית לפיצוי
    };
}