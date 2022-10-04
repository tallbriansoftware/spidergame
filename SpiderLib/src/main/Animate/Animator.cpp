#include "Animate/Animator.h"

#include "Animate/Animation.h"

#include "Utils/GameTime.h"
#include <cassert>
#include <iostream>
#include <algorithm>


Animator::Animator()
    : m_lastRenderTime(0)
{
}

Animator::~Animator()
{
}


Animation* Animator::CreateAnimation(
    float from, float to, int64_t durationMilliSec,
    Animation::MotionCB motion, Animation::CompletionCB done)
{
    m_animationHolder.push_back(std::make_unique<Animation>(*this, from, to, durationMilliSec, motion, done));
    Animation* anim = (m_animationHolder.back()).get();

    return anim;
}


void Animator::StartAnimation(Animation& anim, int64_t delayMSecs)
{
    // Remove the animation from the "Holder" list.
    // And add it to the Queued Animation.
    auto itr = find_if(
            m_animationHolder.begin(),
            m_animationHolder.end(),
            [&anim](std::unique_ptr<Animation>& obj) { 
                return obj.get() == &anim; });

    assert(itr != m_animationHolder.end());

    auto anim_ptr = std::move(*itr);
    m_animationHolder.erase(itr);

    anim.Start(m_lastRenderTime + delayMSecs);

    // Sort the new Animation into the m_animationQueue
    auto anim_itr = std::find_if(m_animationQueue.begin(), m_animationQueue.end(),
        [&anim](std::unique_ptr<Animation>& testAnim) {
            return anim.getStartTime() <= testAnim->getStartTime(); });

    if (anim_itr == m_animationQueue.end())
        m_animationQueue.push_back(std::move(anim_ptr));
    else
        m_animationQueue.insert(anim_itr, std::move(anim_ptr));
}


// Called on every tick of the clock.
// typically every frame.
//
void Animator::Render(int64_t now)
{
    assert(now >= m_lastRenderTime);    // time should not slip backward
    m_lastRenderTime = now;

    if (m_animationQueue.size() == 0)
        return;

    // Pull the animations we are interested in out into
    // our own list, because animations can add more animations to 
    // the list and that will mess up the interator..
    std::vector<Animation*> liveAnimations;
    for (auto& anim_ptr : m_animationQueue)
    {
        if (anim_ptr->getStartTime() >= now)
            break;

        if(!anim_ptr->IsDone())
            liveAnimations.push_back(anim_ptr.get());
    }

    bool didFinishSomeone = false;
    for (Animation* anim : liveAnimations)
    {
        assert(anim->getStartTime() < now);
        assert(!anim->IsDone());

        anim->CallMotion(now);
        if (anim->IsDone())
        {
            anim->CallCompletion();
            didFinishSomeone = true;
        }
    }


    if (didFinishSomeone)
    {
        size_t before = m_animationQueue.size();

        // remove finished Animations
        m_animationQueue.erase(std::remove_if(
                m_animationQueue.begin(),
                m_animationQueue.end(),
                [](std::unique_ptr<Animation>& anim) { return anim->IsDone(); }),
            m_animationQueue.end());

        size_t after = m_animationQueue.size();
    }
}


int64_t Animator::NowMilliSec()
{
    return m_lastRenderTime;
}
