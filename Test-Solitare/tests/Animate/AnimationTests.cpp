#include "Helpers/GoogleTest.h"

#include "Animate/Animation.h"
#include "Animate/Animator.h"

#include <iostream>

TEST(AnimationTest, TestMotion)
{
    Animator animator;
    bool didFirst = false;
    bool didLast = false;

    Animation::MotionCB motion = [&didFirst, &didLast](float value)
    {
        if (value == 0.0f)
            didFirst = true;
        if (value == 1.0f)
            didLast = true;
    };

    Animation::CompletionCB done = []() {};

    animator.Render(900);

    Animation* anim = animator.CreateAnimation(0.0f, 1.0f, 1000, motion, done);
    animator.StartAnimation(*anim);

    for (int i = 1000; i < 4000; i += 100)
    {
        animator.Render(i);
    }

    EXPECT_TRUE(didFirst);
    EXPECT_TRUE(didLast);
}

TEST(AnimationTest, TestCompletion)
{
    Animator animator;
    bool didDone = false;

    Animation::MotionCB motion = [](float value) {};

    Animation::CompletionCB done = [&didDone]()
    {
        didDone = true;
    };

    animator.Render(900);

    Animation* anim = animator.CreateAnimation(0.0f, 1.0f, 1000, motion, done);
    animator.StartAnimation(*anim);

    for (int i = 1000; i < 4000; i += 100)
    {
        animator.Render(i);
    }

    EXPECT_TRUE(didDone);
}


TEST(AnimationTest, TestRange)
{
    Animator animator;
    animator.Render(100);

    float lowest = 1000.0f;
    float greatest = -1000.0f;

    Animation::MotionCB motion = [&lowest, &greatest](float value) {
        if (value < lowest)
            lowest = value;
        if (value > greatest)
            greatest = value;
    };
    Animation::CompletionCB done = []() {};

    Animation* anim = animator.CreateAnimation(5.0f, 10.0f, 1000, motion, done);
    animator.StartAnimation(*anim);

    for (int i = 200; i < 2000; i += 100)
        animator.Render(i);

    EXPECT_EQ(lowest, 5.0f);
    EXPECT_EQ(greatest, 10.0f);
}


TEST(AnimationTest, TestTiming)
{
    Animator animator;

    int64_t start_A = -1;
    int64_t start_B = -1;
    int64_t start_C = -1;

    Animation::MotionCB motion_A = [&start_A, &animator](float value) {
        if (start_A == -1)
        {
            start_A = animator.NowMilliSec();
        }
    };

    Animation::MotionCB motion_B = [&start_B, &animator](float value) {
        if (start_B == -1)
        {
            start_B = animator.NowMilliSec();
        }
    };

    Animation::MotionCB motion_C = [&start_C, &animator](float value) {
        if (start_C == -1)
        {
            start_C = animator.NowMilliSec();
        }
    };

    Animation::CompletionCB done = []() {};

    Animation* anim_A = animator.CreateAnimation(5.0f, 10.0f, 200, motion_A, done);
    Animation* anim_B = animator.CreateAnimation(5.0f, 10.0f, 200, motion_B, done);
    Animation* anim_C = animator.CreateAnimation(5.0f, 10.0f, 200, motion_C, done);
    animator.StartAnimation(*anim_A, 250);
    animator.StartAnimation(*anim_B, 501);
    animator.StartAnimation(*anim_C, 799);

    animator.Render(1);

    for (int i = 100; i < 2000; i += 100)
    {
        animator.Render(i);
    }

    EXPECT_EQ(start_A, 300);
    EXPECT_EQ(start_B, 600);
    EXPECT_EQ(start_C, 800);
}


TEST(AnimationTest, OverlappingTest)
{
    Animator animator;

    Animation::MotionCB motion = [&animator](float value) {
    };

    Animation::CompletionCB done = []() {};

    int64_t delay = 0;
    for (int i = 0; i < 100; i++)
    {
        Animation* anim = animator.CreateAnimation(2.0f, 22.0f, 1, motion, done);
        animator.StartAnimation(*anim, delay);
        delay += 50;
    }

    for (int i = 0; i < 5000; i += 16)
    {
        animator.Render(i);
    }


}
