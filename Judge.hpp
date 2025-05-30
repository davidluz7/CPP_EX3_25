// doodluz7@gmail.com
#pragma once
#include "Player.hpp"

namespace coup {

    class Judge : public Player {
    public:
        Judge(Game& game, const std::string& name);

        std::string role() const override;

        void tax() override;
        void cancel_bribe(Player& target);  // מבטל פעולת שוחד של שחקן אחר
        void on_sanctioned_by(Player& from); // מופעל כאשר שחקן אחר מטיל עליו חרם
    };

}