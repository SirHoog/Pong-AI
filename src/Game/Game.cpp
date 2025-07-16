#include "Game/Game.hpp"

namespace PongAI
{
    static SDL_FPoint posL, posR;
    static SDL_FPoint bPos, bVel;

    using namespace TRAINING;
    using namespace REWARDS;
    using namespace PENALTIES;

    void Game::update(SDL_Window* window, double& gameSpeed, double& dt)
    {
        if (paused) return;

        if (autoStart) ball.shoot();

        ball.update(window, gameSpeed, dt);

        bPos = ball.getPosition();
        bVel = ball.getVelocity();

        handleScoring();

        const SDL_FRect rectL = paddleL.getRect();
        const SDL_FRect rectR = paddleR.getRect();

        posL = { rectL.x, rectL.y };
        posR = { rectR.x, rectR.y };

        if (updatePaddle(paddleL, paddleR, window, gameSpeed, dt))
            updatePaddle(paddleR, paddleL, window, gameSpeed, dt);
    }

    void Game::draw(SDL_Renderer* renderer)
    {
        if (!visible) return;

        paddleL.draw(renderer);
        paddleR.draw(renderer);
        ball   .draw(renderer);
    }

    void Game::handleScoring()
    {
        if (bPos.x <= BALL::RADIUS)
        {
            ++paddleR.score;

            if (paddleR.bot) paddleR.bot->reward(Score);
            if (paddleL.bot) paddleL.bot->punish(ScoredOn);

            ball.reset();
        }
        else if (bPos.x >= WINDOW::WIDTH - BALL::RADIUS)
        {
            ++paddleL.score;

            if (paddleL.bot) paddleL.bot->reward(Score);
            if (paddleR.bot) paddleR.bot->punish(ScoredOn);

            ball.reset();
        }
    }

    // Returns if one side won so the other doesn't have to be checked
    bool Game::updatePaddle(Paddle& paddle, Paddle& other, SDL_Window*& window, double& gameSpeed, double& dt)
    {
        const SDL_FRect rect = paddle.getRect();

        if (paddle.score >= PADDLE::MAX_SCORE)
        {
            paddle.bot->reward(Win);
            other .bot->punish(Lose);
            pause();
            return false;
        }

        if (Paddle::Player == &paddle)
            paddle.updateAsPlayer();
        else
            paddle.updateAsBot({
                paddle.getSide() * SideBias,
                posL.y, posR.y,
                bPos.x, bPos.y,
                bVel.x, bVel.y
            }); // 7 inputs

        if (ball.collided(rect) && paddle.bot)
            paddle.bot->reward(Bounce);

        paddle.update(window, gameSpeed, dt);

        // Update Bots' fitness
        if (Paddle::Player != &paddle && paddle.bot)
        {
            const double bD = static_cast<double>(std::abs(bPos.y           - rect.y - PADDLE::CENTER.y));
            const double cD = static_cast<double>(std::abs(WINDOW::CENTER.y - rect.y - PADDLE::CENTER.y));

            paddle.bot->reward(Living * gameSpeed * dt);
            paddle.bot->punish({
                bD * BallDist,
                cD * CenterDist
            });
        }

        return true;
    }
}