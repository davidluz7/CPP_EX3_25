// doodluz7@gmail.com
#pragma once
#include "Player.hpp"

namespace coup {

    class Merchant : public Player {
    public:
        Merchant(Game& game, const std::string& name);

        void tax() override;
        void gather() override;
        std::string role() const override;

        // נדרש אם arrest יהיה פעולה חיצונית:
        void handle_arrest();  // יופעל על ידי השחקן התוקף
    };
}