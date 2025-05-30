// doodluz7@gmail.com
#pragma once
#include "Player.hpp"

namespace coup {

    class General : public Player {
    public:
        General(Game& game, const std::string& name);

        std::string role() const override;

        void tax() override;
        bool block_coup();         // מונע הפיכה – מחזיר true אם הצליח
        void on_arrested();        // אם נעצר – מקבל בחזרה את המטבע
    };
}