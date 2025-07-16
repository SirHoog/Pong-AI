#pragma once

#include <vector>
#include <numeric> // For std::reduce
#include <memory>
#include <utility>
#include <algorithm>
#include <random>
#include <iostream>
#include <stdexcept>
#include <string>

#include <MiniDNN.h>

#include "Utils/TrainingCfg.hpp"
#include "Utils/RNG.hpp"

namespace PongAI
{
	using namespace MiniDNN;

	typedef Eigen::MatrixXd Matrix;
	typedef Eigen::VectorXd Vector;

	enum Direction
	{
		Up = -1,
		Down = 1
	};

	using namespace CONFIG;

	struct Bot
	{
		const uint8_t inputSize;

		Bot(const uint8_t inputSize_ = 7, const uint8_t hiddenLayerSize = 2, const uint8_t hiddenLayersCount = 2); // Output layer size is constant (2 for each Direction)
		explicit Bot(std::unique_ptr<Network> brain) : m_brain(std::move(brain)),            inputSize(brain->get_layers()[0]->in_size()) {};
		explicit Bot(std::vector<Layer> & layers)    : m_brain(std::make_unique<Network>()), inputSize(layers[0] .in_size()) { for (Layer & layer : layers) m_brain->add_layer(&layer); };
		explicit Bot(std::vector<Layer*>& layers)    : m_brain(std::make_unique<Network>()), inputSize(layers[0]->in_size()) { for (Layer*& layer : layers) m_brain->add_layer( layer); };

		// Move operations
		Bot           (Bot&&) noexcept = default;
		Bot& operator=(Bot&&) noexcept = default;

		// Delete copy operations
		Bot           (const Bot&) = delete;
		Bot& operator=(const Bot&) = delete;

		// Determined by a probability from the neural network's output after one forward pass (which is passed into the Softmax activation function)
		Direction update(const std::vector<float>& inputs);
		 
		/*
		 Draw the Bot's neural network as circles and lines
		 * Work in Progress
		*/
		void draw();

		// Genetic Algorithm (GA) Functions:
		// Inspired by: https://www.cs.cmu.edu/~02317/slides/lec_8.pdf and https://www.geeksforgeeks.org/genetic-algorithms/

		/* 
		 In-place crossover
		 * Crossover two Bots' neural networks, but reuse this Bot instance
		 * Used to avoid unnecessary allocating memory for a new Bot instance
		*/
		void crossoverReplace(const Bot* parentA, const Bot* parentB);
		/* 
		 Create a new Bot "child"
		 * For when you may want to increase Bot population
		 * Returns a new Bot instance
		*/
		Bot crossoverWith(const Bot* spouse) const;
		
		// Modifies the neural network's parameters according to Bot::mutationRate
		void mutate();

		// Fitness

		double getFitness() const { return m_fitness; };
		void   resetFitness()     { m_fitness = 0.0;  };

		void reward(double reward ) { m_fitness += reward;  };
		void punish(double penalty) { m_fitness -= penalty; };

		void reward(const std::vector<double>& rewards  ) { m_fitness += std::reduce(rewards  .begin(), rewards  .end()); };
		void punish(const std::vector<double>& penalties) { m_fitness -= std::reduce(penalties.begin(), penalties.end()); };

		// BRAINS!!!

		Network* getBrain() const            { return m_brain.get();      };
		std::unique_ptr<Network> moveBrain() { return std::move(m_brain); };

		void resetBrain() { m_brain.reset(); };
		void newBrain(std::unique_ptr<Network> brain) { m_brain = std::move(brain); };

		void saveBrain(const std::string& fileName, const std::string& dirPath = NN_DIR) const { m_brain->export_net(dirPath, fileName); };
		void loadBrain(const std::string& fileName, const std::string& dirPath = NN_DIR)       { m_brain->read_net  (dirPath, fileName); };
	private:
		double m_fitness = 0;

		std::unique_ptr<Network> m_brain = nullptr; // Unique pointer so that neural networks can be moved around easily
	};
}