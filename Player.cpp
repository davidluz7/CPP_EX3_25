// doodluz7@gmail.com
#include "Player.hpp"
#include "Game.hpp"
#include "Merchant.hpp"
#include "Judge.hpp"
#include "General.hpp"
#include "Spy.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Player::Player(Game& game, const std::string& name)
        : _name(name), _coins(0), _game(game), _alive(true) {
        _game.add_player(this); // מוסיף את השחקן למשחק ברגע שנוצר
    }

    // פעולה של איסוף מטבע – מקבל מטבע אחד מהקופה
    void Player::gather() {
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
        _coins += 1;
        _last_action = "gather";
        _game.next_turn();
    }

    // ברירת מחדל – תפקיד רגיל לא יכול לבצע tax
    void Player::tax() {
        throw runtime_error("This role cannot perform tax.");
    }

    // ברירת מחדל – רק תפקידים מיוחדים יכולים להשקיע
    void Player::invest() {
        throw runtime_error("This role cannot invest.");
    }

    // פעולה של הפיכה – מדיח שחקן אחר מהמשחק
    void Player::coup(Player& target) {
        if (target.name() == this->name()) {
            throw runtime_error("Cannot coup yourself.");
        }
        if (_coins < 7) {
            throw runtime_error("Not enough coins to coup.");
        }
        if (!target._alive) {
            throw runtime_error("Cannot coup a dead player.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }

        _game.set_last_coup_attacker(this);   // ← שומר מצביע לשחקן התוקף
        _game.set_last_coup_target(&target);  // ← שומר מצביע לשחקן המותקף

        target._alive = false;
        _coins -= 7;
        _last_action = "coup";
        _game.next_turn();
    }

    // פעולה של ביטול – כברירת מחדל לא זמינה
    void Player::undo(Player& target) {
        throw runtime_error("This role cannot undo actions.");
    }

    // שוחד – משלם 4 מטבעות כדי לבצע פעולה נוספת
    void Player::bribe() {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        if (_coins < 4) {
            throw runtime_error("Not enough coins to bribe.");
        }
        _coins -= 4;
        _last_action = "bribe";
        // לא עובר תור – מותר לפעול שוב
    }

    // פעולה של מעצר – לוקח מטבע משחקן אחר אם לא נעצר כבר על ידך
    void Player::arrest(Player& target) {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        if (target.last_arrester() == this->name()) {
            throw runtime_error("Cannot arrest the same player twice in a row.");
        }
        if (!target.is_alive()) {
            throw runtime_error("Target is already eliminated.");
        }
        if (_game.is_arrest_blocked(target.name())) {
            _game.clear_blocked_list();
            throw runtime_error("Target is blocked from arrest.");
        }
        Merchant* merchant = dynamic_cast<Merchant*>(&target);
        if (merchant) {
            merchant->handle_arrest();
            _game.next_turn();
            return;
        }
        General* general = dynamic_cast<General*>(&target);
        if (general) {
            general->on_arrested();
        }
        int stolen = min(1, target._coins);
        target._coins -= stolen;
        _coins += stolen;
        target.set_arrested_by(this->name());
        _last_action = "arrest";
        _game.next_turn();
    }

    // פעולה של חרם – מונע מהשחקן לבצע פעולות כלכליות
    void Player::sanction(Player& target) {
        if (_coins >= 10) {
            throw runtime_error("Must perform a coup when holding 10 or more coins.");
        }
        if (_game.turn() != this->name()) {
            throw runtime_error("Not your turn.");
        }
        if (_coins < 3) {
            throw runtime_error("Not enough coins to sanction.");
        }
        if (!target.is_alive()) {
            throw runtime_error("Cannot sanction a dead player.");
        }
        Judge* judge = dynamic_cast<Judge*>(&target);
        if (judge) {
            judge->on_sanctioned_by(*this);
        }
        _coins -= 3;
        target.set_blocked(true);
        _last_action = "sanction";
        _game.next_turn();
    }

    // Getters רגילים
    string Player::name() const { return _name; }
    int Player::coins() const { return _coins; }
    bool Player::is_alive() const { return _alive; }

    // Setters 
    void Player::set_alive(bool value) {_alive = value;}
    void Player::set_coins(int value) {_coins = value;}


    // בדיקה ועדכון האם השחקן חסום כלכלית
    bool Player::is_blocked() const { return _blocked; }
    void Player::set_blocked(bool value) { _blocked = value; }

    // ניהול מזהה המעצר האחרון
    void Player::set_arrested_by(const string& name) { _last_arrester = name; }
    string Player::last_arrester() const { return _last_arrester; }

    // ניהול פעולה אחרונה לצורך undo
    void Player::set_last_action(const string& action) { _last_action = action; }
    string Player::get_last_action() const { return _last_action; }

    // פעולות עדכון מטבעות בצורה בטוחה
    void Player::add_coins(int amount) { _coins += amount; }
    void Player::remove_coins(int amount) {
        if (_coins < amount) {
            throw runtime_error("Not enough coins.");
        }
        _coins -= amount;
    }
}