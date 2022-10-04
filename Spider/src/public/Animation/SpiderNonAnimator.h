#pragma once

class SpiderGame;


class SpiderNonAnimator
{
public:
    SpiderNonAnimator();
    ~SpiderNonAnimator();

    void DealGame(SpiderGame& game);

private:
    void DealOneCard(SpiderGame& game, int slot, bool faceUp);

};
