#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Card.h"
#include "Player.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <vector>
sf::Font g_font("arial.ttf");

void drawTableGUI(sf::RenderWindow& win,
    const std::array<std::set<Card>, 4>& lastPlay)
{
    constexpr float centerY = 350.f;         // 四個人都往此列排
    constexpr float centerX = 540.f;

    win.clear({ 25,115,40 });                  // 綠色桌面

    for (int p = 0; p < 4; ++p)
    {
        float y = centerY + (p - 1.5f) * 110.f;        // 往上下錯開
        if (lastPlay[p].empty())
        {   // 顯示 Pass
            sf::Text t(g_font);
            t.setString("pass");
            t.setCharacterSize(28);
            t.setFillColor(sf::Color::White);
            t.setPosition({ centerX - 40.f, y - 40.f });
            win.draw(t);
        }
        else
        {   // 顯示這副牌
            float width = static_cast<float>(lastPlay[p].size() - 1) * 65.f * 0.5f;
            Player::drawCardSet(win, lastPlay[p], centerX - width / 2, y);
        }
    }
}
/*==================== 隨機工具 ====================*/
static std::mt19937 rng{
    static_cast<std::uint32_t>(
        std::chrono::steady_clock::now().time_since_epoch().count()) };

template<class T>
void shuffleVec(std::vector<T>& v) { std::shuffle(v.begin(), v.end(), rng); }

/*==================== 發牌 ====================*/
void deal(std::array<std::set<Card>, 4>& hands)
{
    std::vector<Card> deck; deck.reserve(52);
    for (int i = 0; i < 52; ++i) deck.emplace_back();
    shuffleVec(deck);

    for (int i = 0; i < 52; ++i) hands[i / 13].insert(deck[i]);
}

/*==================== 幫忙找下一位 ====================*/
int nextAlive(int cur, const std::array<bool, 4>& done) {
    for (int step = 1; step <= 4; ++step) {
        int j = (cur + step) % 4; if (!done[j]) return j;
    }
    return cur;           // 只剩自己（通常迴圈會提早結束）
}

bool serviceWin(sf::RenderWindow& w) {
    while (auto ev = w.pollEvent())
        if (ev->is<sf::Event::Closed>()) { w.close(); return false; }
    return true;
}

/*==================== main ====================*/
int main()
{
    std::array<std::set<Card>, 4> lastPlaycs{};
    sf::RenderWindow win(sf::VideoMode({ 1080, 960 }),"Big-Two  left-right choose,  Enter play,  P Pass");
    win.setVerticalSyncEnabled(true);
    Card::setpic();

    /*---- 1. 發牌與玩家 ----*/
    std::array<std::set<Card>, 4> rawHands;
    deal(rawHands);

    std::vector<Player> ps;
    ps.emplace_back(rawHands[0], "p1.json", true);  // human
    ps.emplace_back(rawHands[1], "p2.json");        // AI
    ps.emplace_back(rawHands[2], "p3.json");        // AI
    ps.emplace_back(rawHands[3], "p4.json");        // AI

    /*---- 2. 先攻 ----*/
    int turn = 0;
    for (int i = 0;i < 4;++i) if (ps[i].has_club_3()) { turn = i; break; }

    /*---- 3. 遊戲狀態 ----*/
    std::vector<std::set<Card>> table;
    std::array<bool, 4> done{ false,false,false,false };
    std::vector<int> ranking;
    int passes = 0, lastPlay = -1;

    /*---- 4. 迴圈 ----*/
    while (win.isOpen() && ranking.size() < 3)
    {
        if (!serviceWin(win)) break;            // 允許關窗

        /* 其他三家剩牌數 */
        std::array<unsigned, 3> rest{};
        for (int i = 1;i <= 3;++i) {
            int j = (turn + i) % 4;
            rest[i - 1] = done[j] ? 0u : ps[j].get_hand_count();
        }

        /* 讓玩家行動 */
        auto play = ps[turn].players_turn(
            table, rest[0], rest[1], rest[2], &win);
        table.push_back(play);
        lastPlaycs[turn] = play;//+++++++++++++++++++++
        if (play.empty()) ++passes;
        else { passes = 0; lastPlay = turn; }

        /* 出完牌？ */
        if (!done[turn] && ps[turn].get_hand_count() == 0) {
            done[turn] = true; ranking.push_back(turn);
            passes = 0; lastPlay = turn;
        }

        /* 清桌？ */
        int alive = 4 - static_cast<int>(ranking.size());
        if (alive > 1 && passes >= alive - 1) {
            table.clear(); passes = 0; lastPlay = -1;
        }

        /* 下一家 */
        if (alive == 1) break;                       // 僅剩最後一人
        turn = passes ? nextAlive(turn, done)
            : nextAlive(lastPlay, done);   // 若剛出牌→從出牌者下一位
        //drawTableGUI(win, lastPlaycs);   // 更新桌面
        //win.display();                 // 只有這裡 display 一次

    }

    /*---- 5. 結果 ----*/
    int loser = -1;
    for (int i = 0;i < 4;++i) if (!done[i]) { loser = i; break; }

    std::cout << "\n=== Result ===\n";
    if (ranking.size() >= 1) std::cout << "1st: Player " << ranking[0] + 1 << "\n";
    if (ranking.size() >= 2) std::cout << "2nd: Player " << ranking[1] + 1 << "\n";
    if (ranking.size() >= 3) std::cout << "3rd: Player " << ranking[2] + 1 << "\n";
    if (loser != -1)         std::cout << "Loser: Player " << loser + 1 << '\n';

    while (win.isOpen()) serviceWin(win);
    return 0;
}


//#include"Card.h"
//#include"CardSet.h"
//#include"Player.h"
//#include<random>
//#include<iostream>
//#include<vector>
//#include<set>
//#include<tuple>
//#include<chrono>
//#include <SFML/Graphics.hpp>
//using namespace std;
//
//random_device rad;
//mt19937 engin(rad());
//unsigned int cardnum(Card input);
//void printset(std::set<Card>cs) {
//    for (auto c : cs) {
//        c.print();cout << " ";
//    }cout << endl;
//}
//void pcv(vector<set<Card>>cv) {
//    for (auto i : cv) {
//        for (auto c : i) {
//            c.print();
//            cout << " ";
//        }
//        cout << endl;
//    }
//}
//int rand_num(int a, int b) {
//    uniform_int_distribution<int> dice(a, b);
//    return dice(engin);
//}
//void  scramble_all_into(set<Card>all, set<Card>& s1, set<Card>& s2, set<Card>& s3, set<Card>& s4) {
//    if(all.size()==52){
//        vector<Card>cv;
//        for (auto c : all)cv.push_back(c);
//        for (int i = 0;i < 52;i++) {
//            swap(cv[i], cv[rand_num(i, 51)]);
//        }
//        for (int i = 0;i < 13;i++)s1.insert(cv[i]);
//        for (int i = 13;i < 26;i++)s2.insert(cv[i]);
//        for (int i = 26;i < 39;i++)s3.insert(cv[i]);
//        for (int i = 39;i < 52;i++)s4.insert(cv[i]);
//    }
//}
//tuple<unsigned int, unsigned int, unsigned int> other_players_indices(unsigned int i) {
//    if (i == 0)return { 1,2,3 };
//    else if (i == 1)return { 0,2,3 };
//    else if (i == 2)return { 0,1,3 };
//    else if (i == 3)return { 0,1,2 };
//}
//tuple<unsigned int, unsigned int> other_players_indices(unsigned int i, unsigned int eli) {
//    vector<unsigned int>order = { 0,1,2,3 };
//    order.erase(find(order.begin(),order.end(),i));
//    order.erase(find(order.begin(), order.end(), eli));
//    return { order[0],order[1] };
//}
//unsigned int other_players_indices(unsigned int i, unsigned int eli1, unsigned int eli2) {
//    vector<unsigned int>order = { 0,1,2,3 };
//    order.erase(find(order.begin(), order.end(), i));
//    order.erase(find(order.begin(), order.end(), eli1));
//    order.erase(find(order.begin(), order.end(), eli2));
//    return order[0];
//}
//unsigned int next_index(unsigned int i) {
//    if (i < 3)return ++i;
//    else return 0;
//}
//unsigned int next_index(unsigned int i, unsigned int eli) {
//    if (next_index(i) == eli)return next_index(eli);
//    else return next_index(i);
//}
//unsigned int next_index(unsigned int i, unsigned int eli1, unsigned int eli2) {
//    return other_players_indices(i, eli1, eli2);
//}
//void waitForAnyInput(sf::RenderWindow& window, sf::Text t)
//{
//    // ① 先把「等待」畫面渲染出來一次
//    window.clear(sf::Color::Black);
//    window.draw(t);        // 顯示提示文字
//    window.display();
//
//    // ② 阻塞等待事件
//    while (window.isOpen())
//    {
//        if (auto ev = window.waitEvent(); ev)          // 直到收到事件
//        {
//            // 若是鍵盤或滑鼠按下就跳出
//            if (ev->is<sf::Event::KeyPressed>() ||
//                ev->is<sf::Event::MouseButtonPressed>())
//                break;
//
//            if (ev->is<sf::Event::Closed>())           // 允許關窗
//                window.close();
//        }
//    }
//}
//int main()
//{
//    //本來的，透過iostream顯示電腦隨機遊玩的程式
//    sf::RenderWindow window(sf::VideoMode({ 1080, 960 }), "Big Two!");
//    set<Card>all_card;
//    for (int i = 0;i < 52;i++)all_card.insert(Card());
//    vector<int>count_win(4, 0);
//    Card::setpic();
//    set<Card>set1;
//    set<Card>set2;
//    set<Card>set3;
//    set<Card>set4;
//    scramble_all_into(all_card, set1, set2, set3, set4);
//    cout << "p1:";printset(set1);
//    /*cout << "p2:";printset(set2);
//    cout << "p3:";printset(set3);
//    cout << "p4:";printset(set4);*/
//    cout << endl;
//    vector<Player>players;
//    players.push_back(Player(set1, "p1.json"));//players[0] ,p1, real human XD
//    players.push_back(Player(set2, "p2.json"));//players[1] ,p2
//    players.push_back(Player(set3, "p3.json"));//players[2] ,p3
//    players.push_back(Player(set4, "p4.json"));//players[3] ,p4
//    vector<set<Card>>deck;
//        /*vector<set<Card>>deck2;
//        vector<set<Card>>deck3;*/
//    unsigned int now_playing=0;
//    /*if (players[0].has_club_3())now_playing = 0;
//        else if (players[1].has_club_3())now_playing = 1;
//        else if (players[2].has_club_3())now_playing = 2;
//        else if (players[3].has_club_3())now_playing = 3;*/
//    set<Card>now;
//        /*int first_winner, second_winner, third_winner;*/
//        /*unsigned int i1, i2, i3;*/
//        /*int tim = 1;*/
//        cout << "game start\n";
//    while (window.isOpen()){
//        window.setTitle("Big-Two  -  Player " + std::to_string(now_playing + 1) +
//            (players[now_playing].get_hand_count() == 0 ? " (WIN)" : ""));
//        std::set<Card> played =
//            players[now_playing].players_turn(deck,
//                players[(now_playing + 1) % 4].get_hand_count(),
//                players[(now_playing + 2) % 4].get_hand_count(),
//                players[(now_playing + 3) % 4].get_hand_count(),
//                &window);          // <<== 新參數
//
//        deck.push_back(played);
//
//        /*---- 更新下一家 ----*/
//        if (players[now_playing].get_hand_count() == 0)
//            break;                       // 先完成就終局
//        now_playing = (now_playing + 1) % 4;
//    }
//
//    /*--- 6. 收尾: 等玩家關窗 ---*/
//    sf::Font font("arial.ttf");
//    sf::Text endi(font);
//    endi.setString("end");
//    endi.setOrigin({ 0,0 });
//    endi.setPosition({ 310,480 });
//    bool started = false;
//    while (window.isOpen())
//        waitForAnyInput(window, endi);
//}

        //while (true) {//決定第一
        //    cout << "p" << now_playing + 1 << ":";
        //    tie(i1, i2, i3) = other_players_indices(now_playing);
        //    now = players[now_playing].players_turn(deck, players[i1].get_hand_count(), players[i2].get_hand_count(), players[i3].get_hand_count(),&window);
        //    deck.push_back(now);
        //    players[i1].inspect(now);
        //    players[i2].inspect(now);
        //    players[i3].inspect(now);
        //    if (players[now_playing].get_hand_count() == 0) {
        //        first_winner = now_playing;
        //        now_playing = next_index(now_playing, first_winner);
        //        break;
        //    }
        //    else now_playing = next_index(now_playing);
        //    tim++;
        //}
        //cout << "first winner: p" << first_winner + 1 << endl;
        //while (true) {//決定第二
        //    cout << "p" << now_playing + 1 << ":";
        //    tie(i1, i2) = other_players_indices(now_playing, first_winner);
        //    now = players[now_playing].players_turn(deck2, players[i1].get_hand_count(), players[i2].get_hand_count(),0, &window);
        //    deck2.push_back(now);
        //    players[i1].inspect(now);
        //    players[i2].inspect(now);
        //    if (players[now_playing].get_hand_count() == 0) {
        //        second_winner = now_playing;
        //        now_playing = next_index(now_playing, first_winner);
        //        break;
        //    }
        //    else {
        //        now_playing = next_index(now_playing, first_winner);
        //    }
        //    tim++;
        //}
        //cout << "second winner: p" << second_winner + 1 << endl;
        //while (true) {//決定第三
        //    cout << "p" << now_playing + 1 << ":";
        //    i1 = other_players_indices(now_playing, first_winner, second_winner);
        //    now = players[now_playing].players_turn(deck3, players[i1].get_hand_count(), 0, 0, &window);
        //    deck3.push_back(now);
        //    players[i1].inspect(now);
        //    if (players[now_playing].get_hand_count() == 0) {
        //        third_winner = now_playing;
        //        now_playing = next_index(now_playing, first_winner, second_winner);
        //        break;
        //    }
        //    else {
        //        now_playing = next_index(now_playing, first_winner, second_winner);
        //    }
        //    tim++;
        //}
        //
    /*cout << "third winner: p" << third_winner + 1 << endl;
    cout << "loser: p" << now_playing + 1 << endl;
    count_win[first_winner]++;
    for (int i : count_win)cout << i << " ";cout << endl;*/
    
    
    
    //sf::CircleShape shape(100.f);
    //sf::Texture texture1("clubs_2.png");
    //sf::Sprite sprite(texture1);
    //sprite.setOrigin({ 121,170 });
    //sprite.setPosition({ window.getSize().x / 2.f,window.getSize().y / 2.f });
    //sf::Clock pauseClock;            // 專門給暫停用
    //const sf::Time PAUSE_TIME = sf::seconds(3);
    //
    //sf::Font font("arial.ttf");
    //sf::Text greet(font);
    //greet.setString("Hello,\npress anything to start the game");
    //greet.setOrigin({ 0,0 });
    //greet.setPosition({ 310,480 });
    //bool started = false;
    //while (window.isOpen())
    //{
    //    if (!started) {
    //        waitForAnyInput(window, greet);started = true;
    //    }
    //    while (const std::optional event = window.pollEvent())
    //    {
    //        if (event->is<sf::Event::Closed>())
    //            window.close();
    //    }

    //    window.clear(sf::Color(25, 115, 40));
    //    window.display();
    //}
    //
    


