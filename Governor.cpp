// doodluz7@gmail.com
#include "Governor.hpp"
#include "Game.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {}

    // פעולה מיוחדת של Governor – מקבל 3 מטבעות במקום 2
    void Governor::tax() {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        add_coins(3);               // הוספת 3 מטבעות
        set_last_action("tax");    // סימון הפעולה לצורך undo
        _game.next_turn();
    }

    void Governor::undo(Player& target) {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (!target.is_alive()) {
            throw runtime_error("Cannot undo a dead player.");
        }

        int refund = 2;  // ברירת מחדל: שחקן רגיל
        Governor* gov = dynamic_cast<Governor*>(&target);
        if (gov) {
            refund = 3;
        }

        if (target.coins() < refund) {
            throw runtime_error("Target doesn't have enough coins to undo.");
        }

        target.remove_coins(refund);
        target.set_last_action("");
    }

    // זיהוי התפקיד במשחק
    std::string Governor::role() const {
        return "Governor";
    }

}