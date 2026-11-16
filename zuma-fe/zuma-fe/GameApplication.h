#pragma once
#include "AssetManager.h"
#include "IScreen.h"
#include "UIConstants.h"
#include <memory>

class GameApplication {
private:
    AssetManager m_assets;
    std::unique_ptr<IScreen> m_activeScreen;
    UIDifficulty m_difficulty;
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
    void setDifficulty(UIDifficulty diff) { m_difficulty = diff; }
    UIDifficulty getDifficulty() const { return m_difficulty; }
};