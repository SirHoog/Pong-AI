#include <../imgui_internal.h>

#include "UI/UI.hpp"
#include "Application.hpp" // For the actual definition of the Application class

namespace PongAI
{
    using namespace ImGui; // Be careful
    
    UI::UI(Application* app_) : app(app_), mainWindow{app_}
    {
        if (!app)
            throw std::runtime_error("Expected pointer to existing Application.");
    }
    UI::~UI()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();

        ImGui ::DestroyContext(m_context);
        ImPlot::DestroyContext(m_plotContext);
    }

    void UI::init()
    {
        IMGUI_CHECKVERSION();

        m_context     = ImGui ::CreateContext();
        m_plotContext = ImPlot::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard | // Enable Keyboard Controls
            ImGuiConfigFlags_DockingEnable     | // Enable Docking
            ImGuiConfigFlags_ViewportsEnable     // Enable Viewports
        ;

        StyleColorsDark();

        m_IcebergFont = io.Fonts->AddFontFromFileTTF(CONFIG::RESOURCES::FONTS::Iceberg_PATH.data(), 22.0f);

        ImGui_ImplSDL3_InitForSDLRenderer(app->m_window, app->m_renderer);
        ImGui_ImplSDLRenderer3_Init(app->m_renderer);
    }
    void UI::draw()
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        NewFrame();

        PushFont(m_IcebergFont);

        DockSpaceOverViewport(GetID("PongAI - Dock Space"), GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        mainWindow.draw();

        bool gameWindowClosed = false;

		for (size_t i = 0; i < gameWindows.size(); ++i)
        {
            auto& gw = gameWindows[i];

			if (gameWindowClosed) gw->title = "Game " + std::to_string(i + 1); // Match title to Game index

            gw->draw();
            
            if (!gw->isOpen()) // Delete closed GameWindows
            {
                gameWindows.erase(gameWindows.begin() + i);
                gameWindowClosed = true;
                --i;
            }
        }

        PopFont();

        EndFrame();
        Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(GetDrawData(), app->m_renderer);
    }

    void UI::addGameWindow()
    {
        using namespace GAME_WINDOW;
        static bool dockSetup = false;
        static ImGuiID rootID;

        if (!dockSetup)
        {
            dockSetup = true;
            rootID = DockBuilderAddNode(GetID("root"), ImGuiDockNodeFlags_None); // Tab new GameWindows together

            ImGuiViewport* viewport = GetMainViewport();
            ImVec2 center = ImVec2(
                viewport->Pos.x + viewport->Size.x * 0.5f - DEFAULT_SIZE.x / 2.0f,
                viewport->Pos.y + viewport->Size.y * 0.5f - DEFAULT_SIZE.y / 2.0f
            );
            DockBuilderSetNodePos(rootID, center);
            DockBuilderSetNodeSize(rootID, DEFAULT_SIZE);
            DockBuilderFinish(rootID);
        }

        gameWindows.emplace_back(std::make_unique<GameWindow>(app->m_renderer, gameWindows.size() + 1));
        DockBuilderDockWindow(gameWindows.back()->title.c_str(), rootID);
    }
    void UI::addGameWindows(size_t n)
    {
        gameWindows.reserve(gameWindows.size() + n);

        for (size_t i = 0; i < n; ++i)
            addGameWindow();
    }
}