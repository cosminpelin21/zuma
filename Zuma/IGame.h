#pragma once
#include "DataTypes.h"
#include "IGameObserver.h"
#include <vector>
#include <memory>

class IGame {
public:
    virtual ~IGame() = default;

    // Control joc
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void update(float deltaTime) = 0;

    // Actiuni jucator (UI)
    virtual void aimAt(Point2D target) = 0;
    virtual void shoot() = 0;
    virtual void swapBalls() = 0;

    // Date pentru randare
    virtual int getScore() const = 0;
    virtual GameState getState() const = 0;
    virtual LauncherData getLauncherData() const = 0;
    virtual std::vector<BallData> getTrackBallsData() const = 0;
    virtual std::vector<BallData> getFiredBallsData() const = 0;
    virtual std::vector<Point2D> getPathPoints() const = 0;

    // Observer pattern
    virtual void addObserver(IGameObserver* observer) = 0;
    virtual void removeObserver(IGameObserver* observer) = 0;
};

// Factory function pentru a instantia jocul fara a expune clasa Game in UI
std::unique_ptr<IGame> createGame();