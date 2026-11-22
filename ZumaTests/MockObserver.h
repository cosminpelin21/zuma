#pragma once
#include <gmock/gmock.h>
#include "IGameObserver.h"

class MockObserver : public IGameObserver {
public:
    MOCK_METHOD(void, onBoardUpdated, (), (override));
    MOCK_METHOD(void, onScoreChanged, (int newScore), (override));
    MOCK_METHOD(void, onGameStateChanged, (GameState newState), (override));
};