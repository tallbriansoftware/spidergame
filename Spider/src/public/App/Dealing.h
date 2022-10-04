#pragma once

#include "View/Card.h"

#include <memory>
#include <vector>

class SpriteFactory;
class SpriteRenderer;
class Card;

namespace Dealing
{
    std::vector<std::shared_ptr<Card>> CreateDeck(int numSuites, SpriteFactory& factory, SpriteRenderer& renderer);
    void Shuffle(std::vector<Card*>& deck);
}