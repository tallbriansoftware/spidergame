#include "App/Dealing.h"

#include "Model/Deck.h"
#include "View/SpriteFactory.h"
#include "View/SpriteRenderer.h"
#include "Utils/Random.h"


std::vector<std::shared_ptr<Card>> Dealing::CreateDeck(int numSuites, SpriteFactory& factory, SpriteRenderer& renderer)
{
	auto deck = Deck::Create(numSuites);

	std::vector<std::shared_ptr<Card>> cards;

	for (auto& cardModel : deck)
	{
		auto card = factory.CreateCard(cardModel);
		cards.push_back(card);
		renderer.AddCard(card);
	}

	return cards;
}

namespace
{
	void swap(Card*& a, Card*& b)
	{
		auto t = a;
		a = b;
		b = t;
	}
}

void Dealing::Shuffle(std::vector<Card*>& deck)
{
	// Fisher-Yates Shuffle
	int length = (int)deck.size();
	Random rand;

	for (int i = length - 1; i > 0; i--)
	{
		int j = rand.Range(0, i+1);  // Range returns [a ... b) we need [a ... b]
		swap(deck[i], deck[j]);
	}
}
