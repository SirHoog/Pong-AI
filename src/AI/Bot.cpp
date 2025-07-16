#include "AI/Bot.hpp"

namespace PongAI
{
	// I explicitly use CONFIG::RNG because MiniDNN has a class called RNG
	using namespace TRAINING;

	Bot::Bot(const uint8_t inputSize_, const uint8_t hiddenLayerSize, const uint8_t hiddenLayersCount) : inputSize(inputSize_)
	{
		if (hiddenLayersCount == 0) // Not recommended... (Your AI would be really stupid)
		{
			// Input -> Output layer
			m_brain->add_layer(new FullyConnected<Softmax>(inputSize, 2));

			return;
		}

		m_brain = std::make_unique<Network>();
		
		// Input -> First Hidden Layer
		m_brain->add_layer(new FullyConnected<Sigmoid>(inputSize, hiddenLayerSize));

		// Middle Hidden Layers (which is why it already starts at 1)
		for (uint8_t i = 1; i < hiddenLayersCount; ++i)
			m_brain->add_layer(new FullyConnected<Sigmoid>(hiddenLayerSize, hiddenLayerSize));

		// Output Layer
		m_brain->add_layer(new FullyConnected<Softmax>(hiddenLayerSize, 2));

		m_brain->init(0, 0.01, BOT_INIT_SEED); // Assign random parameters
	}

	Direction Bot::update(const std::vector<float>& inputs)
	{
		if (inputs.size() != inputSize)
			throw std::invalid_argument("The provided `std::vector<float> inputs` of size " + std::to_string(inputs.size()) + " does not match neural network's initialized input size of " + std::to_string(inputSize) + ".");

		Matrix inputMat(inputSize, 1);
		inputMat.col(0) = Eigen::Map<const Eigen::RowVectorXf>(inputs.data(), inputSize).cast<double>();
		Matrix output = m_brain->predict(inputMat);

		// Probabilities
		// * Gets the activations of the output neurons (which are Softmaxxed)
		const double* probs = output.row(0).data();

		if (CONFIG::TRAINING::StochasticPolicy)
		{
			std::discrete_distribution<> dist(probs, probs + 2);
			uint8_t index = dist(RNG);

			switch (index)
			{
				case 0: return Up;
				case 1: return Down;
				default:
					throw std::exception("Neural network's output size exceeds expected size of 2");
					break;
			}
		}
		else
		{
			const double* prob  = std::max_element(probs, probs + 2);
			const uint8_t index = std::distance(probs, prob);

			switch (index)
			{
				case 0: return Up;
				case 1: return Down;
				default:
					throw std::exception("Neural network's output size exceeds expected size of 2");
					break;
			}
		}
	}

	void Bot::draw() // WIP
	{}

	void Bot::crossoverReplace(const Bot* parentA, const Bot* parentB)
	{
		if (!parentA || !parentB)
			throw std::invalid_argument("Both parent Bots must exist for crossover.");

		const Network* brainA = parentA->getBrain();
		const Network* brainB = parentB->getBrain();

		if (!brainA || !brainB)
			throw std::invalid_argument("Both parent Bots must have a neural network initialized before crossover.");

		// If this Bot doesn't have a brain, give it one lol
		if (!m_brain)
			m_brain = std::make_unique<Network>(*brainA); // Copy brainA

		const std::vector<const Layer*> layers  = m_brain->get_layers();
		const std::vector<const Layer*> layersA = brainA ->get_layers();
		const std::vector<const Layer*> layersB = brainB ->get_layers();

		if (layers.size() != layersA.size() || layers.size() != layersB.size())
			throw std::invalid_argument("Bots must have the same number of layers");

		std::uniform_real_distribution<float> crossoverProb(0.0f, 1.0f);

		
		// Final neural network parameters
		// * Note: `params` initially inherits brainA parameters
		std::vector<std::vector<double>> params(brainA->get_parameters());

		for (size_t i = 0; i < layers.size(); ++i)
		{
			const std::vector<double> paramsB = layersB[i]->get_parameters();

			if (layersA[i]->get_parameters().size() != paramsB.size())
				throw std::runtime_error("Parent Bots must have the same number of parameters in layer " + std::to_string(i));

			for (size_t j = 0; j < params[i].size(); ++j)
				if (crossoverProb(RNG) < CrossoverRate) // Probability that parameter is taken from brainB parameter
					params[i][j] = paramsB[j]; // Override initially inherited brainA parameters
		}

		m_brain->set_parameters(params);
	}

	Bot Bot::crossoverWith(const Bot* spouse) const
	{
		Bot child;

		child.crossoverReplace(this, spouse);

		return child;
	}

	void Bot::mutate()
	{
		if (!m_brain)
			throw std::exception("Your Bot is stupid (it has no brain).");

		const std::vector<const Layer*>& layers = m_brain->get_layers();

		std::uniform_real_distribution<float> mutationProb(0.0f, 1.0f);
		std::uniform_real_distribution<float> mutationAmountProb(-MutationStrength, MutationStrength);

		std::vector<std::vector<double>> params(m_brain->get_parameters()); // Final neural network parameters

		for (size_t i = 0; i < layers.size(); ++i)
			for (size_t j = 0; j < params[i].size(); ++j)
				if (mutationProb(RNG) < MutationRate) // Probability that neuron is mutated
					params[i][j] += mutationAmountProb(RNG); // Override old neuron parameter with mutated neuron parameter (randomized)

		m_brain->set_parameters(params);
	}
}