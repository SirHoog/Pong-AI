#include "UI/gameWindow.hpp"

namespace PongAI
{
	using namespace ImGui;

	using namespace PLOT_WINDOW;

	GameWindow::GameWindow(SDL_Renderer* renderer, size_t index)
		: Window("Game " + std::to_string(index), GAME_WINDOW::FLAGS), m_renderer(renderer),
		m_fitPlot{ title + " - Fitness Plot", "Seconds", "Fitness",
			{
				SB(LIVE_N, "Left Bot" , LEFT_COL ),
				SB(LIVE_N, "Right Bot", RIGHT_COL)
			}}
	{
		using namespace WINDOW;

		m_gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
		if (!m_gameTexture)
			SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());

		SDL_SetTextureScaleMode(m_gameTexture, SDL_SCALEMODE_LINEAR);

		m_fitPlot.close();

		constexpr float w = static_cast<float>(WIDTH );
		constexpr float h = static_cast<float>(HEIGHT);

		m_minSize = { w / 5.0f, h / 5.0f };
		m_size    = GAME_WINDOW::DEFAULT_SIZE;
		m_maxSize = { w       , h        };

		m_aspectRatio = w / h;
	}

	void GameWindow::draw()
	{
		if (!m_open) return;

		Window::draw();
		if (m_active)
		{
			MenuBar();

			if (!SDL_SetRenderTarget(m_renderer, m_gameTexture))
				SDL_Log("SDL_SetRenderTarget failed: %s", SDL_GetError());

			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
			SDL_RenderClear(m_renderer); // Clear the texture

			game.draw(m_renderer);

			SDL_SetRenderTarget(m_renderer, nullptr); // Reset

			// Maintain aspect ratio
			const ImVec2 avail = GetContentRegionAvail();
			ImVec2 imageSize = avail;
			if (imageSize.x > imageSize.y * m_aspectRatio)
				imageSize.x = imageSize.y * m_aspectRatio;
			else
				imageSize.y = imageSize.x / m_aspectRatio;

			// Center image
			ImVec2 pos = GetCursorScreenPos();
			pos.x += (avail.x - imageSize.x) * 0.5f;
			pos.y += (avail.y - imageSize.y) * 0.5f;
			SetCursorScreenPos(pos);

			Image((ImTextureID)(uintptr_t)m_gameTexture, imageSize);
		}
		End();

		if (auto& bL = game.paddleL.bot) m_fitPlot[0].addPoint(GetTime(), bL->getFitness() * PLOT_WINDOW::SCALE);
		if (auto& bR = game.paddleR.bot) m_fitPlot[1].addPoint(GetTime(), bR->getFitness() * PLOT_WINDOW::SCALE);

		m_fitPlot.draw();
	}
	void GameWindow::MenuBar()
	{
		if (BeginMenuBar())
		{
			if (BeginMenu("Play As"))
			{
				if (Button("Left Paddle"))  game.selectLeft();
				if (Button("Right Paddle")) game.selectRight();
				if (Button("None"))         Paddle::Player = nullptr; // Deselect player Paddle
				
				EndMenu();
			}
			if (BeginMenu("View"))
			{
				if (Button("Bot Fitness")) m_fitPlot.open();
				// Will add more button and options in the future
				EndMenu();
			}

			const bool autoStart = game.autoStart;
			if (autoStart) BeginDisabled();

			if (Button("Start"))
				game.unpause();

			if (autoStart) EndDisabled();

			EndMenuBar();
		}
	}
}