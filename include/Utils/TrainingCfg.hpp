#pragma once

namespace PongAI::CONFIG::TRAINING
{
	constexpr int BOT_INIT_SEED = 123456;

	inline bool StochasticPolicy = true; // Whether or not the Bots' actions are selected probabilistically or deterministically (the more Bots you have with this set to true, the slower they will calculate outputs)

	inline double EpisodeLength = 120.0; constexpr double EPISODE_LENGTH_MIN = 30.0; constexpr double EPISODE_LENGTH_MAX = 600.0; // 10 minutes max (at 1.0x game speed)
	inline size_t EliteCount = 10; // How many Elite Bots from each side to select for crossover

	inline float CrossoverRate    = 0.1f; // Probability that a neural network parameter of the parents are swapped

	inline float MutationRate     = 0.3f; // Probability that a neural network parameter being changed
	inline float MutationStrength = 1.5f; /* Gaussian Mutation(Mutate by normal distribution output) */ constexpr float MUTATION_STRENGTH_MAX = 10.0f;

	inline float SideBias = 5.0f; /* The activation for the Bots' input bias neuron depending on which side its Paddle is on: -SideBias = Left, SideBias = Right */ constexpr float SIDE_BIAS_MAX = 20.0f;

	namespace REWARDS
	{
		inline double Win    = 100.0; constexpr double WIN_MAX    = 100.0;
		inline double Score  = 20.0;  constexpr double SCORE_MAX  = 50.0;
		inline double Bounce = 30.0;  constexpr double BOUNCE_MAX = 50.0;

		// Rewarded for playing longer
		inline double Living  = 1.5; /* PER TICK */ constexpr double LIVING_MAX = 10.0;
	}

	namespace PENALTIES
	{
		inline double Lose     = 80.0; constexpr double LOSE_MAX      = 100.0;
		inline double ScoredOn = 50.0; constexpr double SCORED_ON_MAX = 50.0;

		inline double BallDist   = 0.30; /* PER TICK AND PER PIXEL */ constexpr double BALL_DIST_MAX   = 1.0;
		inline double CenterDist = 0.15; /* PER TICK AND PER PIXEL */ constexpr double CENTER_DIST_MAX = 0.3; // Generally better to use CenterDist early in training, so the Bots don't just stick to the corners. But you should disable it later
	}
}