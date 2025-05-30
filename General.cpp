// doodluz7@gmail.com
#include "General.hpp"
#include "Game.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    General::General(Game& game, const std::string& name)
        : Player(game, name) {}

    // פעולה רגילה של מס – מקבל 2 מטבעות
    void General::tax() {
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

    // זיהוי התפקיד במשחק
    std::string General::role() const {
        return "General";
    }

    // יכולת מיוחדת: לשלם 5 מטבעות כדי למנוע הפיכה (coup) על עצמו או על אחר
    bool General::block_coup() {
        Player* _last_coup_attacker = nullptr;
        if (coins() < 5) {
            return false; // אין מספיק מטבעות – לא ניתן לחסום
        }
        remove_coins(5); // משלמים 5 מטבעות
        _game.last_coup_target()->set_alive(true);
        return true;     // ההפיכה נחסמה
    }

    // אם מישהו ביצע עליו arrest – הוא מקבל את המטבע חזרה
    void General::on_arrested() {
        add_coins(1);
    }

}