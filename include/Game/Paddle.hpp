#pragma once

#include "AI/Bot.hpp"

#include "Utils/PaddleCfg.hpp"

namespace PongAI
{
	enum Side
	{
		Left = -1,
		Right = 1
	};

	struct Paddle
	{
		std::unique_ptr<Bot> bot = nullptr;
		uint8_t score = 0;

		inline static SDL_Texture* BotTexture = nullptr;
		inline static SDL_Texture* PlrTexture = nullptr;

		inline static Paddle*      Player     = nullptr; // Pointer to the *one* Paddle which the player is currently controlling
		
		// @param side: 0 = Left, 1 = Right
		Paddle(Side side, std::unique_ptr<Bot> bot_ = nullptr) :
			m_side(side), m_rect({ 0.0f, WINDOW::CENTER.y - PADDLE::CENTER.y, PADDLE::SIZE.x, PADDLE::SIZE.y }),
			bot(std::move(bot_))
		{ if (side == Left) m_rect.x = PADDLE::LEFT_X; else m_rect.x = PADDLE::RIGHT_X; };

		// Move operations
		Paddle           (Paddle&&) noexcept = default;
		Paddle& operator=(Paddle&&) noexcept = default;

		// Delete copy operations
		Paddle           (const Paddle&) = delete;
		Paddle& operator=(const Paddle&) = delete;

		void updateAsPlayer();
		void updateAsBot(const std::vector<float>& inputs) { if (bot) m_direction = bot->update(inputs); };

		void update(SDL_Window* window, double gameSpeed, double dt);
		void draw(SDL_Renderer* renderer) const;

		void reset(float y = WINDOW::CENTER.y) { m_rect.y = y; score = 0; };

		const Direction getDirection() const { return m_direction; };
		const SDL_FRect getRect()      const { return m_rect;      }; // Get wrecked XD
		const Side      getSide()      const { return m_side;      };
	private:
		Direction m_direction = Up;
		SDL_FRect m_rect;
		Side      m_side;
	};

}