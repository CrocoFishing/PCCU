#ifndef CARDSET_H
#define CARDSET_H
#include<vector>
#include<set>
#include <utility>
#include"Card.h"
class CardSet {
private:
	std::set<Card>hand;//
	std::set<Card>count_remainder;//
	std::pair<std::set<Card>, std::set<Card>>single;//first->hand, second->deck
	std::pair<std::set<Card>, std::set<Card>>pair;//
	std::pair<std::set<Card>, std::set<Card>>straight;//
	std::pair<std::set<Card>, std::set<Card>>full_house;//
	std::pair<std::set<Card>, std::set<Card>>straight_flush;//
	std::pair<std::set<Card>, std::set<Card>>four_of_a_kind;//
	void delete_from_count_remainder(std::set<Card>);
	void delete_from_hand(std::set<Card>);
	void scan_singles(bool initialize_it);//initialize count_remainder
	void scan_pairs();
	void scan_straights();
	void scan_full_houses();
	void scan_straight_flushes();
	void scan_four_of_a_kinds();
	void scan_all(bool initialize_it);
public:
	CardSet();
	CardSet(std::set<Card>input);
	std::set<Card> getset()const;
	unsigned int get_hand_count();
	bool club_3_included();
	std::vector<std::set<Card>> get_singles();//將每張牌做成set
	std::vector<std::set<Card>> get_pairs();//得到所有能出的牌
	std::vector<std::set<Card>> get_straights();//include straight_flush!!!!
	std::vector<std::set<Card>> get_full_houses();
	std::vector<std::set<Card>> get_straight_flushes();
	std::vector<std::set<Card>> get_four_of_a_kinds();
	static std::vector<std::set<Card>> get_singles(std::set<Card>);//輸入一組set<Card>回傳可能性
	static std::vector<std::set<Card>> get_pairs(std::set<Card>);//
	static std::vector<std::set<Card>> get_straights(std::set<Card>);//
	static std::vector<std::set<Card>> get_full_houses(std::set<Card>);
	static std::vector<std::set<Card>> get_straight_flushes(std::set<Card>);
	static std::vector<std::set<Card>> get_four_of_a_kinds(std::set<Card>);
	void scan_deck(std::set<Card>on_deck);
	void play(std::set<Card>played);
	bool is_empty();
	static unsigned int card_to_bool(Card input);
	static unsigned int card_to_bool(unsigned int suit, unsigned int num);
	static std::pair<unsigned int, unsigned int>bool_to_card(unsigned int input);
};
#endif