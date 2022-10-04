#pragma once

// Access to resources:  Shaders, bitmaps, etc.
// Also access to save file locations.

#include <string>

class GameResources
{
public:
    GameResources();

    std::string GetShaderPath();
    std::string GetCardPath();
    std::string GetButtonPath();
    
private:
    std::string m_binDir;
};