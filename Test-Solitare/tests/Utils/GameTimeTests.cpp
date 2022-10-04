#include "Helpers/GoogleTest.h"

#include "Utils/GameTime.h"

TEST(GameTimeTest, Init0)
{
    GameTime gtime;

    auto t1 = gtime.Milliseconds();
    auto t2 = gtime.Milliseconds();

    EXPECT_EQ(t1, t2);
}