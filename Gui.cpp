// doodluz7@gmail.com
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
using namespace std;
using namespace coup;

namespace coup {
    void run_gui(Game &game, vector<Player*> &players) {
        sf::RenderWindow window(sf::VideoMode(1000, 700), "Coup Game - Interactive GUI");
        sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            cerr << "Font not found" << endl;
            return;
        }

        vector<string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup", "Undo Tax", "Prevent Arrest", "Invest", "Block Coup", "Cancel Bribe", "Continue Turn", "Restart Game"};
        vector<sf::RectangleShape> actionButtons;
        vector<sf::Text> actionButtonTexts;
        float buttonWidth = 120, buttonHeight = 40, startX = 20, startY = 600;

        string pendingCoupPlayerName = "";
        string pendingCoupTargetName = "";

        for (size_t i = 0; i < actions.size(); ++i) {
            actionButtons.push_back(sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight)));
            float row = (i < 6) ? 0 : 1;
            float col = (i < 6) ? i : (i - 6);
            actionButtons[i].setPosition(startX + col * (buttonWidth + 10), startY + row * (buttonHeight + 10));
            actionButtons[i].setFillColor(sf::Color::White);
            sf::Text txt(actions[i], font, 16);
            txt.setFillColor(sf::Color::Black);
            txt.setPosition(startX + col * (buttonWidth + 10) + 10, startY + row * (buttonHeight + 10) + 10);
            actionButtonTexts.push_back(txt);
        }

        vector<sf::RectangleShape> playerButtons;
        vector<sf::Text> playerButtonTexts;
        float pStartX = 700, pStartY = 100, pWidth = 200, pHeight = 30;
        for (Player* p : players) {
            playerButtons.push_back(sf::RectangleShape(sf::Vector2f(pWidth, pHeight)));
            playerButtons.back().setPosition(pStartX, pStartY);
            playerButtons.back().setFillColor(sf::Color::White);
            sf::Text txt(p->name(), font, 16);
            txt.setFillColor(sf::Color::Black);
            txt.setPosition(pStartX + 10, pStartY + 5);
            playerButtonTexts.push_back(txt);
            pStartY += 40;
        }

        string message = "Select a player (if needed) and then select an action.";
        int selectedPlayerIndex = -1;
        bool pendingBribe = false;
        bool pendingCoup = false;
        bool gameOver = false;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed) {
                    float mx = event.mouseButton.x;
                    float my = event.mouseButton.y;

                    for (size_t i = 0; i < playerButtons.size(); ++i) {
                        if (playerButtons[i].getGlobalBounds().contains(mx, my)) {
                            selectedPlayerIndex = i;
                            message = "Selected: " + players[i]->name();
                        }
                    }

                    Player* current = nullptr;
                    for (Player* p : players) {
                        if (p->name() == game.turn()) {
                            current = p;
                            break;
                        }
                    }

                    for (size_t i = 0; i < actionButtons.size(); ++i) {
                        if (actionButtons[i].getGlobalBounds().contains(mx, my)) {
                            try {
                                string action = actions[i];

                                try {
                                    std::string winName = game.winner();
                                    message = "🏆 Winner is " + winName + "! Game over.";
                                    gameOver = true;
                                    // כאן אפשר לעצור את המשחק או לאפס אותו
                                } catch (...) {
                                    // אין עדיין מנצח → ממשיכים כרגיל
                                }

                                // בודק אם המשחק נגמר
                                if (gameOver && action != "Restart Game") {
                                    pendingBribe = false;
                                    pendingCoup = false;
                                    message = "Game over! Only Restart Game is available.";
                                    continue;
                                }   

                                // בודק אם קיים הקפאה לביצוע החלטה
                                if (pendingBribe && action != "Cancel Bribe" && action != "Continue Turn") {
                                    message = "Judge decision pending. Only Cancel Bribe or Continue Turn allowed.";
                                    continue;
                                }
                                if (pendingCoup && action != "Block Coup" && action != "Continue Turn") {
                                    message = "General decision pending. Only Block Coup or Continue Turn allowed.";
                                    continue;
                                }

                                // מחייב הפיכה כאשר יש לשחקן 10 מטבעות
                                if (current->coins() >= 10 && action != "Coup" && action != "Block Coup" && action != "Continue Turn") {
                                    message = "You must perform a Coup when you have 10 coins.";
                                    continue;
                                }

                                if (action == "Gather") {
                                    current->gather();
                                    message = "Gather succeeded!";
                                }
                                else if (action == "Tax") {
                                    current->tax();
                                    message = "Tax succeeded!";
                                }
                                else if (action == "Bribe") {
                                    current->bribe();
                                    message = "Bribe done. Judge may cancel. Waiting for decision.";
                                    pendingBribe = true;
                                }
                                else if (action == "Invest") {
                                    Baron* baron = dynamic_cast<Baron*>(current);
                                    if (baron) {
                                        baron->invest();
                                        message = "Baron invested successfully!";
                                        game.next_turn();
                                    } else {
                                        message = "You must be Baron to invest.";
                                    }
                                }
                                else if (action == "Block Coup") {
                                    if (pendingCoup) {
                                        for (Player* p : players) {
                                            General* general = dynamic_cast<General*>(p);
                                            if (general) {
                                                bool success = general->block_coup();
                                                if (success) {
                                                    pendingCoup = false;
                                                    game.next_turn();
                                                    message = "Coup blocked successfully! Turn moved forward.";
                                                } else {
                                                    message = "Failed to block coup (You need 5 coins to block a coup!).";
                                                }
                                            }
                                        }
                                    } else {
                                        message = "No recent coup found to block.";
                                    }
                                }
                                else if (action == "Cancel Bribe") {
                                    Player* lastBriber = nullptr;
                                    for (Player* p : players) {
                                        if (p->get_last_action() == "bribe") {
                                            lastBriber = p;
                                            break;
                                        }
                                    }
                                    if (lastBriber) {
                                        for (Player* p : players) {
                                            Judge* judge = dynamic_cast<Judge*>(p);
                                            if (judge) {
                                                judge->cancel_bribe(*lastBriber);
                                                pendingBribe = false;
                                                game.next_turn();
                                                message = "Bribe canceled on " + lastBriber->name() + ". Turn moved forward.";
                                            }
                                        }
                                    } else {
                                        message = "No recent bribe found to cancel.";
                                    }
                                }
                                else if (action == "Continue Turn") {
                                    if (pendingCoup) {
                                        pendingCoup = false;
                                    } else if (pendingBribe) {
                                        pendingBribe = false;
                                        message = "Bribe executed! " + current->name() + " has another turn!.";
                                    } else {
                                        message = "No pending phase. Already on active turn.";
                                    }
                                }

                                else if (selectedPlayerIndex >= 0) {
                                    Player& targetPlayer = *players[selectedPlayerIndex];
                                    if (action == "Arrest") {
                                        current->arrest(targetPlayer);
                                        message = "Arrest succeeded!";
                                    }
                                    else if (action == "Sanction") {
                                        current->sanction(targetPlayer);
                                        message = "Sanction succeeded!";
                                    }
                                    else if (action == "Undo Tax") {
                                        Governor* gov = dynamic_cast<Governor*>(current);
                                        if (selectedPlayerIndex >= 0 && gov) {
                                            gov->undo(targetPlayer);
                                            message = "Governor undid tax on " + players[selectedPlayerIndex]->name();
                                            game.next_turn();
                                        } else {
                                            message = "Select a target and make sure you are Governor.";
                                        }
                                    }
                                    else if (action == "Prevent Arrest") {
                                        Spy* spy = dynamic_cast<Spy*>(current);
                                        if (selectedPlayerIndex >= 0 && spy) {
                                            spy->block_arrest(targetPlayer);
                                            message = "Spy prevented arrest on " + players[selectedPlayerIndex]->name();
                                            game.next_turn();
                                        } else {
                                            message = "Select a target and make sure you are Spy.";
                                        }
                                    }
                                    else if (action == "Coup") {
                                        bool generalExists = false;
                                        for (Player* p : players) {
                                            if (dynamic_cast<General*>(p) && p->is_alive()) {
                                                generalExists = true;
                                                break;
                                            }
                                        }
                                        if (generalExists) {
                                            pendingCoupPlayerName = current->name();
                                            pendingCoupTargetName = targetPlayer.name();
                                            current->coup(targetPlayer);
                                            message = "Coup by " + pendingCoupPlayerName + " on " + pendingCoupTargetName + ". General may block. Waiting for decision.";
                                            pendingCoup = true;
                                        }
                                        else {
                                            current->coup(targetPlayer);
                                            message = "Coup executed immediately! " + targetPlayer.name() + " has been eliminated.";
                                        }
                                    }
                                }
                                else if (action == "Restart Game") {
                                    for (Player* p : players) {
                                        p->set_alive(true);
                                        p->set_coins(0);
                                    }
                                    pendingCoup = false;
                                    pendingBribe = false;
                                    pendingCoupPlayerName = "";
                                    pendingCoupTargetName = "";
                                    game.reset_turn();
                                    gameOver = false;  // ← כאן מאפסים את המצב
                                    message = "Game has been restarted!";
                                } else {
                                    message = "You must first select a target.";
                                }

                                selectedPlayerIndex = -1;
                            } catch (const exception &e) {
                                message = string("Error: ") + e.what();
                            }
                        }
                    }
                }
            }

            window.clear(sf::Color(30, 30, 30));
            float y = 20;
            sf::Text title("Game Status", font, 24);
            title.setFillColor(sf::Color::White);
            title.setPosition(20, y);
            window.draw(title);

            y += 40;
            sf::Text turn_text("Turn: " + game.turn(), font, 20);
            turn_text.setFillColor(sf::Color::Yellow);
            turn_text.setPosition(20, y);
            window.draw(turn_text);

            y += 40;
            for (Player* p : players) {
                if (!p->is_alive()) continue;
                bool is_current_turn = (p->name() == game.turn());
                ostringstream info;
                info << p->name() << " (" << p->role() << ")";
                if (is_current_turn) {
                    info << " - " << p->coins() << " coins";
                }                sf::Text t(info.str(), font, 18);
                t.setPosition(40, y);
                t.setFillColor(sf::Color::Cyan);
                window.draw(t);
                y += 30;
            }

            for (size_t i = 0; i < actionButtons.size(); ++i) {
                window.draw(actionButtons[i]);
                window.draw(actionButtonTexts[i]);
            }

            for (size_t i = 0; i < playerButtons.size(); ++i) {
                window.draw(playerButtons[i]);
                window.draw(playerButtonTexts[i]);
            }

            sf::Text msg_text(message, font, 16);
            msg_text.setFillColor(sf::Color::Red);
            msg_text.setPosition(20, 550);
            window.draw(msg_text);

            window.display();
        }
    }
}