#pragma once
#include "AssetManager.h"
#include "IScreen.h"
#include "UIConstants.h"
#include <memory>

class GameApplication {
private:
    AssetManager m_assets;
    std::unique_ptr<IScreen> m_activeScreen;
    Difficulty m_difficulty;
    bool m_shouldExit;

    void drawBackground() const;

public:
    GameApplication();
    ~GameApplication();

    void run();
    void requestExit() { m_shouldExit = true; }

    void switchToMainMenu();
    void switchToDifficultyMenu();
    void switchToGameplay();

    const AssetManager& getAssets() const { return m_assets; }
    void setDifficulty(Difficulty diff) { m_difficulty = diff; }
    Difficulty getDifficulty() const { return m_difficulty; }
};