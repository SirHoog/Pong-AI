#pragma once

#include <cmath>
#include <random>

#include <SDL3/SDL.h>

#include "Utils/WindowCfg.hpp"
#include "Utils/BallCfg.hpp"

namespace PongAI
{
	using namespace CONFIG;

	struct Ball
	{
		inline static SDL_Texture* Texture = nullptr;

		Ball(const SDL_FPoint& pos = WINDOW::CENTER) : m_pos( pos ) {};

		// Move operations
		Ball           (Ball&&) = default;
		Ball& operator=(Ball&&) = default;

		// Delete copy operations
		Ball           (const Ball&) = delete;
		Ball& operator=(const Ball&) = delete;

		void update(SDL_Window* window, double& gameSpeed, double& dt);
		void draw(SDL_Renderer* renderer) const;

		const bool collided(const SDL_FRect& rect); // Simple circle-rectangle collision detection. Bounces the ball away from the center of the rect
		
		void reset(const SDL_FPoint& pos = WINDOW::CENTER) { m_pos = pos; m_vel = { 0.0f, 0.0f }; };
		void shoot(); // In random direction

		const SDL_FPoint getPosition() const { return m_pos; };
		const SDL_FPoint getVelocity() const { return m_vel; };
	private:
		SDL_FPoint m_pos; // THE CENTER OF THE BALL
		SDL_FPoint m_vel{ 0.0f, 0.0f };
	};
}