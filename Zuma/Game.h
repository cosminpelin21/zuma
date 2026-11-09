#pragma once
#include "Ball.h"
#include <vector>

class Game {
private:
    GameState m_state;
    int m_score;
    int m_nextBallId;
    std::vector<Ball> m_balls;

public:
    Game();

    void start();
    void pause();
    void resume();
    void update(float deltaTime);

    void spawnBall(Point2D position, BallColor color);

    int getScore() const 
    { 
        return m_score;
    }

    GameState getState() const 
    {
        return m_state;
    }

    const std::vector<Ball>& getBalls() const 
    { 
        return m_balls;
    }

    std::vector<BallData> getBallsData() const;
};