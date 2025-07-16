#include "Application.hpp"

namespace PongAI
{
    Application::Application()
        : m_UI{this}
    {
        using namespace WINDOW;
        SDL_SetAppMetadata(TITLE.data(), VERSION.data(), APP_ID.data());
    }
    Application::~Application()
    {
        SDL_DestroyTexture(Paddle::BotTexture);
        SDL_DestroyTexture(Paddle::PlrTexture);
        SDL_DestroyTexture(Ball::Texture);

        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    bool Application::init()
    {
        using namespace WINDOW;
        
        // Setup SDL
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return false;
        }

        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");

        if (!SDL_CreateWindowAndRenderer(TITLE.data(), WIDTH, HEIGHT, FLAGS, &m_window, &m_renderer))
        {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return false;
        }

        m_last = SDL_GetPerformanceCounter();

        // Setup m_games

        // Load textures
        using namespace RESOURCES::TEXTURES;

        Paddle::BotTexture = loadTexture(PADDLE_BOT_PATH.data());
        Paddle::PlrTexture = loadTexture(PADDLE_PLR_PATH.data());
        Ball::Texture      = loadTexture(BALL_PATH      .data());

        Paddle::Player = nullptr; // No need to set the Bot of the player Paddle to nullptr, because Paddle::updateAsPlayer() is called anyway so the Bot just remains unused

        m_UI.init();

        return true;
    }

    void Application::update()
    {
        m_now = SDL_GetPerformanceCounter();
        m_dt = static_cast<double>(m_now - m_last) / static_cast<double>(SDL_GetPerformanceFrequency());
        m_last = m_now;

        if (!m_UI.mainWindow.initialized) return;
        if (m_UI.gameWindows.empty()) return; // Wait for user to add Game

        bool allGamesPaused = true;

        for (auto& gameWindow : m_UI.gameWindows)
        {
            Game& game = gameWindow->game;

            if (!game.paused)
                allGamesPaused = false;

            game.update(m_window, m_gameSpeed, m_dt);
        }

        if (m_training)
        {
            using namespace TRAINING;

            m_episodeTimer -= m_dt * m_gameSpeed;

            if (m_episodeTimer > EpisodeLength)
                m_episodeTimer = EpisodeLength;

            if (m_episodeTimer <= 0.0 || allGamesPaused)
            {
                m_episodeTimer = EpisodeLength;
                nextGeneration();
            }
        }
    }

    void Application::draw()
    {
        using WINDOW::BGC;
		SDL_SetRenderTarget(m_renderer, nullptr);
        SDL_SetRenderDrawColor(m_renderer, BGC.r, BGC.g, BGC.b, BGC.a);
        SDL_RenderClear(m_renderer);

        m_UI.draw();

        SDL_RenderPresent(m_renderer);
    }

    SDL_AppResult Application::handleEvent(SDL_Event& event)
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
            case (SDL_EVENT_QUIT):
                return SDL_APP_SUCCESS;
            case (SDL_EVENT_WINDOW_CLOSE_REQUESTED):
                if (event.window.windowID == SDL_GetWindowID(m_window))
                    return SDL_APP_SUCCESS;
                break;
            case (SDL_EVENT_KEY_DOWN):
                switch (event.key.scancode)
                {
                    case (SDL_SCANCODE_ESCAPE):
                        return SDL_APP_SUCCESS;
                }
        }

        return SDL_APP_CONTINUE;
    }

    void Application::nextGeneration()
    {
        using namespace TRAINING;

        ++m_gensN;

        m_UI.mainWindow.updateGenFitPlot();

        std::vector<Bot*> botsL;
        std::vector<Bot*> botsR;

        // Reserve space for all paddles on one side, even if they don't have Bots
        botsL.reserve(m_UI.gameWindows.size());
        botsR.reserve(m_UI.gameWindows.size());

        // Get each Bot and reset Games
        for (auto& gameWindow : m_UI.gameWindows)
        {
            Game& game = gameWindow->game;

            Paddle& pL = game.paddleL;
            Paddle& pR = game.paddleR;

            Bot* bL = pL.bot.get();
            Bot* bR = pR.bot.get();

            pL.reset();
            pR.reset();
            game.unpause();

            if (bL) botsL.push_back(bL);
            if (bR) botsR.push_back(bR);
        }

        // Sort Bots from best to worst
        auto descendFit = [](Bot*& a, Bot*& b) { return a->getFitness() > b->getFitness(); };

        std::sort(botsL.begin(), botsL.end(), descendFit);
        std::sort(botsR.begin(), botsR.end(), descendFit);

        const size_t botsNL = botsL.size();
        const size_t botsNR = botsR.size();

        if (botsNL < 2 || botsNR < 2)
            throw std::runtime_error("Not enough Bots for crossover");

        // Elitism Selection
        if (EliteCount == 1)
        {
            // Copy the neural network of the top Bot on both sides
            for (Bot*& bL : botsL)
                bL->newBrain(std::make_unique<Network>(*botsL[0]->getBrain()));
            for (Bot*& bR : botsR)
                bR->newBrain(std::make_unique<Network>(*botsR[0]->getBrain()));

            return;
        }
        
        const size_t elitesNL = std::min(EliteCount, botsNL);
        const size_t elitesNR = std::min(EliteCount, botsNR);
        
        auto nextGen =
            [](auto& bots, auto& elitesN)
            {
                // Crossover the Elite Bots and replace the non-elite Bots
                for (size_t i = elitesN; i < bots.size(); ++i)
                {
                    Bot* child   = bots[i];
                    Bot* parentA = bots[i       % elitesN]; // Pair Elite Bots together systematically
                    Bot* parentB = bots[(i + 1) % elitesN];

                    child->crossoverReplace(parentA, parentB);
                    child->mutate();
                    child->resetFitness(); // Reset for next generation
                }

                for (size_t i = 0; i < elitesN; ++i) // Reset Elite Bots' fitness, also
                    bots[i]->resetFitness();
            };

        nextGen(botsL, elitesNL);
        nextGen(botsR, elitesNR);
    }

    SDL_Texture* Application::loadTexture(const char* path)
    {
        SDL_Log("Loading file `%s`...", path);

        SDL_Texture* texture = IMG_LoadTexture(m_renderer, path);

        if (!texture)
            SDL_Log("The image file `%s` could not be loaded.\nError: %s", path, SDL_GetError());
        else
            SDL_Log("Successfully loaded file `%s`...", path);

        return texture;
    }
}