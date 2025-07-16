#pragma once

#include <string_view>

namespace PongAI::CONFIG::RESOURCES
{
	namespace FONTS
	{
		constexpr std::string_view Iceberg_PATH = RES_DIR "/fonts/Iceberg/Iceberg-Regular.ttf";
	}

	namespace TEXTURES
	{
		constexpr std::string_view PADDLE_BOT_PATH = TEX_DIR "/PaddleBot.png";
		constexpr std::string_view PADDLE_PLR_PATH = TEX_DIR "/PaddlePlayer.png";
		constexpr std::string_view BALL_PATH = TEX_DIR "/Ball.png";
	}
}