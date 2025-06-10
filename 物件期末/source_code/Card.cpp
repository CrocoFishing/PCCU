#include"Card.h"
#include<iostream>
#include<vector>
#include<string>
#include<set>
#include <SFML/Graphics.hpp>
unsigned int Card::suitcount=0;
unsigned int Card::numcount=1;
sf::Texture Card::tex[52];
unsigned int Card::cardnum() const { return (13 * getsuit() + getnum() - 1); }
void Card::setpic() {
	tex[0] = sf::Texture("clubs_A.png");
	tex[0].setSmooth(true);

	tex[1] = sf::Texture("clubs_2.png");
	tex[1].setSmooth(true);

	tex[2] = sf::Texture("clubs_3.png");
	tex[2].setSmooth(true);

	tex[3] = sf::Texture("clubs_4.png");
	tex[3].setSmooth(true);

	tex[4] = sf::Texture("clubs_5.png");
	tex[4].setSmooth(true);

	tex[5] = sf::Texture("clubs_6.png");
	tex[5].setSmooth(true);

	tex[6] = sf::Texture("clubs_7.png");
	tex[6].setSmooth(true);

	tex[7] = sf::Texture("clubs_8.png");
	tex[7].setSmooth(true);

	tex[8] = sf::Texture("clubs_9.png");
	tex[8].setSmooth(true);

	tex[9] = sf::Texture("clubs_10.png");
	tex[9].setSmooth(true);

	tex[10] = sf::Texture("clubs_J.png");
	tex[10].setSmooth(true);

	tex[11] = sf::Texture("clubs_Q.png");
	tex[11].setSmooth(true);

	tex[12] = sf::Texture("clubs_K.png");
	tex[12].setSmooth(true);

	// Diamonds
	tex[13] = sf::Texture("diamonds_A.png");
	tex[13].setSmooth(true);

	tex[14] = sf::Texture("diamonds_2.png");
	tex[14].setSmooth(true);

	tex[15] = sf::Texture("diamonds_3.png");
	tex[15].setSmooth(true);

	tex[16] = sf::Texture("diamonds_4.png");
	tex[16].setSmooth(true);

	tex[17] = sf::Texture("diamonds_5.png");
	tex[17].setSmooth(true);

	tex[18] = sf::Texture("diamonds_6.png");
	tex[18].setSmooth(true);

	tex[19] = sf::Texture("diamonds_7.png");
	tex[19].setSmooth(true);

	tex[20] = sf::Texture("diamonds_8.png");
	tex[20].setSmooth(true);

	tex[21] = sf::Texture("diamonds_9.png");
	tex[21].setSmooth(true);

	tex[22] = sf::Texture("diamonds_10.png");
	tex[22].setSmooth(true);

	tex[23] = sf::Texture("diamonds_J.png");
	tex[23].setSmooth(true);

	tex[24] = sf::Texture("diamonds_Q.png");
	tex[24].setSmooth(true);

	tex[25] = sf::Texture("diamonds_K.png");
	tex[25].setSmooth(true);

	// Hearts
	tex[26] = sf::Texture("hearts_A.png");
	tex[26].setSmooth(true);

	tex[27] = sf::Texture("hearts_2.png");
	tex[27].setSmooth(true);

	tex[28] = sf::Texture("hearts_3.png");
	tex[28].setSmooth(true);

	tex[29] = sf::Texture("hearts_4.png");
	tex[29].setSmooth(true);

	tex[30] = sf::Texture("hearts_5.png");
	tex[30].setSmooth(true);

	tex[31] = sf::Texture("hearts_6.png");
	tex[31].setSmooth(true);

	tex[32] = sf::Texture("hearts_7.png");
	tex[32].setSmooth(true);

	tex[33] = sf::Texture("hearts_8.png");
	tex[33].setSmooth(true);

	tex[34] = sf::Texture("hearts_9.png");
	tex[34].setSmooth(true);

	tex[35] = sf::Texture("hearts_10.png");
	tex[35].setSmooth(true);

	tex[36] = sf::Texture("hearts_J.png");
	tex[36].setSmooth(true);

	tex[37] = sf::Texture("hearts_Q.png");
	tex[37].setSmooth(true);

	tex[38] = sf::Texture("hearts_K.png");
	tex[38].setSmooth(true);

	// Spades
	tex[39] = sf::Texture("spades_A.png");
	tex[39].setSmooth(true);

	tex[40] = sf::Texture("spades_2.png");
	tex[40].setSmooth(true);

	tex[41] = sf::Texture("spades_3.png");
	tex[41].setSmooth(true);

	tex[42] = sf::Texture("spades_4.png");
	tex[42].setSmooth(true);

	tex[43] = sf::Texture("spades_5.png");
	tex[43].setSmooth(true);

	tex[44] = sf::Texture("spades_6.png");
	tex[44].setSmooth(true);

	tex[45] = sf::Texture("spades_7.png");
	tex[45].setSmooth(true);

	tex[46] = sf::Texture("spades_8.png");
	tex[46].setSmooth(true);

	tex[47] = sf::Texture("spades_9.png");
	tex[47].setSmooth(true);

	tex[48] = sf::Texture("spades_10.png");
	tex[48].setSmooth(true);

	tex[49] = sf::Texture("spades_J.png");
	tex[49].setSmooth(true);

	tex[50] = sf::Texture("spades_Q.png");
	tex[50].setSmooth(true);

	tex[51] = sf::Texture("spades_K.png");
	tex[51].setSmooth(true);

}
void Card::dis(sf::RenderWindow* window, float x, float y, float s) const {
	sf::Sprite spr(tex[cardnum()]);
	spr.setOrigin({ 141,170 });
	spr.setPosition({ x,y });
	spr.setScale({s,s});
	window->draw(spr);
}
Card::Card(unsigned int suit, unsigned int num) { this->suit = suit; this->num = num; };
//重製循環
void Card::reset() { suitcount = 0; numcount = 1; }
//無參數實例化物件52次一個循環
Card::Card() {
	suit = suitcount; num = numcount;
	if (numcount == 13 && suitcount == 3)reset();
	else if (numcount == 13) {suitcount++; numcount = 1;}
	else numcount++;
}
unsigned int Card::getnum()const { return num; }
unsigned int Card::getsuit()const { return suit; }
std::string Card::getsuitstring() const{ return suits[suit]; }
std::string Card::getfname()const {
	std::string tore(suits[suit]);
	tore.append("_");
	tore.append(nums[num]);
	tore.append(".png");
	return tore;
}
//印出牌的內容，ex: diamond K
void Card::print() {
	std::string p(suits[suit]); p.append("-"); p.append(nums[num]);
	std::cout << p;
}
//定義operators(<, >, ==, !=)，2 > A > K
bool Card::operator<(Card const& card2)const {
	unsigned num1 = (num <= 2) ? 20 * num : num;
	unsigned num2 = (card2.getnum() <= 2) ? 20 * card2.getnum() : card2.getnum();
	if (num1 < num2)return true;
	else if (num1 == num2 && suit < card2.getsuit())return true;
	else return false;
}
bool Card::operator>(Card const& card2)const {
	unsigned num1 = (num <= 2) ? 20 * num : num;
	unsigned num2 = (card2.getnum() <= 2) ? 20 * card2.getnum() : card2.getnum();
	if (num1 > num2)return true;
	else if (num1 == num2 && suit > card2.getsuit())return true;
	else return false;
}
bool Card::operator==(Card const& card2)const {
	if (num == card2.getnum() && suit == card2.getsuit())return true;
	else return false;
}
bool Card::operator!=(Card const& card2)const {
	if (num != card2.getnum() && suit != card2.getsuit())return true;
	else return false;
}
//useless
Card Card::pair_return_max(Card a, Card b) {
	if (a.num > b.num)return a;
	else if (a.num == b.num && a.suit > b.suit)return a;
	else return b;
}
