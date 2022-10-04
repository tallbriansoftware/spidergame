#include "App/AppCards.h"

#include "MyGL/GLErrors.h"

#include <memory>
#include <string>


int main()
{
    // Create things;
    auto app = std::make_unique<AppCards>("OpenGL-Cards", 1600, 1200);
    if (app == nullptr)
    {
        LOG_ERROR("Failed to create Application class");
        return 1;
    }

    app->Init();
    while (app->ShouldContinue())
    {
        app->Clear();

        app->Render();

        app->EndFrame();
    }
    app->Shutdown();
}

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    main();
}