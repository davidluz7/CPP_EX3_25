// doodluz7@gmail.com
#pragma once
#include <string>

namespace coup {
    class Game;

    class Player {
    protected:
        std::string _name;
        int _coins;
        Game& _game;
        bool _alive;
        std::string _last_action;

        bool _blocked = false;
        std::string _last_arrester;

    public:
        Player(Game& game, const std::string& name);
        virtual ~Player() = default;

        virtual void gather();
        virtual void tax();
        virtual void invest();
        virtual void coup(Player& target);
        virtual void undo(Player& target);

        virtual void bribe();
        virtual void arrest(Player& target);
        virtual void sanction(Player& target);

        std::string name() const;
        int coins() const;
        bool is_alive() const;
        virtual std::string role() const = 0;

        void set_alive(bool value);
        void set_coins(int value);

        bool is_blocked() const;
        void set_blocked(bool value);
        void set_arrested_by(const std::string& name);
        std::string last_arrester() const;

        void set_last_action(const std::string& action); // מגדיר את הפעולה האחרונה
        std::string get_last_action() const;             // מחזיר את הפעולה האחרונה

        void add_coins(int amount);     // מוסיף מטבעות לשחקן
        void remove_coins(int amount);  // מוריד מטבעות לשחקן עם בדיקת חריגה
    };
}