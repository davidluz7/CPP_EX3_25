// doodluz7@gmail.com
#include "Judge.hpp"
#include "Game.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {}

    // זיהוי תפקיד
    std::string Judge::role() const {
        return "Judge";
    }

    // פעולה רגילה של מס – מקבל 2 מטבעות
    void Judge::tax() {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        add_coins(2);
        set_last_action("tax");
        _game.next_turn();
    }

    // השופט יכול לבטל bribe של שחקן אחר – גורם לו להפסיד את המטבעות
    void Judge::cancel_bribe(Player& target) {
        if (target.get_last_action() != "bribe") {
            throw runtime_error("No bribe to cancel.");
        }
        // אין צורך להחזיר לו את הפעולה, רק מוחקים את התיעוד
        target.set_last_action("");
    }

    // אם שחקן עשה sanction על שופט – הוא משלם עוד מטבע
    void Judge::on_sanctioned_by(Player& from) {
        from.remove_coins(1); // עונש נוסף למטיל החרם
    }
}