#include<vector>
#include<set>
#include<utility>
#include<iostream>
#include"Card.h"
#include"CardSet.h"
CardSet::CardSet(){}
CardSet::CardSet(std::set<Card>input) { 
	hand = input;
	scan_all(true);
}
std::set<Card> CardSet::getset()const { return hand; }
unsigned int CardSet::get_hand_count() {
	return hand.size();
}
bool CardSet::club_3_included() {
	return hand.find(Card(0, 3)) != hand.end();
}
void CardSet::delete_from_count_remainder(std::set<Card>to_delete) {
	for(auto c : to_delete){
		count_remainder.erase(c);
	}	
}
void CardSet::delete_from_hand(std::set<Card>to_delete) {
	for (auto c : to_delete) {
		hand.erase(c);
	}
}
void CardSet::scan_deck(std::set<Card>on_deck) {
	delete_from_count_remainder(on_deck);
	scan_all(false);
}
void CardSet::play(std::set<Card>played) {
	delete_from_hand(played);
	scan_all(false);
}
bool CardSet::is_empty() { return hand.empty(); }
unsigned int CardSet::card_to_bool(Card input) { return (13 * input.getsuit() + input.getnum() - 1); }
unsigned int CardSet::card_to_bool(unsigned int suit, unsigned int num) { return card_to_bool(Card(suit, num)); }
std::pair<unsigned int, unsigned int>CardSet::bool_to_card(unsigned int input) { return { input / 13, input % 13 + 1 }; }
void CardSet::scan_all(bool initialize_it){
	scan_singles(initialize_it);
	scan_pairs();
	scan_straights();
	scan_full_houses();
	scan_straight_flushes();
	scan_four_of_a_kinds();

}
void CardSet::scan_singles(bool initialize_it) {//include producing count_remainder
	if (initialize_it) {
		for (int i = 0;i < 52;i++) { Card card;count_remainder.insert(card); }
		for (auto c : hand)count_remainder.erase(c);
		single.first = hand;
		single.second = count_remainder;
	}
	else {
		single.first = hand;
		single.second = count_remainder;
	}
}
void CardSet::scan_pairs() {//ЧΘ
	if (false) {
		for (int i = 0;i < 13;i++)Card dummies;//skip clubs
		for (int i = 0;i < 39;i++) { Card card; pair.second.insert(card); }
		std::vector<std::vector<Card>>count(14, std::vector<Card>(0));//counting each numbers. count[0] is empty
		for (auto c : hand)count[c.getnum()].push_back(c);
		for (int i = 1;i <= 13;i++) {//add the biggest, delete card pair in hand from deck 
			if (count[i].size() == 1) {
				pair.second.erase(count[i][0]);
				if (count[i][0].getsuit() == 0)pair.second.erase(Card(1, count[i][0].getnum()));
			}
			else if (count[i].size() == 2) { 
				pair.first.insert(count[i][1]);
				pair.second.erase(count[i][0]);
				pair.second.erase(count[i][1]);//璝ΤClubpair, 惠璶盢逞緇耕礟埃EX:0(club),3(spade) -> erase 1(diamond), 3
				if (count[i][0].getsuit() == 0) { int mc = (count[i][1].getsuit() == 1) ? 2 : 1; pair.second.erase(Card(mc,i)); }
			}
			else if (count[i].size() >= 3) {
				for (int s = 0;s < 4;s++)pair.second.erase(Card(s, i));
				for (int s = 1;s < count[i].size();s++)pair.first.insert(count[i][s]);
			}
		}
	}
	else if (true) {//э跑礟┦
		std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
		for (auto c : count_remainder)count[c.getnum()].push_back(c);
		pair.second.clear();
		for (int i = 1;i <= 13;i++) { 
			if (count[i].size() == 2) {
				pair.second.insert(count[i][1]);
			}
			else if (count[i].size() == 3) {
				pair.second.insert(count[i][1]);
				pair.second.insert(count[i][2]);
			}
			else if (count[i].size() == 4) {
				pair.second.insert(count[i][1]);
				pair.second.insert(count[i][2]);
				pair.second.insert(count[i][3]);
			}
		}
		std::vector<std::vector<Card>>counth(14, std::vector<Card>(0));
		for (auto c : hand)counth[c.getnum()].push_back(c);
		pair.first.clear();
		for (int i = 1;i <= 13;i++) {
			if (counth[i].size() == 2) {
				pair.first.insert(counth[i][1]);
			}
			else if (counth[i].size() == 3) {
				pair.first.insert(counth[i][1]);
				pair.first.insert(counth[i][2]);
			}
			else if (counth[i].size() == 4) {
				pair.first.insert(counth[i][1]);
				pair.first.insert(counth[i][2]);
				pair.first.insert(counth[i][3]);
			}
		}
	}
	else {//э跑礟┦
		std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
		for (auto c : hand)count[c.getnum()].push_back(c);
		pair.first.clear();
		for (int i = 1;i <= 13;i++) {
			if (count[i].size() == 2) {
				pair.first.insert(count[i][1]);
			}
			else if (count[i].size() == 3) {
				pair.first.insert(count[i][1]);
				pair.first.insert(count[i][2]);
			}
			else if (count[i].size() == 4) {
				pair.first.insert(count[i][1]);
				pair.first.insert(count[i][2]);
				pair.first.insert(count[i][3]);
			}
		}
	}
}
void CardSet::scan_straights() {
	if (true) {//硄ノ
		straight.first.clear();
		straight.second.clear();
		std::vector<std::vector<Card>>count_hand(14, std::vector<Card>(0));
		std::vector<std::vector<Card>>count_deck(14, std::vector<Card>(0));
		bool hand_possi[14] = { 1,1,1,0,0,0,0,1,1,1,1,1,1,1 };//魁礟ゑ膀非straights籔
		bool deck_possi[14] = { 1,1,1,0,0,0,0,1,1,1,1,1,1,1 };
		for (auto c : hand)count_hand[c.getnum()].push_back(c);//璸计
		for (auto c : count_remainder)count_deck[c.getnum()].push_back(c);
		for (int i = 1;i <= 13;i++) {
			if (count_hand[i].size() == 0 && i >= 2 && i <= 9)for (int j = i;j <= i + 4;j++)hand_possi[j] = 0;//璝2-9ヴ计秖0ㄤ计+4ゑ膀非straightsぃ
			else if (count_hand[i].size() == 0 && i >= 10) {//Aゑ膀非straights
				for (int j = i;j <= i + 4;j++)
					if (j < 14)hand_possi[j] = 0;
				hand_possi[1] = 0;
			}
			if (count_hand[i].size() == 0 && i == 1)hand_possi[1] = 0;//Aセō
		}
		for (int i = 1;i <= 13;i++) {
			if (count_deck[i].size() == 0 && i >= 2 && i <= 9)for (int j = i;j <= i + 4;j++)deck_possi[j] = 0;
			else if (count_deck[i].size() == 0 && i >= 10) {
				for (int j = i;j <= i + 4;j++)
					if (j < 14)deck_possi[j] = 0;
				deck_possi[1] = 0;
			}
			if (count_deck[i].size() == 0 && i == 1)deck_possi[1] = 0;
		}
		for (auto c : hand) {
			if (hand_possi[c.getnum()])straight.first.insert(c);
		}
		for (auto c : count_remainder) {
			if (deck_possi[c.getnum()])straight.second.insert(c);
		}
	}
}
void CardSet::scan_full_houses() {
	if (true) {//硄ノ参魁club
		full_house.first.clear();
		full_house.second.clear();
		std::vector<std::vector<Card>>count_hand(14, std::vector<Card>(0));
		std::vector<std::vector<Card>>count_deck(14, std::vector<Card>(0));
		for (auto c : hand)count_hand[c.getnum()].push_back(c);//璸计
		for (auto c : count_remainder)count_deck[c.getnum()].push_back(c);
		for (int i = 1;i <= 13;i++) {
			if (count_hand[i].size() >= 3)
				for (int j = 1;j <= 13;j++)
					if (count_hand[j].size() >= 2 && i != j)full_house.first.insert(count_hand[i][0]);
		}
		for (int i = 1;i <= 13;i++) {
			if (count_deck[i].size() >= 3)
				for (int j = 1;j <= 13;j++)
					if (count_deck[j].size() >= 2 && i != j)full_house.second.insert(count_deck[i][0]);
		}
	}
}
void CardSet::scan_straight_flushes() {
	if (true) {//硄ノ
		straight_flush.first.clear();
		straight_flush.second.clear();
		for (auto c : hand) {
			unsigned int su = c.getsuit();
			unsigned int nu = c.getnum();
			auto en = hand.end();
			if (nu <= 2 &&
				hand.find(Card(su, nu + 1)) != en &&
				hand.find(Card(su, nu + 2)) != en &&
				hand.find(Card(su, nu + 3)) != en &&
				hand.find(Card(su, nu + 4)) != en
				)straight_flush.first.insert(std::max(c, Card(su, nu + 1)));
			else if (nu >= 7 && nu <= 13 &&
				hand.find(Card(su, nu - 1)) != en &&
				hand.find(Card(su, nu - 2)) != en &&
				hand.find(Card(su, nu - 3)) != en &&
				hand.find(Card(su, nu - 4)) != en
				)straight_flush.first.insert(c);
			if (nu == 1 &&
				hand.find(Card(su,13)) != en &&
				hand.find(Card(su,12)) != en &&
				hand.find(Card(su,11)) != en &&
				hand.find(Card(su,10)) != en
				)straight_flush.first.insert(c);
		}
		for (auto c : count_remainder) {
			unsigned int su = c.getsuit();
			unsigned int nu = c.getnum();
			auto en = count_remainder.end();
			if (nu <= 2 &&
				count_remainder.find(Card(su, nu + 1)) != en &&
				count_remainder.find(Card(su, nu + 2)) != en &&
				count_remainder.find(Card(su, nu + 3)) != en &&
				count_remainder.find(Card(su, nu + 4)) != en
				)straight_flush.second.insert(std::max(c, Card(su, nu + 1)));
			else if (nu >= 7 && nu <= 12 &&
				count_remainder.find(Card(su, nu - 1)) != en &&
				count_remainder.find(Card(su, nu - 2)) != en &&
				count_remainder.find(Card(su, nu - 3)) != en &&
				count_remainder.find(Card(su, nu - 4)) != en
				)straight_flush.second.insert(c);
			if (nu == 1 &&
				count_remainder.find(Card(su, 13)) != en &&
				count_remainder.find(Card(su, 12)) != en &&
				count_remainder.find(Card(su, 11)) != en &&
				count_remainder.find(Card(su, 10)) != en
				)straight_flush.second.insert(c);
		}
	}
}
void CardSet::scan_four_of_a_kinds() {
	if (true) {//硄ノ
		four_of_a_kind.first.clear();
		four_of_a_kind.second.clear();
		std::vector<std::vector<Card>>count_hand(14, std::vector<Card>(0));
		std::vector<std::vector<Card>>count_deck(14, std::vector<Card>(0));
		for (auto c : hand)count_hand[c.getnum()].push_back(c);//璸计
		for (auto c : count_remainder)count_deck[c.getnum()].push_back(c);
		for (int i = 1;i <= 13;i++) {
			if (hand.size() >= 5 && count_hand[i].size() == 4)four_of_a_kind.first.insert(count_hand[i][0]);
		}
		for (int i = 1;i <= 13;i++) {
			if (count_remainder.size() >= 5 && count_deck[i].size() == 4)four_of_a_kind.second.insert(count_deck[i][0]);
		}
	}
}
std::vector<std::set<Card>> CardSet::get_singles() {
	std::vector<std::set<Card>>combi;
	std::set<Card>temp;
	for (auto c : hand) {
		temp = { c };
		combi.push_back(temp);
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_pairs() {
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	for (auto c : hand)count[c.getnum()].push_back(c);
	for (int i = 1;i <= 13;i++) {
		if (count[i].size() == 2) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
		}
		else if (count[i].size() == 3) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][2] };
			combi.push_back(temp);
		}
		else if (count[i].size() == 4) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][3] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][3] };
			combi.push_back(temp);
			temp = { count[i][2],count[i][3] };
			combi.push_back(temp);
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_straights() {
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	for (auto c : hand)count[c.getnum()].push_back(c);
	std::vector<bool>count_num(14,false);
	for (auto c : straight.first)count_num[c.getnum()] = true;
	for (int i = 1;i <= 13;i++) {
		if (count_num[i]) {
			if (i >= 7) {
				for (auto fir : count[i])
					for (auto sec : count[i - 1])
						for (auto thr : count[i - 2])
							for (auto fou : count[i - 3])
								for (auto fif : count[i - 4]) {
									temp = { fir,sec,thr,fou,fif };
									combi.push_back(temp);
								}
			}
			else if (i == 1) {
				for (auto fir : count[10])
					for (auto sec : count[11])
						for (auto thr : count[12])
							for (auto fou : count[13])
								for (auto fif : count[1]) {
									temp = { fir,sec,thr,fou,fif };
									combi.push_back(temp);
								}
			}
			else if (i == 2) {
				if (!count[1].empty()) {
					for (auto fir : count[1])
						for (auto sec : count[2])
							for (auto thr : count[3])
								for (auto fou : count[4])
									for (auto fif : count[5]) {
										temp = { fir,sec,thr,fou,fif };
										combi.push_back(temp);
									}
				}
				if (!count[6].empty()) {
					for (auto fir : count[2])
						for (auto sec : count[3])
							for (auto thr : count[4])
								for (auto fou : count[5])
									for (auto fif : count[6]) {
										temp = { fir,sec,thr,fou,fif };
										combi.push_back(temp);
									}
				}
			}
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_full_houses() {
	std::vector<std::set<Card>>pair = get_pairs();
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	for (auto c : hand)count[c.getnum()].push_back(c);
	for (auto c : full_house.first) {
		if (count[c.getnum()].size() == 3) {
			for (auto p : pair) {
				if ((*p.begin()).getnum() != c.getnum()) {
					temp = { count[c.getnum()][0], count[c.getnum()][1], count[c.getnum()][2] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
				}
			}
		}
		else if (count[c.getnum()].size() == 4) {
			for (auto p : pair) {
				if ((*p.begin()).getnum() != c.getnum()) {
					temp = { count[c.getnum()][0], count[c.getnum()][1], count[c.getnum()][2] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][1], count[c.getnum()][2], count[c.getnum()][3] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][2], count[c.getnum()][3], count[c.getnum()][0] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][3], count[c.getnum()][0], count[c.getnum()][1] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
				}
			}
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_straight_flushes() {
	std::vector<std::set<Card>>combi;
	if (!straight_flush.first.empty()) {
		std::vector<std::set<Card>>my_straight;
		my_straight = get_straights();
		for (auto s : my_straight) {
			bool is_flush = true;
			bool f = true;
			int suit;
			for (auto ss : s) {
				if (f) {
					suit = ss.getsuit();
					f = false;
				}
				else {
					if (suit != ss.getsuit())is_flush = false;
				}
			}
			if (is_flush)combi.push_back(s);
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_four_of_a_kinds() {
	std::vector<std::set<Card>>combi;
	if (!four_of_a_kind.first.empty()) {
		std::set<Card>excluded;
		std::set<Card>temp;
		for (auto s : four_of_a_kind.first) {
			excluded = hand;
			for (int i = 0;i <= 3;i++)excluded.erase(Card(i, s.getnum()));
			for (auto c : excluded) {
				temp = { c,Card(0, s.getnum()), Card(1, s.getnum()), Card(2, s.getnum()), Card(3, s.getnum()) };
				combi.push_back(temp);
			}
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_singles(std::set<Card>input) {
	std::vector<std::set<Card>>combi;
	std::set<Card>temp;
	for (auto c : input) {
		temp = { c };
		combi.push_back(temp);
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_pairs(std::set<Card>input) {
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	for (auto c : input)count[c.getnum()].push_back(c);
	for (int i = 1;i <= 13;i++) {
		if (count[i].size() == 2) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
		}
		else if (count[i].size() == 3) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][2] };
			combi.push_back(temp);
		}
		else if (count[i].size() == 4) {
			temp = { count[i][0],count[i][1] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][0],count[i][3] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][2] };
			combi.push_back(temp);
			temp = { count[i][1],count[i][3] };
			combi.push_back(temp);
			temp = { count[i][2],count[i][3] };
			combi.push_back(temp);
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_straights(std::set<Card>input) {
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	for (auto c : input)count[c.getnum()].push_back(c);
	std::vector<bool>count_num(14, false);
	std::vector<std::vector<Card>>count_hand(14, std::vector<Card>(0));
	std::set<Card>straight;
	bool hand_possi[14] = { 1,1,1,0,0,0,0,1,1,1,1,1,1,1 };//魁礟ゑ膀非straights籔
	bool deck_possi[14] = { 1,1,1,0,0,0,0,1,1,1,1,1,1,1 };
	for (auto c : input)count_hand[c.getnum()].push_back(c);//璸计
	for (int i = 1;i <= 13;i++) {
		if (count_hand[i].size() == 0 && i >= 2 && i <= 9)for (int j = i;j <= i + 4;j++)hand_possi[j] = 0;//璝2-9ヴ计秖0ㄤ计+4ゑ膀非straightsぃ
		else if (count_hand[i].size() == 0 && i >= 10) {//Aゑ膀非straights
			for (int j = i;j <= i + 4;j++)
				if (j < 14)hand_possi[j] = 0;
			hand_possi[1] = 0;
		}
		if (count_hand[i].size() == 0 && i == 1)hand_possi[1] = 0;//Aセō
	}
	for (auto c : input) {
		if (hand_possi[c.getnum()])straight.insert(c);
	}
	for (auto c : straight)count_num[c.getnum()] = true;
	for (int i = 1;i <= 13;i++) {
		if (count_num[i]) {
			if (i >= 7) {
				for (auto fir : count[i])
					for (auto sec : count[i - 1])
						for (auto thr : count[i - 2])
							for (auto fou : count[i - 3])
								for (auto fif : count[i - 4]) {
									temp = { fir,sec,thr,fou,fif };
									combi.push_back(temp);
								}
			}
			else if (i == 1) {
				for (auto fir : count[10])
					for (auto sec : count[11])
						for (auto thr : count[12])
							for (auto fou : count[13])
								for (auto fif : count[1]) {
									temp = { fir,sec,thr,fou,fif };
									combi.push_back(temp);
								}
			}
			else if (i == 2) {
				if (!count[1].empty()) {
					for (auto fir : count[1])
						for (auto sec : count[2])
							for (auto thr : count[3])
								for (auto fou : count[4])
									for (auto fif : count[5]) {
										temp = { fir,sec,thr,fou,fif };
										combi.push_back(temp);
									}
				}
				if (!count[6].empty()) {
					for (auto fir : count[2])
						for (auto sec : count[3])
							for (auto thr : count[4])
								for (auto fou : count[5])
									for (auto fif : count[6]) {
										temp = { fir,sec,thr,fou,fif };
										combi.push_back(temp);
									}
				}
			}
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_full_houses(std::set<Card>input) {
	std::vector<std::set<Card>>pair = get_pairs(input);
	std::vector<std::set<Card>>combi;
	std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
	std::set<Card>temp;
	std::set<Card>full_house;
	for (auto c : input)count[c.getnum()].push_back(c);
	for (int i = 1;i <= 13;i++) {
		if (count[i].size() >= 3)
			for (int j = 1;j <= 13;j++)
				if (count[j].size() >= 2 && i != j)full_house.insert(count[i][0]);
	}
	for (auto c : full_house) {
		if (count[c.getnum()].size() == 3) {
			for (auto p : pair) {
				if ((*p.begin()).getnum() != c.getnum()) {
					temp = { count[c.getnum()][0], count[c.getnum()][1], count[c.getnum()][2] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
				}
			}
		}
		else if (count[c.getnum()].size() == 4) {
			for (auto p : pair) {
				if ((*p.begin()).getnum() != c.getnum()) {
					temp = { count[c.getnum()][0], count[c.getnum()][1], count[c.getnum()][2] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][1], count[c.getnum()][2], count[c.getnum()][3] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][2], count[c.getnum()][3], count[c.getnum()][0] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
					temp = { count[c.getnum()][3], count[c.getnum()][0], count[c.getnum()][1] };
					for (auto pp : p)temp.insert(pp);
					combi.push_back(temp);
				}
			}
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_straight_flushes(std::set<Card>input) {
	std::vector<std::set<Card>>combi;
	if (true) {
		std::vector<std::set<Card>>my_straight;
		my_straight = get_straights(input);
		for (auto s : my_straight) {
			bool is_flush = true;
			bool f = true;
			int suit;
			for (auto ss : s) {
				if (f) {
					suit = ss.getsuit();
					f = false;
				}
				else {
					if (suit != ss.getsuit())is_flush = false;
				}
			}
			if (is_flush)combi.push_back(s);
		}
	}
	return combi;
}
std::vector<std::set<Card>> CardSet::get_four_of_a_kinds(std::set<Card>input) {
	std::vector<std::set<Card>>combi;
	if (true) {
		std::set<Card>excluded;
		std::set<Card>temp;
		std::set<Card>four_of_a_kind;
		std::vector<std::vector<Card>>count(14, std::vector<Card>(0));
		for (auto c : input)count[c.getnum()].push_back(c);
		for (int i = 1;i <= 13;i++) {
			if (input.size() >= 5 && count[i].size() == 4)four_of_a_kind.insert(count[i][0]);
		}
		for (auto s : four_of_a_kind) {
			excluded = input;
			for (int i = 0;i <= 3;i++)excluded.erase(Card(i, s.getnum()));
			for (auto c : excluded) {
				temp = { c,Card(0, s.getnum()), Card(1, s.getnum()), Card(2, s.getnum()), Card(3, s.getnum()) };
				combi.push_back(temp);
			}
		}
	}
	return combi;
}