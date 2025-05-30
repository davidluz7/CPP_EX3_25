// doodluz7@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include "Merchant.hpp"
#include <stdexcept>
#include <algorithm> // נדרש עבור std::find

using namespace std;

namespace coup {

    Game::Game() : _current_turn(0), _started(false) {}

    // מוסיף שחקן למשחק (רק לפני שהתחיל)
    void Game::add_player(Player* player) {
        if (_players.size() >= 6) {
            throw runtime_error("Game already has maximum number of players (6).\n");
        }
        if (_started) {
            throw runtime_error("Cannot add players after game started.\n");
        }
        _players.push_back(player);
    }

    // מחזיר את רשימת שמות השחקנים החיים
    vector<string> Game::players() const {
        vector<string> names;
        for (const auto& p : _players) {
            if (p->is_alive()) {
                names.push_back(p->name());
            }
        }
        return names;
    }

    // מחזיר את שם השחקן שתורו כעת
    string Game::turn() const {
        if (_players.empty()) {
            throw runtime_error("No players in the game.\n");
        }
        return _players[_current_turn % _players.size()]->name();
    }

    // מעבר לתור הבא – מדלג על שחקנים שמתו
    void Game::next_turn() {
        if (_players.empty()) return;

        size_t count = _players.size();

        // איפוס חסימות זמניות
        for (auto& p : _players) {
            if (p->is_alive()) {
                p->set_blocked(false);        // מסיר חסימה כלכלית
                p->set_arrested_by("");      // מאפס את זהות העוצר הקודם
            }
        }

        // מעבר לשחקן הבא החי
        do {
            _current_turn = (_current_turn + 1) % count;
        } while (!_players[_current_turn]->is_alive());
        Player* nextPlayer = _players[_current_turn];
        Merchant* merchant = dynamic_cast<Merchant*>(nextPlayer);
        if (merchant && merchant->coins() >= 3) {
            merchant->add_coins(1);
        }
        
    }

    // מאתחל משחק
    void Game::reset_turn() {
    _current_turn = 0;
}

    // בדיקה אם המשחק התחיל
    bool Game::has_started() const {
        return _started;
    }

    // מחזיר את שם המנצח – רק אם נשאר שחקן אחד
    string Game::winner() const {
        vector<string> alive;
        for (const auto& p : _players) {
            if (p->is_alive()) {
                alive.push_back(p->name());
            }
        }
        if (alive.size() != 1) {
            throw runtime_error("Game is still ongoing.");
        }
        return alive[0];
    }

    Player* Game::last_coup_attacker() const {
        return _last_coup_attacker;
    }

    Player* Game::last_coup_target() const {
        return _last_coup_target;
    }

    void Game::set_last_coup_attacker(Player* player) {
        _last_coup_attacker = player;
    }
    void Game::set_last_coup_target(Player* player) {
        _last_coup_target = player;
    }

    // בדיקה אם שחקן מסוים חסום מ-arrest
    bool Game::is_arrest_blocked(const std::string& name) const {
        return std::find(_blocked_arrest_list.begin(), _blocked_arrest_list.end(), name) != _blocked_arrest_list.end();
    }

    // איפוס רשימת החסימות – נקרא בתחילת כל תור
    void Game::clear_blocked_list() {
        _blocked_arrest_list.clear();
    }

    // הוספה לתור
    void Game::add_to_blocked_list(Player& target) {
        if (!target.is_alive()) {
            throw runtime_error("Target is dead.");
        }
        _blocked_arrest_list.push_back(target.name());
    }
}