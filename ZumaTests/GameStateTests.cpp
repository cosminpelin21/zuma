// Teste pentru ciclul de viata al jocului: stare, start/pause/resume,
// dificultate, miscarea lantului de bile, Game Over
#include <cmath>
#include <memory>
#include "GameFixture.h"
#include "TestHelpers.h"

// ---- Factory si stare initiala ------------------------------------------
TEST(GameFactoryTest, CreateGameReturnsValidInstance) {
    std::unique_ptr<IGame> g = createGame();

    ASSERT_NE(g, nullptr);
    EXPECT_EQ(g->getState(), GameState::Ready);
}

TEST_F(GameTest, InitialStateIsReady) {
    EXPECT_EQ(game.getState(), GameState::Ready);
    EXPECT_EQ(game.getScore(), 0);
    EXPECT_TRUE(game.getTrackBallsData().empty());
    EXPECT_TRUE(game.getFiredBallsData().empty());
}

TEST_F(GameTest, PathIsGeneratedOnConstruction) {
    // initEllipticSpiralMap: steps = 500 => 501 puncte
    EXPECT_EQ(game.getPathPoints().size(), 501u);
}

TEST_F(GameTest, LauncherStartsAtCenterRight) {
    expectPointNear(game.getLauncherData().position, 450.0f, 300.0f);
}

// ---- Start ---------------------------------------------------------------
TEST_F(GameTest, StartSetsStateToPlayingAndResetsScore) {
    game.start();

    EXPECT_EQ(game.getState(), GameState::Playing);
    EXPECT_EQ(game.getScore(), 0);
}

TEST_F(GameTest, StartAssignsLauncherColors) {
    game.start();

    LauncherData l = game.getLauncherData();
    EXPECT_NE(l.currentColor, BallColor::None);
    EXPECT_NE(l.nextColor, BallColor::None);
}

TEST_F(GameTest, StartCreatesBallsWithUniqueIds) {
    game.start();

    auto balls = game.getTrackBallsData();
    for (size_t i = 0; i < balls.size(); ++i)
        for (size_t j = i + 1; j < balls.size(); ++j)
            EXPECT_NE(balls[i].id, balls[j].id);
}

TEST_F(GameTest, StartPlacesBallsNoFurtherApartThanDiameter) {
    game.start();

    auto balls = game.getTrackBallsData();
    for (size_t i = 1; i < balls.size(); ++i) {
        float dx = balls[i].position.x - balls[i - 1].position.x;
        float dy = balls[i].position.y - balls[i - 1].position.y;
        EXPECT_LE(std::sqrt(dx * dx + dy * dy), 30.1f);
    }
}

// ---- Pause / Resume ------------------------------------------------------
TEST_F(GameTest, PauseOnlyWorksWhilePlaying) {
    game.pause();
    EXPECT_EQ(game.getState(), GameState::Ready);

    game.start();
    game.pause();
    EXPECT_EQ(game.getState(), GameState::Paused);
}

TEST_F(GameTest, ResumeOnlyWorksWhilePaused) {
    game.resume();
    EXPECT_EQ(game.getState(), GameState::Ready);

    game.start();
    game.pause();
    game.resume();
    EXPECT_EQ(game.getState(), GameState::Playing);
}

TEST_F(GameTest, UpdateDoesNothingWhilePaused) {
    game.start();
    auto before = game.getTrackBallsData();
    game.pause();

    game.update(1.0f);

    auto after = game.getTrackBallsData();
    ASSERT_EQ(before.size(), after.size());
    for (size_t i = 0; i < before.size(); ++i)
        expectPointNear(after[i].position, before[i].position.x, before[i].position.y);
}

// ---- Dificultate (test parametrizat) ------------------------------------
struct DifficultyCase {
    Difficulty difficulty;
    size_t expectedBalls;
    int expectedColors;
};

class GameDifficultyTest : public ::testing::TestWithParam<DifficultyCase> {};

TEST_P(GameDifficultyTest, StartCreatesExpectedBallCountAndColors) {
    Game game;
    game.setDifficulty(GetParam().difficulty);
    game.start();

    auto balls = game.getTrackBallsData();
    EXPECT_EQ(balls.size(), GetParam().expectedBalls);
    for (const auto& b : balls) {
        EXPECT_NE(b.color, BallColor::None);
        EXPECT_LT(static_cast<int>(b.color), GetParam().expectedColors);
    }
}

INSTANTIATE_TEST_SUITE_P(
    AllDifficulties, GameDifficultyTest,
    ::testing::Values(
        DifficultyCase{ Difficulty::Easy,   14, 4 },
        DifficultyCase{ Difficulty::Medium, 18, 5 },
        DifficultyCase{ Difficulty::Hard,   24, 6 }));

// ---- Miscarea lantului si Game Over -------------------------------------
TEST_F(GameTest, UpdateMovesLastTrackBallAlongPath) {
    game.start(); // Medium: speed = 40
    Point2D before = game.getTrackBallsData().back().position;

    game.update(1.0f);

    Point2D after = game.getTrackBallsData().back().position;
    float dx = after.x - before.x;
    float dy = after.y - before.y;
    EXPECT_NEAR(std::sqrt(dx * dx + dy * dy), 40.0f, 1.0f);
}

TEST_F(GameTest, GameOverWhenBallsReachEndOfPath) {
    game.start();

    game.update(10000.0f); // timp urias => lantul depaseste traseul

    EXPECT_EQ(game.getState(), GameState::GameOver);
}