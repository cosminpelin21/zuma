#pragma once
#include "IScreen.h"
#include "Button.h"
#include "IGame.h"
#include "AssetManager.h"
#include <vector>
#include <memory>

class GameApplication;

class GameplayScreen : public IScreen, public IGameObserver {
private:
    GameApplication& m_app;
    std::unique_ptr<IGame> m_game;

    int m_displayedScore = 0;
    GameState m_currentState = GameState::Ready;
    Vector2 m_vortexPosition{ 0.0f, 0.0f };

    Button m_pauseBtn;
    Button m_resumeBtn;
    Button m_menuBtn;
    Button m_playAgainBtn;
    
    void renderTrack() const;
    void renderBalls() const;
    void renderLauncher() const;
    void renderOverlay(const char* title, Color titleColor, bool isPaused) const;

public:
    explicit GameplayScreen(GameApplication& app);
    ~GameplayScreen() override;

    void onBoardUpdated() override;
    void onScoreChanged(int newScore) override;
    void onGameStateChanged(GameState newState) override;

    void update(float deltaTime) override;
    void render() const override;
};