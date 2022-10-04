#include "App/GameResources.h"

#include <iostream>

#include <windows.h>


namespace
{
    int local_symbol = 42;


    std::string GetBinPath()
    {
        char path[MAX_PATH];
        HMODULE hm = NULL;

        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&local_symbol, &hm) == 0)
        {
            int ret = GetLastError();
            fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
            return "";
        }
        if (GetModuleFileNameA(hm, path, sizeof(path)) == 0)
        {
            int ret = GetLastError();
            fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
            return "";
        }
        return path;
    }
}


GameResources::GameResources()
{
    std::string binPath = GetBinPath();
    if (binPath.length() == 0)
    {
        std::cout << "GetBinPath() failed" << std::endl;
        return;
    }

    size_t index = binPath.rfind('\\');
    if (index == std::string::npos)
    {
        std::cout << "Could not find '\\' character in bin path" << std::endl;
        return;
    }

     m_binDir = binPath.substr(0, index);
}


std::string GameResources::GetShaderPath()
{
    static std::string shaderSubpath("\\res\\shaders");
    return m_binDir + shaderSubpath;
}

std::string GameResources::GetCardPath()
{
    static std::string cardsSubpath("\\res\\cards");
    return m_binDir + cardsSubpath;
}

std::string GameResources::GetButtonPath()
{
    static std::string buttonSubpath("\\res\\buttons");
    return m_binDir + buttonSubpath;
}

