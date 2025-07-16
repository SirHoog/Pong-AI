#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "application.hpp"

using namespace PongAI;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char *argv[])
{
    Application* app = new Application(); // The Cherno wouldn't be pleased by this, but I need it for the other SDL callbacks ;)

    if (!app->init())
    {
        delete app;
        return SDL_APP_FAILURE;
    }

    *appstate = app;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    Application* app = static_cast<Application*>(appstate);

    if (!app->running)
        return SDL_APP_SUCCESS;

    app->update();
    app->draw();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    Application* app = static_cast<Application*>(appstate);

    return app->handleEvent(*event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    delete static_cast<Application *>(appstate);
}