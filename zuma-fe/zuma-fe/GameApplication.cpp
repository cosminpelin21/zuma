#include "GameApplication.h"
#include "MainMenuScreen.h"
#include "DifficultyScreen.h"
#include "GameplayScreen.h"
#include "raylib.h"

GameApplication::GameApplication()
    : m_difficulty(UIDifficulty::Medium), m_shouldExit(false) {
    InitWindow(UITheme::SCREEN_WIDTH, UITheme::SCREEN_HEIGHT, "Disco Zuma");
    SetTargetFPS(60);

    m_assets.loadAll();
    switchToMainMenu();
}

GameApplication::~GameApplication() {
    m_activeScreen.reset();
    m_assets.unloadAll();
    CloseWindow();
}

void GameApplication::switchToMainMenu() {
    m_activeScreen = std::make_unique<MainMenuScreen>(*this);
}

void GameApplication::switchToDifficultyMenu() {
    m_activeScreen = std::make_unique<DifficultyScreen>(*this);
}

void GameApplication::switchToGameplay() {
    m_activeScreen = std::make_unique<GameplayScreen>(*this);
}

void GameApplication::drawBackground() const {
    ClearBackground(UITheme::BG_DARK);
    for (int x = 0; x < UITheme::SCREEN_WIDTH; x += 40) {
        DrawLine(x, 0, x, UITheme::SCREEN_HEIGHT, UITheme::GRID_LINE);
    }
    for (int y = 0; y < UITheme::SCREEN_HEIGHT; y += 40) {
        DrawLine(0, y, UITheme::SCREEN_WIDTH, y, UITheme::GRID_LINE);
    }
}

void GameApplication::run() {
    while (!WindowShouldClose() && !m_shouldExit) {
        if (IsKeyPressed(KEY_ONE)) {
            m_shouldExit = true;
        }

        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }

        float dt = GetFrameTime();

        if (m_activeScreen) {
            m_activeScreen->update(dt);
        }

        BeginDrawing();
        drawBackground();
        if (m_activeScreen) {
            m_activeScreen->render();
        }
        EndDrawing();
    }
}