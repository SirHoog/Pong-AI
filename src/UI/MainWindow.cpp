#include "Application.hpp" // For the actual definition of the Application class

namespace PongAI
{
	using namespace ImGui;

    using namespace MAIN_WINDOW;
    using namespace PLOT_WINDOW;

    MainWindow::MainWindow(Application* app)
        : Window("Pong AI", FLAGS_INIT), m_app(app),
        m_liveFitPlot{ "Live Fitness", "Seconds", "Fitness",
            {
                SB(LIVE_N, "Top Fitness",     TOP_FIT_COL),
                SB(LIVE_N, "Average Fitness", AVG_FIT_COL)
            }},
        m_genFitPlot{"Generational Fitness", "Generation", "Fitness",
            {
                SB(GEN_N, "Top Fitness",     TOP_FIT_COL),
                SB(GEN_N, "Average Fitness", AVG_FIT_COL)
            }}
    {}

	void MainWindow::init()
	{
        const uint16_t stepFast = 25;
        TextUnformatted("Initialize "); SameLine();
        // Fit input width to text size
        int len = snprintf(nullptr, 0, "Initialize %d Games", gamesN);
        char* text = (char*)malloc(len+1); sprintf(text, "Initialize %d Games", gamesN);
        SetNextItemWidth(CalcTextSize(text).x + GetStyle().FramePadding.x * 2); free(text);

        InputScalar("##init", ImGuiDataType_U16, &gamesN, &oU16, &stepFast, "%u Games"); // Format input in-between

		if (gamesN < 2) gamesN = 2; // Must be at least 2 games (for Bot crossover)
		if (gamesN > MAX_GAMES_INIT) gamesN = MAX_GAMES_INIT;

		if (Button("Start"))
		{
			initialized = true;
			title = "Dashboard"; // Rename title to normal
			m_flags = MAIN_WINDOW::FLAGS; // Use normal flags after initialization

            m_minSize = MIN_SIZE;
            m_maxSize = { WINDOW::WIDTH, WINDOW::HEIGHT };

            m_app->m_UI.addGameWindows(gamesN);

            gamesN = 1; // Reset to 1 to reuse for adding N games
		}
		SameLine();
		if (Button("Close")) m_app->running = false;
	}
	void MainWindow::draw()
	{
		if (!m_open) m_app->running = false;

		if (!initialized) // Center MainWindow during initialization
            SetNextWindowPos(GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		Window::draw();

		if (!initialized)
		{
			init(); End();
			return;
		}

        if (m_active)
        {
            MenuBar();
            TrainingHeader();
        }

        m_botsN = 0;

        float avgFit = 0.0f;
        float topFit = std::numeric_limits<float>::lowest();
        auto  getFit = [&](const Bot* bot)
            {
                ++m_botsN;
                const float fit = static_cast<float>(bot->getFitness());
                avgFit += fit;
                if (fit > topFit) topFit = fit;
            };

        loopGames([&](Game& g) {
            if (const Bot* bL = g.paddleL.bot.get()) getFit(bL);
            if (const Bot* bR = g.paddleR.bot.get()) getFit(bR);
        });

        if (m_botsN == 0) avgFit = 0.0f; // Avoid dividing by zero
        else              avgFit /= m_botsN;

        m_liveFitPlot[0].addPoint(GetTime(), topFit * PLOT_WINDOW::SCALE);
        m_liveFitPlot[1].addPoint(GetTime(), avgFit * PLOT_WINDOW::SCALE);

        m_liveFitPlot.draw();
        m_genFitPlot .draw();

		End();
	}

    void MainWindow::updateGenFitPlot()
    {
        m_genFitPlot[0].addPoint(m_app->m_gensN, m_liveFitPlot[0].data.back().y);
        m_genFitPlot[1].addPoint(m_app->m_gensN, m_liveFitPlot[1].data.back().y);
    }

    void MainWindow::loopGames(std::function<void(Game&)> func)
    {
        for (auto& gameWindow : m_app->m_UI.gameWindows)
            func(gameWindow->game);
    }

	void MainWindow::MenuBar()
	{
        if (BeginMenuBar())
        {
            if (BeginMenu("Edit"))
            {
				if (BeginMenu("Add Games"))
				{
					const uint16_t stepFast = 10;
                    if (Button("Add "))
                        m_app->m_UI.addGameWindows(gamesN);
                    SameLine();
                    // Fit input width to text size
                    int len = snprintf(nullptr, 0, "Add %d Games", gamesN);
                    char* text = (char*)malloc(len + 1); sprintf(text, "Add %d Games", gamesN);
                    SetNextItemWidth(CalcTextSize(text).x + GetStyle().FramePadding.x * 10); free(text);

                    InputScalar("##add", ImGuiDataType_U16, &gamesN, &oU16, &stepFast, "%u Games");
				    if (gamesN < 1) gamesN = 1;
					if (gamesN > MAX_GAMES_ADD) gamesN = MAX_GAMES_ADD;

                    EndMenu();
                }

                Separator();

                if (BeginMenu("Game Controls"))
                {
                    TextUnformatted("* Applies to all games *");

                    if (Button("Disable Auto-start")) loopGames([](Game& g) { g.autoStart = false; });
                    if (Button("Enable Auto-start" )) loopGames([](Game& g) { g.autoStart = true;  });

                    Separator();
                    if (Button("Pause"  )) loopGames([](Game& g) { g.pause();   });
                    if (Button("Unpause")) loopGames([](Game& g) { g.unpause(); });

                    Separator();
                    if (Button("Hide")) loopGames([](Game& g) { g.hide(); });
                    if (Button("Show")) loopGames([](Game& g) { g.show(); });

                    Separator();
                    if (BeginMenu("Game Speed Multiplier"))
                    {
                        SliderScalar("##GameSpeedMult", ImGuiDataType_Double, &m_app->m_gameSpeed, &zD, &m_app->m_maxGameSpeed, "%.2f");
                        EndMenu();
                    }

                    EndMenu();
                }

                EndMenu();
            }
            if (BeginMenu("View"))
            {
				if (Button("Open Live Fitness Plot")) m_liveFitPlot.open();
				if (Button("Open Generational Fitness Plot")) m_genFitPlot.open();
                Separator();
                if (Checkbox("Track Top Bot", &m_app->m_trackTopBot)) {};

                EndMenu();
            }

            EndMenuBar();
        }
	}

	void MainWindow::TrainingHeader()
	{
		ValueBox("Generation", m_app->m_gensN);
		Text("New Generation in: %.1f seconds", m_app->m_episodeTimer / m_app->m_gameSpeed);
		Checkbox("Continue Training", &m_app->m_training);

        GA_Tree();
		RewardSliders();
	}
    void MainWindow::GA_Tree()
    {
        if (TreeNode("Genetic Algorithm"))
        {
            using namespace TRAINING;

            Checkbox("Stochastic Policy", &StochasticPolicy);

            SliderScalar("Side Bias",          ImGuiDataType_Float,  &SideBias,         &oF,                 &SIDE_BIAS_MAX,         "%.2f");
            SliderScalar("Max Episode Length", ImGuiDataType_Double, &EpisodeLength,    &EPISODE_LENGTH_MIN, &EPISODE_LENGTH_MAX,    "%.2f");                                  Separator();

            SliderScalar("Selection Size",     ImGuiDataType_U64,    &EliteCount,       &oU64,               &m_botsN                      );
            SliderScalar("Crossover Rate",     ImGuiDataType_Float,  &CrossoverRate,    &zF,                 &oF,                    "%.2f");                                  Separator();

            SliderScalar("Mutation Rate",      ImGuiDataType_Float,  &MutationRate,     &zF,                 &oF,                    "%.2f");
            SliderScalar("Mutation Strength",  ImGuiDataType_Float,  &MutationStrength, &zF,                 &MUTATION_STRENGTH_MAX, "%.2f");

            TreePop();
        }
    }
    void MainWindow::RewardSliders()
    {
        using namespace TRAINING;

        auto RewardSlider = [](auto& label, auto& value, auto& max) { SliderScalar(label, ImGuiDataType_Double, &value, &zD, &max, "%.2f"); };

        if (TreeNode("Rewards"))
        {
            using namespace REWARDS;

            RewardSlider("Win",    Win,    WIN_MAX   );
            RewardSlider("Score",  Score,  SCORE_MAX );

            RewardSlider("Bounce", Bounce, BOUNCE_MAX);
            RewardSlider("Living", Living, LIVING_MAX);

            TreePop();
        }
        if (TreeNode("Penalties"))
        {
            using namespace PENALTIES;

            RewardSlider("Lose",            Lose,       LOSE_MAX       );
            RewardSlider("ScoredOn",        ScoredOn,   SCORED_ON_MAX  );

            RewardSlider("Ball Distance",   BallDist,   BALL_DIST_MAX  );
            RewardSlider("Center Distance", CenterDist, CENTER_DIST_MAX);

            TreePop();
        }
    }
}