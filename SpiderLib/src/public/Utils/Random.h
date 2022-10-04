#pragma once

#include <random>

class Random
{
public:
    // If no seed then seed it randomly
    Random();
    Random(int seed);

    int Next();

    bool Toss();

    bool FanTest(double rate);

    int Range(int start, int end);
    double Range(double start, double end);

private:
    std::default_random_engine m_r;
};
