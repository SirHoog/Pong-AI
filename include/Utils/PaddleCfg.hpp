#pragma	once

#include "WindowCfg.hpp"

namespace PongAI::CONFIG::PADDLE
{
	constexpr SDL_Point TEXTURE_SIZE  = { 120, 750 };
	constexpr float     TEXTURE_SCALE = 0.25f;

	constexpr SDL_FPoint SIZE   = { TEXTURE_SIZE.x * TEXTURE_SCALE, TEXTURE_SIZE.y * TEXTURE_SCALE };
	constexpr SDL_FPoint CENTER = { SIZE.x / 2.0f, SIZE.y / 2.0f }; // Watch out for ambiguity between PADDLE::CENTER and WINDOW::CENTER

	constexpr float MARGIN  = 30.0f;
	constexpr float LEFT_X  = MARGIN;
	const     float RIGHT_X = WINDOW::WIDTH - MARGIN - SIZE.x;

	constexpr size_t MAX_SCORE = 10;
	constexpr double SPEED     = 200.0;

	inline bool USE_ARROW_KEYS = true;
}