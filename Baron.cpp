// doodluz7@gmail.com
#include "Baron.hpp"
#include "Game.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {}

    // פעולה רגילה של מס – מקבל 2 מטבעות
    void Baron::tax() {
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

    // יכולת מיוחדת של השקעה – 3 יוצאים, 6 נכנסים
    void Baron::invest() {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        if (coins() < 3) {
            throw runtime_error("Not enough coins to invest.");
        }
        remove_coins(3);
        add_coins(6);
        set_last_action("invest");
        _game.next_turn();
    }

    // אם הוא נענש עם sanction – מקבל מטבע פיצוי
    void Baron::on_sanctioned() {
        add_coins(1);
    }

    std::string Baron::role() const {
        return "Baron";
    }

    //void Baron::sanction_defense(Player& attacker) { /* TODO: implement sanction_defense */ }
}