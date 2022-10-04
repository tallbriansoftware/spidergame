#include "Utils/GameTime.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;

GameTime::GameTime()
{
    m_gameStart = std::chrono::high_resolution_clock::now();
}

GameTime::~GameTime()
{
}

int64_t GameTime::Milliseconds()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto gameMilliseconds = duration_cast<milliseconds>(now - m_gameStart);
    return gameMilliseconds.count();
}
