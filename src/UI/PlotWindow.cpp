#include "UI/PlotWindow.hpp"

namespace PongAI
{
	using namespace ImGui;
	using namespace ImPlot;

	PlotWindow::PlotWindow(std::string title, const char* xLabel_, const char* yLabel_, std::vector<SB> lines_)
		: Window(title, PLOT_WINDOW::FLAGS),
		xLabel(xLabel_), yLabel(yLabel_), lines(lines_)
	{
		m_size    = PLOT_WINDOW::PLOT_SIZE;
		m_minSize = m_size;
		m_maxSize = m_size;
	}

	void PlotWindow::draw()
	{
		if (!m_open || lines.empty()) return;

		Window::draw();

		if (m_active && BeginPlot(("##" + title).c_str(), m_size, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame))
		{
			SetupAxes(xLabel, yLabel, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal);

			for (SB& line : lines)
			{
				if (line.data.empty()) break;

				SetNextLineStyle(line.color);
				PlotLine(
					line.label,
				   &line.data[0].x,
				   &line.data[0].y,
					line.data.size(),
					0,
					line.offset,
					sizeof(float) * 2 // ImVec2 size
				);
			}

			EndPlot();
		}

		End();
	}
}