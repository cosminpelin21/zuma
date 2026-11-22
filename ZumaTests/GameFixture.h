#pragma once
#include <gtest/gtest.h>
#include "Game.h"

// Fixture comun: fiecare test primeste un Game nou, in starea Ready
class GameTest : public ::testing::Test {
protected:
    Game game;
};