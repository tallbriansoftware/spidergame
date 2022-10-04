#include "Helpers/GoogleTest.h"

#include "Utils/Random.h"


TEST(RandomTest, Init0)
{
    Random random(42);

    int a = random.Next();
    int b = random.Next();

    Random random2(42);

    int c = random2.Next();
    int d = random2.Next();

    EXPECT_EQ(a, c);
    EXPECT_EQ(b, d);

    EXPECT_NE(a, b);
}


TEST(RandomTest, Init1)
{
    // Initing with no seen implicly randomizes the seed
    Random random;

    int a = random.Next();
    int b = random.Next();

    // Initing with no seen implicly randomizes the seed
    Random random2;

    int c = random2.Next();
    int d = random2.Next();

    EXPECT_NE(a, c);
    EXPECT_NE(b, d);

    EXPECT_NE(a, b);
}


TEST(RandomTest, RangeInt)
{
    Random random;

    int min = -3;
    int max = 47;

    for (int i = 0; i < 100; i++)
    {
        int value = random.Range(min, max);
        EXPECT_LE(min, value);
        EXPECT_GT(max, value);
    }
}


TEST(RandomTest, RangeDouble)
{
    Random random;

    double min = -3.3;
    double max = 47.3;

    for (int i = 0; i < 100; i++)
    {
        double value = random.Range(min, max);
        EXPECT_LE(min, value);
        EXPECT_GT(max, value);
    }
}
