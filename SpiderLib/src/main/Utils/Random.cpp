#include "Utils/Random.h"

#include <assert.h>


Random::Random()
    : m_r()
{
    std::random_device r;
    m_r.seed(r());
}

Random::Random(int seed)
    : m_r(seed)
{
}

int Random::Next()
{
    // return only positive integers (and 0);
    // can't decide which is best:
    // mod (%) with INT_MASK.  But what really are the rules with mod and negitive numbers?
    // Mask (&) with INT_MASK but then we are assuming that INT_MAX == 0x7FFFFFFF;
    // Mask with 0x7FFFFFF directly.
    return (m_r() - m_r.min()) & INT_MAX;
}


// Coin flip.
bool Random::Toss()
{
    return (Next() & 0x01) == 0;
}


// Bool test if something is less than the "rate"
// ie.  if(FanTest(0.25))  // less than 25%
bool Random::FanTest(double rate)
{
    return (Range(0.0, 1.0) < rate);
}


// Return an Int in the range [start - end)
// including "start" but not including end.
int Random::Range(int start, int end)
{
    assert(start < end);
    int range = end - start;
    int next = this->Next();
    int base = next % range;
    return base + start;
}


// Return a Double in the range [start - end)
// including "start" but not including end.
double Random::Range(double start, double end)
{
    constexpr int million = 1000000;

    assert(start < end);
    double range = end - start;
    int rnum = this->Next() % million;
    double base = ((rnum * range) / million);
    return base + start;
}
