#include "App/SpiderGame.h"

#include "App/Dealing.h"
#include "App/TestConfigs.h"

#include "Animation/SpiderAnimator.h"
#include "Animation/SpiderNonAnimator.h"
#include "Model/MoveRecord.h"
#include "View/Card.h"
#include "View/CardStack.h"
#include "View/Sprite.h"
#include "View/SpriteFactory.h"
#include "View/SpriteRenderer.h"
#include "View/StackSlot.h"

#include "MyGL/GLErrors.h"


SpiderGame::SpiderGame(SpriteFactory& spriteFactory, SpriteRenderer& spriteRenderer, SpiderAnimator& animation)
	: m_spriteFactory(spriteFactory)
	, m_spriteRenderer(spriteRenderer)
	, m_spiderAnimator(animation)
	, m_busy(false)
{
	Init_Stacks();
	Init_Cards();
}


SpiderGame::~SpiderGame()
{
}

void SpiderGame::Init(int numSuits, int seed)
{
}

void SpiderGame::Reshuffle()
{
	if (m_busy)
		return;

	std::vector<Card*> allCards = CollectAllCards();

	for (Card* card : allCards)
		card->TurnFaceUp();

	Dealing::Shuffle(allCards);
	m_busy = true;
	DealGame(allCards);
}

CardStack& SpiderGame::getStack(int slot)
{
	return *m_stacks[slot];
}

CardStack& SpiderGame::getStock()
{
	return *m_stock;
}

CardStack& SpiderGame::getDonePacks()
{
	return *m_donePacks;
}

void SpiderGame::Init_Cards()
{
	m_cards = Dealing::CreateDeck(2, m_spriteFactory, m_spriteRenderer);

	std::vector<Card*> cards;
	for (auto card : m_cards)
		cards.push_back(card.get());

	Dealing::Shuffle(cards);
	DealGame(cards);
}


void SpiderGame::Init_Stacks()
{
	// Create the Slots and their Stacks
	// Create the Stacks first
	for (int slotNumber = 0; slotNumber < 10; ++slotNumber)
	{
		m_stacks[slotNumber] = std::make_unique<CardStack>(CardStack::Type::Slot, slotNumber);
		m_stacks[slotNumber]->setIsDropTarget(true);
	}

	const float leftBorder = 180.0f;
	const float topBorder = 20.0f;

	const float slotWidth = 220.0f;
	const float donePile_x = leftBorder + (slotWidth * 10.2);
	const float donePile_y = 400.0f;
	const float stockPile_x = leftBorder + (slotWidth * 10.2);
	const float stockPile_y = 900.0f;

	// Create the Slots and attach the stacks.
	for (int slotNumber = 0; slotNumber < 10; ++slotNumber)
	{
		auto slot = m_spriteFactory.CreateStackSlot(*m_stacks[slotNumber]);
		slot->setSlotNumber(slotNumber);
		slot->setPosition({ leftBorder + (slotWidth * slotNumber), topBorder });
		slot->setSize({ 200.0f, 280.0f });
		m_spriteRenderer.AddStackSlot(slot);
	}

	// Create and place the "done" pile.
	m_donePacks = std::make_unique<CardStack>(CardStack::Type::Packs);
	m_donePacks->setPosition({ donePile_x, donePile_y });
	m_donePacks->setIsDropTarget(false);
	m_donePacks->setFaceUpSpacing(1.0f);
	m_donePacks->setGapSpacingInterval(13);
	m_donePacks->setGapSpace(60.0f);

	// Create and place the "stock" pile.
	m_stock = std::make_unique<CardStack>(CardStack::Type::Stock);
	m_stock->setPosition({ stockPile_x, stockPile_y });
	m_stock->setIsDropTarget(false);
	m_stock->setFaceDownSpacing(2.0f);
	m_stock->setGapSpacingInterval(10);
	m_stock->setGapSpace(16.0f);
}


void SpiderGame::SetupStock(std::vector<Card*>& cards)
{
	for(Card* card: cards)
	{
		card->TurnFaceDown();
		m_stock->AddCard(*card);
		card->BringToTop();
	}
	cards.clear();
}


// CollectAllCards is used for "re-dealing"
std::vector<Card*> SpiderGame::CollectAllCards()
{
	std::vector<Card*> allCards = m_donePacks->RemoveCards(0);

	std::vector<Card*> cards = m_stock->RemoveCards(0);
	std::copy(std::begin(cards), std::end(cards), std::back_inserter(allCards));

	for (int slot = 0; slot < 10; ++slot)
	{
		std::vector<Card*> cards = m_stacks[slot]->RemoveCards(0);
		std::copy(std::begin(cards), std::end(cards), std::back_inserter(allCards));
	}
	return allCards;
}


void SpiderGame::DealGame(std::vector<Card*>& cards)
{
	m_undoStack.clear();
	SetupStock(cards);

	bool* busyFlag = &this->m_busy;
	m_spiderAnimator.DealGame(*this, [busyFlag]() { *busyFlag = false; });
}


namespace
{
	CardStack* CardStackFromSprite(Sprite& sprite)
	{
		Card* card = dynamic_cast<Card*>(&sprite);
		if (card != nullptr)
			return card->getParentStack();

		StackSlot* slot = dynamic_cast<StackSlot*>(&sprite);
		if (slot != nullptr)
			return &slot->getCardStack();

		return nullptr;
	}
}


void SpiderGame::DropDragStack(std::unique_ptr<CardStack> dragStack, Sprite* target, CardStack& returnStack)
{
	// If move is not possible.  Put the cards back.
	if (target == nullptr || !target->IsDropTarget() || !target->CanDrop(*dragStack))
	{
		// The animation takes some time and needs to take ownership of the dragStack.
		m_spiderAnimator.SimpleStackMove(std::move(dragStack), returnStack);
		return;
	}

	CardStack* targetStack = CardStackFromSprite(*target);
	assert(targetStack != nullptr);

	// Allocate a StackMoveEvent for the undo list.
	StackMoveRecord record_in;

	record_in.srcStack = returnStack.getSlotNumber();
	record_in.srcCard = returnStack.getCountOfCards();
	record_in.destStack = targetStack->getSlotNumber();
	record_in.destCard = targetStack->getCountOfCards();

	m_busy = true;
	m_spiderAnimator.GameStackMove(*this, record_in, std::move(dragStack),
		[this](const StackMoveRecord& record_out) { this->StackMoveDone(record_out); });
}


void SpiderGame::Deal()
{
	m_spiderAnimator.Deal10(*this, [this](const DealMoveRecord record_out)
		{ this->DealMoveDone(record_out); });
}

bool SpiderGame::Hint()
{
	return false;
}

bool SpiderGame::Undo()
{
	if (m_busy)
		return false;

	if (m_undoStack.empty())
		return false;

	MoveRecord& rec = *m_undoStack.back();
	switch (rec.getType())
	{
	case MoveRecord::Type::Deal:
	{
		DealMoveRecord& deal_rec = static_cast<DealMoveRecord&>(rec);
		if (Undo_Deal(deal_rec))
		{
			m_undoStack.pop_back();
			return true;
		}
	}
	break;
	case MoveRecord::Type::StackMove:
	{
		StackMoveRecord& stack_rec = static_cast<StackMoveRecord&>(rec);
		if (Undo_StackMove(stack_rec))
		{
			m_undoStack.pop_back();
			return true;
		}
	}
	break;
	}
	return false;
}


bool SpiderGame::Undo_StackMove(const StackMoveRecord& rec)
{
	m_busy = true;
	m_spiderAnimator.GameStackMove_Undo(*this, rec, [this]() {this->Undo_Done(); });
	return true;
}


bool SpiderGame::Undo_Deal(const DealMoveRecord& rec)
{
	m_busy = true;
	m_spiderAnimator.Deal10_Undo(*this, [this]() {this->Undo_Done(); });
	return true;
}


void SpiderGame::Undo_Done()
{
	m_busy = false;
}


void SpiderGame::StackMoveDone(const StackMoveRecord& rec)
{
	std::cout << "Stack Move stored on Undo Stack" << std::endl;

	// Don't add null moves to the undo stack.
	if (rec.srcStack != rec.destStack)
	{
		auto rec_ptr = std::make_unique<StackMoveRecord>(rec);
		m_undoStack.push_back(std::move(rec_ptr));
	}
	m_busy = false;
}


void SpiderGame::DealMoveDone(const DealMoveRecord& rec)
{
	std::cout << "Deal Move stored on Undo Stack" << std::endl;
	auto rec_ptr = std::make_unique<DealMoveRecord>(rec);
	m_undoStack.push_back(std::move(rec_ptr));
}


// ======  Debugging assistance =======

namespace
{
	Card* FindAndRemoveCard(std::vector<Card*>& allCards, const CardModel& c)
	{
		auto match_card = [c](Card* card)
		{
			return card->getRank() == c.getRank() && card->getSuit() == c.getSuit();
		};

		auto card_itr = std::find_if(allCards.begin(), allCards.end(), match_card);
		assert(card_itr != allCards.end());
		Card* card = *card_itr;
		allCards.erase(card_itr);
		return card;
	}
}


void SpiderGame::SetStackTestConfig(std::vector<Card*>& allCards, const std::vector<CardModel>& source, CardStack& target)
{
	for (const CardModel& c : source)
	{
		CardModel cardPeek = c;
		cardPeek.TurnFaceUp();
		Card* card = FindAndRemoveCard(allCards, cardPeek);
		if (c.IsFaceUp())
			card->TurnFaceUp();
		else
			card->TurnFaceDown();
		target.AddCard(*card);
		card->BringToTop();
	}
}


void SpiderGame::SetTestConfig(int num)
{
	if (m_busy)
		return;

	std::vector<Card*> allCards = CollectAllCards();

	for (Card* card : allCards)
		card->TurnFaceUp();

	// Re-deploy the cards per the pattern given.
	TestConfigs::Tableau* pattern = &TestConfigs::One;

	SetStackTestConfig(allCards, pattern->m_stock, *m_stock);
	SetStackTestConfig(allCards, pattern->m_packs, *m_donePacks);
	for (int slot = 0; slot < 10; ++slot)
	{
		SetStackTestConfig(allCards, pattern->m_stacks[slot], *m_stacks[slot]);
	}
}
