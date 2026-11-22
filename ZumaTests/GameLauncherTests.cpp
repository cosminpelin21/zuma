// Teste pentru lansator: tintire, tragere, schimbarea bilelor
#include "GameFixture.h"
#include "TestHelpers.h"

// ---- aimAt ---------------------------------------------------------------
TEST_F(GameTest, AimAtRightGivesAngleZero) {
    game.aimAt({ 550.0f, 300.0f });
    EXPECT_NEAR(game.getLauncherData().angleRadians, 0.0f, 1e-5f);
}

TEST_F(GameTest, AimAtDownGivesHalfPi) {
    game.aimAt({ 450.0f, 400.0f });
    EXPECT_NEAR(game.getLauncherData().angleRadians, 1.5707963f, 1e-4f);
}

TEST_F(GameTest, AimAtLeftGivesPi) {
    game.aimAt({ 350.0f, 300.0f });
    EXPECT_NEAR(game.getLauncherData().angleRadians, 3.1415926f, 1e-4f);
}

// ---- shoot ---------------------------------------------------------------
TEST_F(GameTest, ShootDoesNothingWhenNotPlaying) {
    game.shoot();
    EXPECT_TRUE(game.getFiredBallsData().empty());
}

TEST_F(GameTest, ShootCreatesProjectileWithLauncherColor) {
    game.start();
    LauncherData before = game.getLauncherData();

    game.shoot();

    auto fired = game.getFiredBallsData();
    ASSERT_EQ(fired.size(), 1u);
    EXPECT_EQ(fired[0].color, before.currentColor);
    // culoarea "next" devine "current"
    EXPECT_EQ(game.getLauncherData().currentColor, before.nextColor);
}

TEST_F(GameTest, FiredBallMovesInAimedDirection) {
    game.start();
    game.aimAt({ 550.0f, 300.0f }); // spre dreapta
    game.shoot();

    game.update(0.1f); // 650 * 0.1 = 65 unitati

    auto fired = game.getFiredBallsData();
    ASSERT_EQ(fired.size(), 1u);
    expectPointNear(fired[0].position, 515.0f, 300.0f, 0.1f);
}

TEST_F(GameTest, FiredBallIsRemovedWhenOutOfBounds) {
    game.start();
    game.aimAt({ 550.0f, 300.0f });
    game.shoot();

    game.update(5.0f); // ajunge mult in afara ecranului

    EXPECT_TRUE(game.getFiredBallsData().empty());
}

// ---- swapBalls -----------------------------------------------------------
TEST_F(GameTest, SwapBallsExchangesLauncherColors) {
    game.start();
    LauncherData before = game.getLauncherData();

    game.swapBalls();

    LauncherData after = game.getLauncherData();
    EXPECT_EQ(after.currentColor, before.nextColor);
    EXPECT_EQ(after.nextColor, before.currentColor);
}