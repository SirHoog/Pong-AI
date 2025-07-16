#pragma once

#include "PlotWindow.hpp"
#include "Game/Game.hpp"

namespace PongAI
{
	struct GameWindow : Window
	{
		Game game;

		GameWindow(SDL_Renderer* renderer, size_t index);
		~GameWindow() { if (m_gameTexture) SDL_DestroyTexture(m_gameTexture); };

		void draw() override;
	private:
		SDL_Renderer* m_renderer    = nullptr;
		SDL_Texture*  m_gameTexture = nullptr;
		PlotWindow m_fitPlot;
		float m_aspectRatio;

		void MenuBar();
	};
}