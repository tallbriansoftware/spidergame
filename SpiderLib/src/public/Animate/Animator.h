#pragma once

#include "Animate/Animation.h"

#include <memory>
#include <vector>

class GameTime;

class Animator
{
public:
    Animator();
    ~Animator();

public:
    Animation* CreateAnimation(float from, float to, int64_t durationMilliSec,
        Animation::MotionCB motion, Animation::CompletionCB done);
    void StartAnimation(Animation& anim, int64_t delayMilliSec = 0);
    void Render(int64_t timeMilliSec);
    int64_t NowMilliSec();

private:
    int64_t m_lastRenderTime;

    std::vector<std::unique_ptr<Animation>> m_animationHolder;
    std::vector<std::unique_ptr<Animation>> m_animationQueue;
};
