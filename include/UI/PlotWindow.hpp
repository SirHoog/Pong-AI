#pragma once

#include <vector>
#include <stdexcept>

#include <implot.h>

#include "Window.hpp"

namespace PongAI
{
	struct PlotWindow : Window
	{
		std::vector<SB> lines;
		const char* xLabel = "X";
		const char* yLabel = "Y";

		PlotWindow(std::string title, const char* xLabel_, const char* yLabel_, std::vector<SB> lines_);

		void draw() override;

		SB& operator[](int index)
		{
			if (index < 0 || index >= lines.size())
				throw std::out_of_range("Index given outside range of `PlotWindow::lines`");
			return lines[index];
		}
	};
}