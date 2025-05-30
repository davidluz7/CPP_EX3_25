// doodluz7@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

// בדיקה: ניסיון לקרוא ל-winner לפני שהמשחק הסתיים זורק חריגה
TEST_CASE("Winner throws if game still active") {
    Game game;
    Governor gov(game, "Moshe");
    Spy spy(game, "Yossi");
    CHECK_THROWS(game.winner());
}

// בדיקה: סדר התורות מתקדם נכון בין השחקנים
TEST_CASE("Turn order progresses correctly") {
    Game game;
    Governor gov(game, "Moshe");
    Spy spy(game, "Yossi");
    Baron baron(game, "Meirav");
    CHECK(game.turn() == "Moshe");
    game.next_turn();
    CHECK(game.turn() == "Yossi");
    game.next_turn();
    CHECK(game.turn() == "Meirav");
    game.next_turn();
    CHECK(game.turn() == "Moshe");
}

// בדיקת הפעולת arrest - שחקן רגיל
TEST_CASE("Normal player loses coin on arrest") {
    Game game;
    Spy spy(game, "Yossi");
    Governor governor(game, "Moshe");

    spy.gather(); // +1 coin
    governor.arrest(spy);

    CHECK(spy.coins() == 0); // המרגל איבד מטבע
    CHECK(governor.coins() == 1); // המושל קיבל מטבע
}

//בדיקה ששחקן לא יכול לעשות הפיכה בלי מספיק מטבעות
TEST_CASE("Cannot coup without enough coins") {
    Game game;
    Governor gov(game, "Moshe");
    Spy spy(game, "Yossi");

    gov.gather();  // רק מטבע 1
    CHECK_THROWS(gov.coup(spy));  // צריך לפחות 7 מטבעות
}

//בדיקה ששחקן יכול לבצע פעולה פשוטה בתורו (למשל איסוף משאבים)
TEST_CASE("Player can perform gather on their turn") {
    Game game;
    Governor gov(game, "Moshe");

    CHECK(game.turn() == "Moshe");  // לוודא שזה תורו
    gov.gather();  // פעולה פשוטה
    CHECK(gov.coins() == 1);  // קיבל מטבע אחד
}

//בדיקה שסוחר מקבל בונוס אם יש לו לפחות 3 מטבעות
TEST_CASE("Merchant bonus applies at start of turn") {
    Game game;
    Merchant merchant(game, "Eve");
    for (int i = 0; i < 3; ++i) merchant.gather();
    game.next_turn();
    CHECK(merchant.coins() == 5);
}

//בדיקה שברון יכול להשקיע ולהכפיל מטבעות
TEST_CASE("Baron investment gives bonus") {
    Game game;
    Baron baron(game, "Dan");
    for (int i = 0; i < 3; ++i) baron.gather();
    baron.invest();
    CHECK(baron.coins() == 6);
}
