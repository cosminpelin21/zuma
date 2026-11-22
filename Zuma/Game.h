#pragma once
#include "IGame.h"
#include "Ball.h"
#include <vector>
#include <deque>
#include <random>

class Game : public IGame {
private:
    GameState m_state;
    Difficulty m_difficulty;
    int m_score;
    int m_nextBallId;

    float m_trackSpeed;
    int m_initialBallCount;
    int m_colorCount;

    std::vector<Point2D> m_path;
    std::vector<float> m_segmentLengths;
    float m_totalPathLength;

    std::deque<Ball> m_trackBalls;
    std::vector<Ball> m_firedBalls;
    LauncherData m_launcher;

    std::vector<IGameObserver*> m_observers;
    std::mt19937 m_rng;

    void initEllipticSpiralMap();
    Point2D getPointAtDistance(float distance) const;
    BallColor generateInitialColor();
    BallColor generateLauncherColor();
    void updateTrackBalls(float deltaTime);
    void updateFiredBalls(float deltaTime);
    void checkCollisions();
    void insertBallIntoTrack(size_t index, const Ball& firedBall);
    void checkAndRemoveMatches(size_t insertedIndex);
    void setState(GameState newState);
    void addScore(int points);

    void notifyBoardUpdated();
    void notifyScoreChanged();
    void notifyStateChanged();

public:
    Game();

    void setDifficulty(Difficulty difficulty) override;
    void start() override;
    void pause() override;
    void resume() override;
    void update(float deltaTime) override;

    void aimAt(Point2D target) override;
    void shoot() override;
    void swapBalls() override;

    int getScore() const override { return m_score; }
    GameState getState() const override { return m_state; }
    LauncherData getLauncherData() const override { return m_launcher; }
    std::vector<BallData> getTrackBallsData() const override;
    std::vector<BallData> getFiredBallsData() const override;
    std::vector<Point2D> getPathPoints() const override { return m_path; }

    void addObserver(IGameObserver* observer) override;
    void removeObserver(IGameObserver* observer) override;
};