#ifndef CARD_H
#define CARD_H
#include<vector>
#include<string>
#include <SFML/Graphics.hpp>
class Card {
private:
	unsigned int suit;
	unsigned int num;
	static sf::Texture tex[52];
	static unsigned int suitcount;
	static unsigned int numcount;
	std::vector<std::string>suits = { "clubs","diamonds","hearts","spades" };
	std::vector<std::string>nums = { "NULL","A","2","3","4","5","6","7","8","9","10","J","Q","K" };
public:
	static void setpic();
	unsigned int cardnum()const;
	void dis(sf::RenderWindow* window, float x, float y,float s)const;
	Card(unsigned int suit, unsigned int num);
	static void reset();
	Card();
	unsigned int getnum()const;
	unsigned int getsuit()const;
	std::string getsuitstring()const;
	std::string getfname()const;
	void print();
	bool operator<(Card const& card2)const;
	bool operator>(Card const& card2)const;
	bool operator==(Card const& card2)const;
	bool operator!=(Card const& card2)const;
	static Card pair_return_max(Card a, Card b);
};
#endif