// doodluz7@gmail.com
#include "Merchant.hpp"
#include "Game.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Merchant::Merchant(Game& game, const std::string& name)
        : Player(game, name) {}

    // פעולה רגילה של מס – מקבל 2 מטבעות
    void Merchant::tax() {
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

    // פעולה של gather – אם מתחיל תור עם 3 מטבעות ומעלה, מקבל אחד נוסף
    void Merchant::gather() {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        if (_blocked) {
            throw runtime_error("You are blocked from economic actions.");
        }
        if (_coins >= 10) {
            throw runtime_error("You must coup when you have 10 coins.");
        }

        int bonus = (_coins >= 3) ? 1 : 0;
        _coins += 1 + bonus;
        _last_action = "gather";
        _game.next_turn();
    }

    // אם נעשתה עליו פעולה arrest – משלם 2 לקופה במקום להעביר למבצע
    void Merchant::handle_arrest() {
        if (_coins >= 2) {
            _coins -= 2;
        } else {
            _coins = 0;
        }
    }

    std::string Merchant::role() const {
        return "Merchant";
    }
}