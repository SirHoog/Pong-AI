#pragma once

#include <functional>

#include "GameWindow.hpp"

namespace PongAI
{
	struct Application;

	// Titled Dashboard in the actual UI but titled "Initialization" at the start
	struct MainWindow : Window
	{
		uint16_t gamesN = 1;
		bool initialized = false;
		
		MainWindow(Application* app);

		void draw() override;

		void updateGenFitPlot();
	private:
		Application* m_app = nullptr;

		PlotWindow m_liveFitPlot;
		PlotWindow m_genFitPlot;

		size_t m_botsN = 0;

		void init();

		void loopGames(std::function<void(Game&)> func);

		void MenuBar();

		void TrainingHeader();
		void GA_Tree();
		void RewardSliders();
	};
}