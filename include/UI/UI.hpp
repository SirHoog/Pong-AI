#pragma once

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include "MainWindow.hpp"

#include "Utils/ResourcesCfg.hpp"

namespace PongAI
{
	struct UI
	{
		Application* app;
		MainWindow mainWindow;
		std::vector<std::unique_ptr<GameWindow>> gameWindows;

		UI() = delete;
		UI(Application* app);
		~UI();

		void init();
		void draw();

		void addGameWindow();
		void addGameWindows(size_t n);
	private:
		ImGuiContext*  m_context     = nullptr;
		ImPlotContext* m_plotContext = nullptr;

		ImFont* m_IcebergFont = nullptr;
	};
}