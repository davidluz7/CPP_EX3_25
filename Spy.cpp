// doodluz7@gmail.com
#include "Spy.hpp"
#include "Game.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm> // נדרש עבור std::find
#include <vector>    // נדרש עבור std::vector

using namespace std;

namespace coup {

    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {}

    // פעולה רגילה של מס – מקבל 2 מטבעות
    void Spy::tax() {
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

    // היכולת המיוחדת של המרגל – לחסום arrest משחקן אחר
    void Spy::block_arrest(Player& target) {
        if (!target.is_alive()) {
            throw runtime_error("Target is dead.");
        }
        _game.add_to_blocked_list(target);
        // אין צורך לעדכן תור – פעולה מחוץ לתור
        throw runtime_error("Target " + target.name() + " has " + std::to_string(target.coins()) + " left");
    }

    std::string Spy::role() const {
        return "Spy";
    }

}