#ifndef PLAYER_H
#define PLAYER_H
#include<vector>
#include<set>
#include<string>
#include"Card.h"
#include"CardSet.h"
#include <SFML/Graphics.hpp>
class Player {
private:
	char filenamechr[50];
	bool human;
	std::vector<double>weights;
	
	std::set<Card>last_play;
	void play(std::set<Card>on_deck);
	std::vector<std::set<Card>> get_all_possi(std::set<Card>on_deck);
	bool is_single(std::set<Card>cs);
	bool is_pair(std::set<Card>cs);
	bool is_straight(std::set<Card>cs);
	bool is_full_house(std::set<Card>cs);
	bool is_straight_flush(std::set<Card>cs);
	bool is_four_of_a_kind(std::set<Card>cs);
	Card get_identifier(std::set<Card>cs);
	std::string file_name;
	std::set<Card> chooseMoveGUI(
		sf::RenderWindow& win,
		const std::vector<std::set<Card>>& possi, std::set<Card>lp, unsigned int c1, unsigned int c2, unsigned int c3);   // Åý¯u¤H¬DµP
	void drawHandGUI(sf::RenderWindow& win,
		const std::set<Card>& highlight) const;
public:
	CardSet mine;
	Player(std::set<Card> input, std::string f_n, bool is_human = false);
	bool has_club_3();
	void inspect(std::set<Card>on_deck);
	//return empty set if player are choosing pass
	std::set<Card> players_turn(const std::vector<std::set<Card>>& deck, unsigned int hand_count1, unsigned int hand_count2, unsigned int hand_count3, sf::RenderWindow* gui);
	unsigned int get_hand_count();
	static void drawCardSet(sf::RenderWindow& win,
		const std::set<Card>& cs,
		float x, float y,
		float scale = 0.5f);
};
#endif
