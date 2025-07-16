#pragma once

#include "Paddle.hpp"
#include "Ball.hpp"

namespace PongAI
{
	struct Game
	{
		Paddle paddleL;
		Paddle paddleR;
		Ball ball;

		bool autoStart = true;
		bool paused    = false; // Disables update()
		bool visible   = true;  // Enables draw()

		Game(Paddle&& paddleLeft   = Paddle(Left,  std::make_unique<Bot>()),
			 Paddle&& paddleRight  = Paddle(Right, std::make_unique<Bot>()),
			 Ball  && ball_        = Ball(),

			 const bool autoStart_ = true,
			 const bool visible_   = true,
			 const bool paused_    = false)
			: paddleL(std::move(paddleLeft)), paddleR(std::move(paddleRight)), ball(std::move(ball_)), autoStart(autoStart_), visible(visible_), paused(paused_) {};

		// Move operations
		Game           (Game&&) noexcept = default;
		Game& operator=(Game&&) noexcept = default;

		// Delete copy operations
		Game           (const Game&) = delete;
		Game& operator=(const Game&) = delete;

		void update(SDL_Window* window, double& gameSpeed, double& dt);
		void draw(SDL_Renderer* renderer);

		inline void pause()   { paused = true;  };
		inline void unpause() { paused = false; };

		inline void hide() { visible = false; };
		inline void show() { visible = true;  };

		inline void selectLeft()  { Paddle::Player = &paddleL; };
		inline void selectRight() { Paddle::Player = &paddleR; };
	private:
		void handleScoring();
		bool updatePaddle(Paddle& paddle, Paddle& other, SDL_Window*& window, double& gameSpeed, double& dt);
	};
}