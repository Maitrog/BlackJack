#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib> 

class Card
{
public:
	enum CardSuit
	{
		SUIT_CLUB,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_SPADE,
		MAX_SUITS
	};
	enum CardRank
	{
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_10,
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		MAX_RANKS
	};
private:
	CardRank m_rank;
	CardSuit m_suit;
public:
	Card(CardRank rank = MAX_RANKS, CardSuit suit = MAX_SUITS):m_rank(rank), m_suit(suit)
	{
	}
	void printCard() const
	{
		switch (m_rank)
		{
		case RANK_2:		std::cout << '2'; break;
		case RANK_3:		std::cout << '3'; break;
		case RANK_4:		std::cout << '4'; break;
		case RANK_5:		std::cout << '5'; break;
		case RANK_6:		std::cout << '6'; break;
		case RANK_7:		std::cout << '7'; break;
		case RANK_8:		std::cout << '8'; break;
		case RANK_9:		std::cout << '9'; break;
		case RANK_10:		std::cout << 'T'; break;
		case RANK_JACK:		std::cout << 'J'; break;
		case RANK_QUEEN:	std::cout << 'Q'; break;
		case RANK_KING:		std::cout << 'K'; break;
		case RANK_ACE:		std::cout << 'A'; break;
		}
		switch (m_suit)
		{
		case SUIT_CLUB:		std::cout << "C "; break;
		case SUIT_DIAMOND:	std::cout << "D "; break;
		case SUIT_HEART:	std::cout << "H "; break;
		case SUIT_SPADE:	std::cout << "S "; break;
		}
	}

	int getCardValue() const
	{
		switch (m_rank)
		{
		case RANK_2:		return 2;
		case RANK_3:		return 3;
		case RANK_4:		return 4;
		case RANK_5:		return 5;
		case RANK_6:		return 6;
		case RANK_7:		return 7;
		case RANK_8:		return 8;
		case RANK_9:		return 9;
		case RANK_10:		return 10;
		case RANK_JACK:		return 10;
		case RANK_QUEEN:	return 10;
		case RANK_KING:		return 10;
		case RANK_ACE:		return 11;
		}

		return 0;
	}
};

class Deck
{
private:
	std::vector<Card> m_deck;
	int m_cardIndex = 0;
public:
	Deck()
	{
		int card = 0;
		for (size_t suit = 0; suit < Card::MAX_SUITS; ++suit)
			for (size_t rank = 0; rank < Card::MAX_RANKS; ++rank)
			{
				Card c(static_cast<Card::CardRank>(rank), static_cast<Card::CardSuit>(suit));
				m_deck.push_back(c);
				++card;
			}
	}
	void printDeck() const
	{
		for (const auto &card : m_deck)
		{
			card.Card::printCard();
			std::cout << ' ';
		}

		std::cout << '\n';
	}
	void swapCard(Card &a, Card &b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}
	void shuffleDeck()
	{
		// Перебираем каждую карту в колоде
		for (int index = 51; index != 0 ; --index)
		{
			// Выбираем любую случайную карту
			int swapIndex = rand() % index;
			// Меняем местами с нашей текущей картой
			swapCard(m_deck[index], m_deck[swapIndex]);
		}
		m_cardIndex = 0;
	}
	Card& dealCard()
	{
		++m_cardIndex;
		return m_deck[m_cardIndex-1];
	}
};

class Player
{
private:
	std::vector<Card> m_heand;
	double m_money;
public:
	Player()
	{
		m_money = 100;
	}
	double getPlayerMoney() const { return m_money; }
	void setPlayerMoney(double money) { m_money = money; }
	void setPlayerHeand(std::vector<Card> heand) { m_heand = heand; }
	std::vector<Card> getPlayerHeand() { return m_heand; }
	void printPlayerHeand() const 
	{
		std::cout << "Your heand: ";
		for (Card card : m_heand)
			card.printCard();
		std::cout << std::endl;
	}
	int getTotal() const
	{
		int total = 0;
		short flag = 0;
		for (Card i : m_heand)
		{
			if (i.getCardValue() == 11 && flag == 1)
				++total;
			else
				total += i.getCardValue();
			if (i.getCardValue() == 11)
				flag = 1;
		}
		return total;
	}
	void push_backCard(Card card) { m_heand.push_back(card); }
	void clearPlayerHeand() { m_heand.clear(); }
};


char getPlayerChoice()
{
	std::cout << "(h) to hit, or (s) to stand: ";
	char choice;
	do
	{
		std::cin >> choice;
	} while (choice != 'h' && choice != 's');

	return choice;
}

short playBlackjack(Deck& deck, Player& player)
{
	std::vector<Card> heand;
	int playerTotal = 0;
	int dealerTotal = 0;
	int cardCount = 0;

	// Дилер получает одну карту
	dealerTotal += deck.Deck::dealCard().Card::getCardValue();
	std::cout << "The dealer is showing: " << dealerTotal << '\n';

	// Игрок получает две карты
	player.push_backCard(deck.Deck::dealCard());
	++cardCount;
	player.push_backCard(deck.Deck::dealCard());
	++cardCount;
	playerTotal = player.getTotal();
	player.printPlayerHeand();
	std::cout << "You have: " << playerTotal << '\n';


	// Игрок начинает
	while (1)
	{
		char choice = getPlayerChoice();
		if (choice == 's')
			break;

		player.push_backCard(deck.Deck::dealCard());
		playerTotal = player.getTotal();
		++cardCount;

		player.printPlayerHeand();
		std::cout << "You have: " << playerTotal << '\n';

		// Смотрим, не проиграл ли игрок
		if (playerTotal > 21)
			return -1;
	}

	// Если игрок не проиграл (у него не больше 21), тогда дилер получает карты до тех пор, пока у него в сумме будет не меньше 17
	while (dealerTotal < 17)
	{
		dealerTotal += deck.Deck::dealCard().Card::getCardValue();
		std::cout << "The dealer now has: " << dealerTotal << '\n';
	}

	// Если у дилера больше 21 - он проиграл. Игрок выиграл
	if (dealerTotal > 21)
		return 1;
	
	// 1 - игрок выйграл, -1 - игрок проиграл, 0 - ничья
	if (playerTotal > dealerTotal)
		return 1;
	else if (playerTotal < dealerTotal)
		return -1;
	else return 0;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();
	Player player;
	Deck deck;
	char choice;
	double bet = 0;
	short result = 0;
	while (1)
	{
		std::cout << "Your money: " << player.getPlayerMoney() << std::endl;
		std::cout << "Do you want to playdo you want to play? (y) - yes, (n) - no\n";
		std::cin >> choice;
		if (choice == 'y')
		{
			std::cout << "Enter your bet: ";
			std::cin >> bet;
			deck.shuffleDeck();
			result = playBlackjack(deck, player);
			if (result == 1)
			{
				std::cout << "You win!\n\n";
				player.setPlayerMoney(player.getPlayerMoney() + bet);
				player.clearPlayerHeand();
			}
			else 
				if (result == -1)
				{
					std::cout << "You lose!\n\n";
					player.setPlayerMoney(player.getPlayerMoney() - bet);
					player.clearPlayerHeand();
				}
					else
					{
						std::cout << "Draw";
						player.clearPlayerHeand();
					}
		}
		else
			break;
	}
	return 0;
}