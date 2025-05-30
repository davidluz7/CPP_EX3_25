// doodluz7@gmail.com
#pragma once
#include <string>
#include <vector>

namespace coup {
    class Player;  // Forward declaration

    class Game {
    private:
        std::vector<Player*> _players;
        size_t _current_turn;
        bool _started;
        Player* _last_coup_attacker = nullptr;
        Player* _last_coup_target = nullptr;
        std::vector<std::string> _blocked_arrest_list; // רשימת שחקנים שנחסמו מ-arrest

    public:
        Game();

        void add_player(Player* player);
        std::vector<std::string> players() const;
        std::string turn() const;
        void next_turn();
        void reset_turn();
        Player* last_coup_attacker() const;
        Player* last_coup_target() const;
        void set_last_coup_attacker(Player* player);
        void set_last_coup_target(Player* player);
        bool has_started() const;
        bool is_arrest_blocked(const std::string& name) const; // בדיקה אם שחקן חסום
        void clear_blocked_list();                         // איפוס הרשימה בתחילת תור
        void add_to_blocked_list(Player& target);
        std::string winner() const; // מחזיר את שם המנצח אם נשאר רק שחקן אחד
    };
} 