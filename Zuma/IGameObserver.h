#pragma once
#include "Enums.h"

class IGameObserver {
public:
    virtual ~IGameObserver() = default;

    virtual void onBoardUpdated() = 0;
    virtual void onScoreChanged(int newScore) = 0;
    virtual void onGameStateChanged(GameState newState) = 0;
};