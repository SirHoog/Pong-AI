#pragma once

#include <limits>
#include <cstring>

#include <SDL3_image/SDL_image.h>

#include "UI/UI.hpp"

namespace PongAI
{
	struct Application
	{
		bool running = true;

		Application();
		~Application();

		bool init();
		SDL_AppResult handleEvent(SDL_Event& event);
		void update();
		void draw();
	private:
		SDL_Window*   m_window   = nullptr;
		SDL_Renderer* m_renderer = nullptr;

		// Treat all UI like an extension of Application
		// (An excuse to say that friending these classes doesn't actually break encapsulation)
		// (But I also don't want to clutter my Application class)
		friend struct MainWindow;
		friend struct GameWindow;
		friend struct PlotWindow;
		friend struct UI;
		UI m_UI;

		uint64_t m_now  = 0;   // Milliseconds
		uint64_t m_last = 0;   // Milliseconds
		double   m_dt   = 1.0; // Seconds
		double   m_gameSpeed = 1.0; // Game Speed Multiplier
		double   m_maxGameSpeed = 10.0;

		SDL_Texture* loadTexture(const char* path);

		// AI Training

		uint16_t m_gensN = 0;
		bool m_trackTopBot = false; // Constantly shows the Game with the top Bot
		bool m_training = true; // Turn off if you want to test the Bots or manage them manually

		/* How long until episode/generation ends
		 * Respective of m_gameSpeed
		 */
		double m_episodeTimer = TRAINING::EpisodeLength * m_gameSpeed; // Seconds

		/* Manages the next generation of Bots
		 * Unpauses the Games and resets Paddle positions
		 * Selects next Bot generation, ticking up m_generationN
		 * Elitist: Selects the top Bots from *all* games
		 * Applies Bot::crossoverReplace() between the elite Bots on the lower fitness Bots (to avoid unnecessary allocation)
		 * Mutates the replaced Bots
		 */
		void nextGeneration();
	};
}