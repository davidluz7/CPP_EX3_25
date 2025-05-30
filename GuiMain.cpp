// doodluz7@gmail.com
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"
#include "Gui.hpp"

#include <vector>
using namespace std;
using namespace coup;

int main() {
    Game game;
    Governor g(game, "Moshe");
    Spy s(game, "Yossi");
    Baron b(game, "Meirav");
    General gn(game, "Reut");
    Judge j(game, "Gilad");

    vector<Player*> players = {&g, &s, &b, &gn, &j};

    run_gui(game, players);

    return 0;
}