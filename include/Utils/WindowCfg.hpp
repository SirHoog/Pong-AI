#pragma once

#include <string_view>

#include <SDL3/SDL.h>

namespace PongAI::CONFIG::WINDOW
{
	constexpr std::string_view TITLE   = "Pong AI";
	constexpr std::string_view VERSION = "1.0";
	constexpr std::string_view APP_ID  = "PONG-AI-1.0";
	constexpr SDL_WindowFlags  FLAGS   = SDL_WINDOW_HIGH_PIXEL_DENSITY;

	constexpr int        WIDTH  = 1280;
	constexpr int        HEIGHT = 720;
	constexpr SDL_FPoint CENTER = { WIDTH / 2.0f, HEIGHT / 2.0f };

	constexpr SDL_Color BGC = { 0, 0, 0, 255 };
}