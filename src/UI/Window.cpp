#include "UI/Window.hpp"

namespace PongAI
{
	void Window::draw()
	{
		using namespace ImGui;

		if ((m_flags & ImGuiWindowFlags_AlwaysAutoResize) != ImGuiWindowFlags_AlwaysAutoResize)
		{
			SetNextWindowSizeConstraints(m_minSize, m_maxSize);
			SetNextWindowSize(m_size, m_cond);
		}

		m_active = Begin(title.c_str(), &m_open, m_flags);

		// Don't forget to call ImGui::End() on the Window
	}
}