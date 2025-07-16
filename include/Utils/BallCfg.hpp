#pragma once

namespace PongAI::CONFIG::BALL
{
	constexpr int    TEXTURE_DIAMETER = 120;
	constexpr float  TEXTURE_SCALE    = 0.25f;

	constexpr float  DIAMETER = TEXTURE_DIAMETER * TEXTURE_SCALE;
	constexpr float  RADIUS   = DIAMETER / 2.0f;
	constexpr double SPEED    = 400.0;
}