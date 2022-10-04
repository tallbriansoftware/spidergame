#pragma once

#include <chrono>

class GameTime
{
public:
    GameTime();
    ~GameTime();
    int64_t Milliseconds();

private:
    std::chrono::high_resolution_clock::time_point m_gameStart;
};