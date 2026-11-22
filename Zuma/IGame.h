#pragma once
#include "DataTypes.h"
#include "IGameObserver.h"
#include <vector>
#include <memory>

class IGame {
public:
    virtual ~IGame() = default;

    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void update(float deltaTime) = 0;

    virtual void aimAt(Point2D target) = 0;
    virtual void shoot() = 0;
    virtual void swapBalls() = 0;

    virtual int getScore() const = 0;
    virtual GameState getState() const = 0;
    virtual LauncherData getLauncherData() const = 0;
    virtual std::vector<BallData> getTrackBallsData() const = 0;
    virtual std::vector<BallData> getFiredBallsData() const = 0;
    virtual std::vector<Point2D> getPathPoints() const = 0;

    virtual void setDifficulty(Difficulty difficulty) = 0;

    virtual void addObserver(IGameObserver* observer) = 0;
    virtual void removeObserver(IGameObserver* observer) = 0;
};

std::unique_ptr<IGame> createGame();