// Teste pentru notificarile catre observeri (folosesc GMock)
#include "GameFixture.h"
#include "MockObserver.h"

using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::StrictMock;

TEST_F(GameTest, StartNotifiesObserver) {
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);

    EXPECT_CALL(obs, onGameStateChanged(GameState::Playing)).Times(1);
    EXPECT_CALL(obs, onScoreChanged(0)).Times(1);
    EXPECT_CALL(obs, onBoardUpdated()).Times(1);

    game.start();
}

TEST_F(GameTest, UpdateNotifiesBoardUpdated) {
    game.start();
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);

    EXPECT_CALL(obs, onBoardUpdated()).Times(1);

    game.update(0.016f);
}

TEST_F(GameTest, PauseNotifiesStateChanged) {
    game.start();
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);

    EXPECT_CALL(obs, onGameStateChanged(GameState::Paused)).Times(1);

    game.pause();
}

TEST_F(GameTest, GameOverNotifiesObserver) {
    game.start();
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);

    EXPECT_CALL(obs, onGameStateChanged(GameState::GameOver)).Times(1);
    EXPECT_CALL(obs, onBoardUpdated()).Times(AtLeast(1));

    game.update(10000.0f);
}

TEST_F(GameTest, SwapBallsNotifiesBoardUpdated) {
    game.start();
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);

    EXPECT_CALL(obs, onBoardUpdated()).Times(1);

    game.swapBalls();
}

TEST_F(GameTest, SameObserverAddedTwiceIsNotifiedOnce) {
    NiceMock<MockObserver> obs;
    game.addObserver(&obs);
    game.addObserver(&obs);

    EXPECT_CALL(obs, onScoreChanged(0)).Times(1);

    game.start();
}

TEST_F(GameTest, RemovedObserverIsNotNotified) {
    StrictMock<MockObserver> obs; // orice apel neasteptat = esec
    game.addObserver(&obs);
    game.removeObserver(&obs);

    game.start();
    game.update(0.016f);
}

TEST_F(GameTest, AddingNullObserverDoesNotCrash) {
    game.addObserver(nullptr);
    EXPECT_NO_THROW(game.start());
}

TEST_F(GameTest, UpdateAfterGameOverDoesNotNotify) {
    game.start();
    game.update(10000.0f);
    ASSERT_EQ(game.getState(), GameState::GameOver);

    StrictMock<MockObserver> obs;
    game.addObserver(&obs);

    game.update(1.0f); // nu mai are voie sa notifice nimic
}