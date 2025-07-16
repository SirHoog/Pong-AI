#pragma once

#include <random>

namespace PongAI::CONFIG
{
	inline std::mt19937 RNG(std::random_device{}());
}