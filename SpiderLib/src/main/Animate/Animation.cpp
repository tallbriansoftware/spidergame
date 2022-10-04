#include "Animate/Animation.h"

#include "Animate/Animator.h"

#include <assert.h>
#include <iostream>


namespace
{
    float LinearInterpolate(float frac, float start, float end)
    {
        float extent = end - start;
        float interpolate = (extent * frac) + start;
        return interpolate;
    }
}


Animation::Animation(Animator& animator,
    float from, float to,
    int64_t durationMSec,
    MotionCB motion, CompletionCB done)
    : m_animator(animator)
    , m_startValue(from)
    , m_endValue(to)
    , m_startTimeMSecs(LLONG_MAX)
    , m_durationMSec(durationMSec)
    , m_progress(-1)
    , m_isDone(false)
    , m_motion(motion)
    , m_completion(done)
{
}


Animation::~Animation()
{
}


void Animation::Start(int64_t now)
{
    m_startTimeMSecs = now;
}


int64_t Animation::getStartTime()
{
    return m_startTimeMSecs;
}


bool Animation::IsDone()
{
    if (m_progress == -1)
        return false;

    if (m_progress >= m_durationMSec)
        return true;

    return false;
}


void Animation::CallMotion(int64_t now)
{
    if (now < m_startTimeMSecs)
        return;

    assert(m_startTimeMSecs != LONG_MAX);

    m_progress = now - m_startTimeMSecs;

    if (m_progress >= m_durationMSec)
    {
        m_motion(m_endValue);
        m_isDone = true;
        return;
    }

    float fraction = (float)(m_progress / (float)m_durationMSec);
    float interpolate = LinearInterpolate(fraction, m_startValue, m_endValue);

    m_motion(interpolate);
}


void Animation::CallCompletion()
{
    m_completion();
}

std::string Animation::ToString()
{
    std::string s;
    s += "startTime:" + std::to_string(m_startTimeMSecs);
    s += " duration:" + std::to_string(m_durationMSec);
    s += " progress:" + std::to_string(m_progress);
    s += " done:" + std::to_string(m_isDone);
    s += " values(" + std::to_string(m_startValue) + ", " + std::to_string(m_endValue) + ")";
    return s;
}

