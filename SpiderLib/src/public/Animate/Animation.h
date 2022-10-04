#pragma once

#include <functional>

#include <string>

class Animator;


class Animation
{
public:
    // MotionCB: called every frame that the animation is running
    using MotionCB = std::function<void(float value)>;

    // CompletionCB: Called when the animation is done.
    using CompletionCB = std::function<void()>;

    // Interpolator: Takes a value from 0.0 - 1.0 and returns a value from 0.0 - 1.0
    using Interpolator = std::function<float(float)>;

public:
    Animation(Animator& animator,
        float from, float to,
        int64_t durationMSec,
        MotionCB motion, CompletionCB done);
    ~Animation();
        

public:
    void Start(int64_t now);
    int64_t getStartTime();
    bool IsDone();
    void CallMotion(int64_t progress);
    void CallCompletion();

    std::string ToString();


private:
    Animator& m_animator;
    float m_startValue;
    float m_endValue;
    int64_t m_startTimeMSecs;
    int64_t m_durationMSec;
    int64_t m_progress;
    bool m_isDone;

    MotionCB m_motion;
    CompletionCB m_completion;
};
