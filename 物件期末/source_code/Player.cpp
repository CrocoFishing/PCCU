#include<vector>
#include<set>
#include<random>
#include<iostream>
#include<fstream>
#include<string>
#include"Card.h"
#include"CardSet.h"
#include"Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
using namespace std;
std::random_device rd;
std::mt19937 engine(rd());
sf::Font font("arial.ttf");
void printsetin(std::set<Card>cs) {//測試用
    if (cs.empty())cout << "pass";
    else {
        for (auto c : cs) {
            c.print();cout << " ";
        }//cout << endl;
    }
}
Player::Player(std::set<Card> input, std::string f_n, bool is_human)
    : mine(input), file_name(std::move(f_n)), human(is_human) {
}
bool Player::has_club_3() {
	return mine.club_3_included();
}
void Player::inspect(std::set<Card>on_deck) {
	mine.scan_deck(on_deck);
}
void Player::play(std::set<Card>played) {
	mine.play(played);
}
std::vector<std::set<Card>> Player::get_all_possi(std::set<Card>on_deck) {
    std::vector<std::set<Card>>possi;
    std::set<Card>empty_set;
    possi.push_back(empty_set);
    if (has_club_3()) {//means you start the game
        //std::cout << "start the game\n";
        std::vector<std::set<Card>>singles = mine.get_singles();
        std::vector<std::set<Card>>pairs = mine.get_pairs();
        std::vector<std::set<Card>>straights = mine.get_straights();
        std::vector<std::set<Card>>full_houses = mine.get_full_houses();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        possi.insert(possi.end(), singles.begin(), singles.end());
        possi.insert(possi.end(), pairs.begin(), pairs.end());
        possi.insert(possi.end(), straights.begin(), straights.end());
        possi.insert(possi.end(), full_houses.begin(), full_houses.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
        for (int i = 0;i < possi.size();) {//delete cardset without club-3, thus no need for deleting the empty set
            if (possi[i].find(Card(0, 3)) == possi[i].end())possi.erase(possi.begin() + i);
            else ++i;
        }
        /*std::cout << "got possible options(empty):\n";
        for (auto v : possi)printsetin(v);*/
    }
    else if (on_deck == last_play || on_deck.empty()) {
        //std::cout << "on_deck == last_play || on_deck.empty()\n";
        possi.clear();//上副牌是自己出的牌就不應該出空牌
        std::vector<std::set<Card>>singles = mine.get_singles();
        std::vector<std::set<Card>>pairs = mine.get_pairs();
        std::vector<std::set<Card>>straights = mine.get_straights();
        std::vector<std::set<Card>>full_houses = mine.get_full_houses();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        possi.insert(possi.end(), singles.begin(), singles.end());
        possi.insert(possi.end(), pairs.begin(), pairs.end());
        possi.insert(possi.end(), straights.begin(), straights.end());
        possi.insert(possi.end(), full_houses.begin(), full_houses.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
        /*std::cout << "got possible options(on_deck == last_play):\n";
        for (auto v : possi)printsetin(v);*/
    }
    else if (is_single(on_deck)) {
        //std::cout << "saw single\n";
        std::vector<std::set<Card>>singles = mine.get_singles();
        std::vector<std::set<Card>>straight_flushes = mine.get_straight_flushes();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        for (auto c : on_deck) {
            for (int i = 0;i < singles.size();) {
                if (get_identifier(singles[i]) < get_identifier(on_deck))singles.erase(singles.begin() + i);
                else ++i;
            }
        }
        possi.insert(possi.end(), singles.begin(), singles.end());
        possi.insert(possi.end(), straight_flushes.begin(), straight_flushes.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
        /*std::cout << "got possible options(single):\n";
        for (auto v : possi)printsetin(v);*/
    }
    else if (is_pair(on_deck)) {
        //std::cout << "saw pair\n";
        std::vector<std::set<Card>>pairs = mine.get_pairs();
        std::vector<std::set<Card>>straight_flushes = mine.get_straight_flushes();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        for (auto c : on_deck) {
            for (int i = 0;i < pairs.size();) {
                if (get_identifier(pairs[i]) < get_identifier(on_deck))pairs.erase(pairs.begin() + i);
                else ++i;
            }
        }
        possi.insert(possi.end(), pairs.begin(), pairs.end());
        possi.insert(possi.end(), straight_flushes.begin(), straight_flushes.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
        /*std::cout << "got possible options(pair):\n";
        for (auto v : possi)printsetin(v);*/
    }
    else if (is_straight(on_deck) && !is_straight_flush(on_deck)) {
        //std::cout << "saw straight\n";
        std::vector<std::set<Card>>straights = mine.get_straights();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        for (auto c : on_deck) {
            for (int i = 0;i < straights.size();) {
                if ((!is_straight_flush(straights[i])) && get_identifier(straights[i]) < get_identifier(on_deck))straights.erase(straights.begin() + i);
                else ++i;
            }
        }
        possi.insert(possi.end(), straights.begin(), straights.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
        /*std::cout << "got possible options(straight):\n";
        for (auto v : possi)printsetin(v);*/
    }
    else if (is_full_house(on_deck)) {
        //std::cout << "saw full house\n";
        std::vector<std::set<Card>>full_houses = mine.get_full_houses();
        std::vector<std::set<Card>>straight_flushes = mine.get_straight_flushes();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        for (auto c : on_deck) {
            for (int i = 0;i < full_houses.size();) {
                if (get_identifier(full_houses[i]) < get_identifier(on_deck))full_houses.erase(full_houses.begin() + i);
                else ++i;
            }
        }
        possi.insert(possi.end(), full_houses.begin(), full_houses.end());
        possi.insert(possi.end(), straight_flushes.begin(), straight_flushes.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
    }
    else if (is_straight_flush(on_deck) || is_four_of_a_kind(on_deck)) {
        std::vector<std::set<Card>>straight_flushes = mine.get_straight_flushes();
        std::vector<std::set<Card>>four_of_a_kinds = mine.get_four_of_a_kinds();
        possi.insert(possi.end(), straight_flushes.begin(), straight_flushes.end());
        possi.insert(possi.end(), four_of_a_kinds.begin(), four_of_a_kinds.end());
    }
    return possi;
}
std::set<Card> Player::players_turn(const std::vector<std::set<Card>>&deck, unsigned int hand_count1, unsigned int hand_count2, unsigned int hand_count3, sf::RenderWindow* gui) {
    int passes = 0;
    std::vector<std::set<Card>>possi;
    std::set<Card>last_on_deck;
    if (deck.empty()) {
        std::set<Card>empty_set;
        possi = get_all_possi(empty_set);
	}
    else if (deck.back().empty()) {
        /*for (int i = deck.size() - 1;i >= 0;i--) {
            if (deck[i].empty())passes++;
            else {
                last_on_deck = deck[i];
                break;
            }
        }
        possi = get_all_possi(last_on_deck);*/
        for (std::size_t idx = deck.size(); idx-- > 0; )
        {
            if (deck[idx].empty())
                ++passes;              // 連續 Pass 計數
            else
            {
                last_on_deck = deck[idx];
                break;
            }
        }
        possi = get_all_possi(last_on_deck);
    }
    else {
        possi = get_all_possi(deck.back());
        last_on_deck = deck.back();
    }
    std::set<Card>final_op;
    if (!human) {
        if (possi.empty())    // 真的沒有可出的牌 → Pass
             final_op = {};
        else {
            std::uniform_int_distribution<int> choose(0, possi.size() - 1);
            final_op = possi[choose(engine)];
            
        }
    }
    else {
        cout << "last on deck: ";
        printsetin(last_on_deck);
        final_op = chooseMoveGUI(*gui, possi, last_on_deck, hand_count1, hand_count2, hand_count3);
    }
    play(final_op);
    last_play = final_op;
    printsetin(final_op);cout << endl;
    return final_op;
}
unsigned int Player::get_hand_count() {
	return mine.get_hand_count();
}
bool Player::is_single(std::set<Card>cs) { return cs.size() == 1; }
bool Player::is_pair(std::set<Card>cs) { 
    if (cs.size() != 2)return false;
    else {
        vector<int>n;
        for (auto c : cs)n.push_back(c.getnum());
        return n[0] == n[1];
    }
}
bool Player::is_straight(std::set<Card>cs) {
    if (cs.size() != 5)return false;
    else if (cs.size() == 5) {
        vector<int>c;
        for (auto car : cs) c.push_back(car.getnum());
        if (c[0] == c[1] - 1 && c[1] == c[2] - 1 && c[2] == c[3] - 1 && c[3] == c[4] - 1)return true;
        else if (c[0] == 10 && c[1] == 11 && c[2] == 12 && c[3] == 13 && c[4] == 1)return true;
        else if (c[0] == 3 && c[1] == 4 && c[2] == 5 && c[3] == 6 && c[4] == 2)return true;
        else if (c[0] == 3 && c[1] == 4 && c[2] == 5 && c[3] == 1 && c[4] == 2)return true;
        else return false;
    }
}
bool Player::is_full_house(std::set<Card>cs) {
    if (cs.size() != 5)return false;
    else if (cs.size() == 5) {
        vector<int>n;
        for (auto car : cs)n.push_back(car.getnum());
        if (n[0] == n[1] && n[1] == n[2] && n[0] == n[2] && n[3] == n[4])return true;
        if (n[0] == n[1] && n[2] == n[3] && n[3] == n[4] && n[2] == n[4])return true;
        else return false;
    }
}
bool Player::is_straight_flush(std::set<Card>cs) { 
    if (cs.size() != 5)return false;
    else if (cs.size() == 5) {
        vector<int>c;
        vector<int>s;
        for (auto car : cs) c.push_back(car.getnum());
        for (auto car : cs) s.push_back(car.getsuit());
        if (s[0] == s[1] && s[1] == s[2] && s[2] == s[3] && s[3] == s[4]) {
            if (c[0] == c[1] - 1 && c[1] == c[2] - 1 && c[2] == c[3] - 1 && c[3] == c[4] - 1)return true;
            else if (c[0] == 10 && c[1] == 11 && c[2] == 12 && c[3] == 13 && c[4] == 1)return true;
            else if (c[0] == 3 && c[1] == 4 && c[2] == 5 && c[3] == 6 && c[4] == 2)return true;
            else if (c[0] == 3 && c[1] == 4 && c[2] == 5 && c[3] == 1 && c[4] == 2)return true;
            else return false;
        }
        else return false;
    }
}
bool Player::is_four_of_a_kind(std::set<Card>cs) { 
    if (cs.size() != 5)return false;
    else if (cs.size() == 5) {
        vector<int>n;
        for (auto car : cs)n.push_back(car.getnum());
        if (n[0] == n[1] && n[0] == n[2] && n[0] == n[3] && n[0] != n[4])return true;
        if (n[0] != n[1] && n[1] == n[2] && n[1] == n[3] && n[1] == n[4])return true;
        else return false;
    }
}
Card Player::get_identifier(std::set<Card>cs) {//doesn't distinguish between straights and straight flushes, useful for compare full houses
    if (is_single(cs))return *cs.begin();
    else if (is_pair(cs) || is_straight(cs))return *cs.rbegin();
    else if (is_full_house(cs)) {
        vector<int>coun(14, 0);
        for (auto c : cs)coun[c.getnum()]++;
        for (int i = 1;i < 14;i++) {
            if (coun[i] == 3)return Card(0, i);
        }
    }
}
void Player::drawCardSet(sf::RenderWindow& win,
    const std::set<Card>& cssss,
    float x, float y,
    float scale)
{
    float dx = 65.f * scale;          // 每張牌水平間距
    std::size_t i = 0;
    for (const Card& c : cssss)
        c.dis(&win, x + i++ * dx, y, scale);
}
std::set<Card> Player::chooseMoveGUI(
    sf::RenderWindow& win,
    const std::vector<std::set<Card>>& possi
    , std::set<Card>lp, unsigned int c1, unsigned int c2, unsigned int c3)
{
    if (possi.empty())        
        return {};

    std::size_t idx = 0;        
    bool needRedraw = true;   

    while (win.isOpen())
    {
       
        while (auto ev = win.pollEvent())
        {
            if (ev->is<sf::Event::Closed>())
                win.close();

            if (ev->is<sf::Event::KeyPressed>())
            {
                auto key = ev->getIf<sf::Event::KeyPressed>()->code;

                switch (key)
                {
                case sf::Keyboard::Key::Left:     
                    if (idx > 0) { --idx; needRedraw = true; }
                    break;

                case sf::Keyboard::Key::Right:    
                    if (idx + 1 < possi.size()) { ++idx; needRedraw = true; }
                    break;

                case sf::Keyboard::Key::Enter:    
                    return possi[idx];

                case sf::Keyboard::Key::P:       return {};

                default: break;
                }
            }
        }

        if (needRedraw)
        {
            win.clear({ 25, 115, 40 });             // 綠色牌桌 (任意)
            
            sf::Text t(font);
            sf::Text coh(font);
            t.setPosition({ 150,900 });
            t.setString("View Choices: <- or ->  Play: Enter  Pass/Play: Enter or P");
            coh.setPosition({ 50,50 });
            std::string  ss = "Other's:\n";ss.append(std::to_string(c1));ss.append("\t");ss.append(std::to_string(c2));ss.append("\t");ss.append(std::to_string(c3));
            coh.setString(ss);
            drawHandGUI(win, possi[idx]);         // 手上的牌 + 高亮
            
            drawCardSet(win, lp, 240, 240, 0.6);
            win.draw(t);
            win.draw(coh);
            win.display();
            needRedraw = false;
        }
    }

    return {};   // 視窗被關閉  視同 Pass
}

/*====================  畫手牌 ====================*/
void Player::drawHandGUI(sf::RenderWindow& win,
    const std::set<Card>& highlight) const
{
    const float spacing = 65.f;
    const float y = static_cast<float>(win.getSize().y) - 220.f;
    std::size_t i = 0;
    for (const Card& c : mine.getset())
    {
        float x = 60.f + i * spacing;
        bool hi = highlight.count(c);
        c.dis(&win, x+95, y - (hi ? 20.f : 0.f), hi ? 0.55f : 0.45f);
        ++i;
    }
}
