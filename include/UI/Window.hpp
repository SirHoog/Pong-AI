#pragma once

#include <string>

#include "Utils/UI_Cfg.hpp"
#include "Utils/ImGuiHelpers.hpp"

namespace PongAI
{
	using namespace CONFIG::UI_CFG;
	using namespace CONFIG::IMGUI_HELPERS;

	struct Window
	{
		std::string title;

		Window() = delete;
		Window(std::string title_ = "", ImGuiWindowFlags flags = 0)
			: title(title_), m_flags(flags) {};

		virtual void draw(); // Code here for quick ImGui window initialization

		void open()  { m_open = true;  };
		void close() { m_open = false; };

		bool isOpen() const { return m_open; };
	protected:
		ImGuiWindowFlags m_flags;
		ImGuiCond m_cond = ImGuiCond_None;
		bool m_open = true;
		bool m_active = true;

		ImVec2 m_size;
		ImVec2 m_minSize;
		ImVec2 m_maxSize;
	};
}