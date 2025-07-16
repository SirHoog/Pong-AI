#pragma once

#include <imgui.h>

#include "WindowCfg.hpp"

namespace PongAI::CONFIG::UI_CFG
{
	// Constants for ImGui sliders
	constexpr float    zF   = 0.0f;
	constexpr float    zD   = 0.0;
	constexpr uint8_t  zU8  = 0;
	constexpr uint16_t zU16 = 0;
	constexpr uint32_t zU32 = 0;
	constexpr uint64_t zU64 = 0;

	constexpr float    oF   = 1.0f;
	constexpr float    oD   = 1.0;
	constexpr uint8_t  oU8  = 1;
	constexpr uint16_t oU16 = 1;
	constexpr uint32_t oU32 = 1;
	constexpr uint64_t oU64 = 1;

	namespace MAIN_WINDOW
	{
		constexpr ImGuiWindowFlags FLAGS_INIT =
			ImGuiWindowFlags_NoNav            |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoResize         |
			ImGuiWindowFlags_NoCollapse
		;
		constexpr ImGuiWindowFlags FLAGS =
			ImGuiWindowFlags_NoNav   |
			ImGuiWindowFlags_MenuBar
		;
		
		constexpr uint16_t MAX_GAMES_INIT = 5'000; // Most amount games you can initialize
		constexpr uint16_t MAX_GAMES_ADD = 100;     // Most amount games you can add at a time
		constexpr ImVec2   MIN_SIZE{ 200.0f, 100.0f };
	}

	namespace GAME_WINDOW
	{
		constexpr ImGuiWindowFlags FLAGS =
			ImGuiWindowFlags_NoNav   |
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoScrollbar
		;
		constexpr ImVec2 DEFAULT_SIZE{ WINDOW::CENTER.x, WINDOW::CENTER.y }; // Same as half WINDOW:WIDTH and WINDOW::HEIGHT
	}

	namespace PLOT_WINDOW
	{
		constexpr ImGuiWindowFlags FLAGS =
			ImGuiWindowFlags_NoNav       |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse
		;

		constexpr ImVec2 PLOT_SIZE{ 600.0f, 300.0f };
		constexpr float  SCALE = 0.0005f; // How much to scale down the fitness on the plots

		constexpr ImVec4 AVG_FIT_COL{ 1.0f, 0.5f, 0.0f, 0.8f }; // Orange
		constexpr ImVec4 TOP_FIT_COL{ 0.0f, 0.5f, 1.0f, 0.8f }; // Blue

		constexpr ImVec4 LEFT_COL  = AVG_FIT_COL;
		constexpr ImVec4 RIGHT_COL = TOP_FIT_COL;

		constexpr size_t LIVE_N = 30'000; // How many milliseconds to save the fitness of the current generation
		constexpr size_t  GEN_N = 200;    // How many generations to plot the fitness for
	}

	namespace TAB_BARS
	{
		constexpr ImGuiTabBarFlags FLAGS =
			ImGuiTabBarFlags_Reorderable                  |
			ImGuiTabBarFlags_FittingPolicyScroll          |
			ImGuiTabBarFlags_NoCloseWithMiddleMouseButton
		;
	}
}