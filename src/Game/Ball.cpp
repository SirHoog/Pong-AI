#include "Game/Ball.hpp"

#include "Utils/PaddleCfg.hpp"
#include "Utils/RNG.hpp"

namespace PongAI
{
	using namespace BALL;
	
	void Ball::update(SDL_Window* window, double& gameSpeed, double& dt)
	{
		// Normalize the velocity
		const double magnitude = sqrt(pow(m_vel.x, 2) + pow(m_vel.y, 2));
		const double speed = SPEED * dt;

		if (magnitude > 0.0f)
		{
			m_vel.x *= static_cast<float>(speed / magnitude);
			m_vel.y *= static_cast<float>(speed / magnitude);
		}

		m_pos.x += m_vel.x * static_cast<float>(gameSpeed); // Multiply by gameSpeed here, so that setting gameSpeed to 0 doesn't call Ball::shoot()
		m_pos.y += m_vel.y * static_cast<float>(gameSpeed);

		const float bounceX = static_cast<float>(speed) * (m_vel.x < 0.0f ? -1.0f : 1.0f);

		// Bouncing on walls
		if (m_pos.y <= RADIUS)
			m_vel = { bounceX,  abs(m_vel.y) }; // Make x equal to speed so the ball doesn't bounce between the top and bottom forever
		else
		if (m_pos.y >= static_cast<float>(WINDOW::HEIGHT) - RADIUS)
			m_vel = { bounceX, -abs(m_vel.y) };
	}
	void Ball::draw(SDL_Renderer* renderer) const
	{
		const SDL_FRect rect{ m_pos.x - RADIUS, m_pos.y - RADIUS, DIAMETER, DIAMETER };

		SDL_RenderTexture(renderer, Texture, NULL, &rect);
	}

	const bool Ball::collided(const SDL_FRect& rect)
	{
		SDL_FPoint closest = m_pos;      // Closest corner of the rect to the ball

		if (  m_pos.x < rect.x)
			closest.x = rect.x;          // Left edge is closest
		else
		if (  m_pos.x > rect.x + rect.w)
			closest.x = rect.x + rect.w; // Right edge is closest

		if (  m_pos.y < rect.y)
			closest.y = rect.y;          // Top edge is closest
		else
		if (  m_pos.y > rect.y + rect.h)
			closest.y = rect.y + rect.h; // Bottom edge is closest

		const float distance = sqrt(pow((closest.x - m_pos.x), 2) + pow((closest.y - m_pos.y), 2));

		if (distance <= RADIUS)
		{
			// Bounce from center of paddle
			m_vel.x += m_pos.x - (rect.x + PADDLE::CENTER.x);
			m_vel.y += m_pos.y - (rect.y + PADDLE::CENTER.y);

			return true;
		}

		return false;
	}

	void Ball::shoot()
	{
		if (m_vel.x == 0.0f && m_vel.y == 0.0f)
		{
			const float angle = std::uniform_real_distribution<float>{ 0, 2 * SDL_PI_F }(RNG); // Random angle between BALL_DIR_MIN_ANGLE and BALL_DIR_MAX_ANGLE

			m_vel = {
				cos(angle) * 100.0f,
				sin(angle) * 100.0f
			};
		}
	}
}