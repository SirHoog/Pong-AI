#include "Game/Paddle.hpp"

#include "Utils/PaddleCfg.hpp"
#include "Utils/ResourcesCfg.hpp"

namespace PongAI
{
	using namespace PADDLE;

	void Paddle::updateAsPlayer()
	{
		const bool* keys = SDL_GetKeyboardState(NULL);

		if (USE_ARROW_KEYS)
			if (keys[SDL_SCANCODE_UP])
				m_direction = Up;
			else
			if (keys[SDL_SCANCODE_DOWN])
				m_direction = Down;
		else
			if (keys[SDL_SCANCODE_W])
				m_direction = Up;
			else
			if (keys[SDL_SCANCODE_S])
				m_direction = Down;
	}

	void Paddle::update(SDL_Window* window, double gameSpeed, double dt)
	{
		m_rect.y += static_cast<float>(SPEED * gameSpeed * dt) * m_direction;

		if (m_rect.y < 0.0f)
			m_rect.y = 0.0f;
		if (m_rect.y > static_cast<float>(WINDOW::HEIGHT) - m_rect.h)
			m_rect.y = static_cast<float>(WINDOW::HEIGHT) - m_rect.h;
	}
	void Paddle::draw(SDL_Renderer* renderer) const
	{
		using namespace RESOURCES::TEXTURES;

		if (Player == this)
			SDL_RenderTexture(renderer, PlrTexture, NULL, &m_rect);
		else
			SDL_RenderTexture(renderer, BotTexture, NULL, &m_rect);
	}
}