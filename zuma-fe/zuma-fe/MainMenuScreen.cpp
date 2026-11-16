#include "MainMenuScreen.h"
#include "GameApplication.h"
#include "UIConstants.h"

MainMenuScreen::MainMenuScreen(GameApplication& app)
    : m_app(app),
    m_playBtn({ 275, 250, 250, 55 }, "PLAY", { 120, 40, 200, 255 }, { 170, 60, 255, 255 }),
    m_difficultyBtn({ 275, 325, 250, 55 }, "DIFFICULTY", { 90, 30, 160, 255 }, { 140, 50, 220, 255 }),
    m_exitBtn({ 275, 400, 250, 55 }, "EXIT", { 160, 30, 80, 255 }, { 220, 40, 110, 255 }) {
}

void MainMenuScreen::update(float deltaTime) {
    if (m_playBtn.isClicked()) {
        m_app.switchToGameplay();
    }
    else if (m_difficultyBtn.isClicked()) {
        m_app.switchToDifficultyMenu();
    }
    else if (m_exitBtn.isClicked()) {
        m_app.requestExit();
    }
}

void MainMenuScreen::render() const {
    const char* title = "DISCO ZUMA";
    int titleSize = 68;
    int titleW = MeasureText(title, titleSize);

    DrawText(title, (UITheme::SCREEN_WIDTH - titleW) / 2 + 4, 104, titleSize, UITheme::NEON_PINK);
    DrawText(title, (UITheme::SCREEN_WIDTH - titleW) / 2, 100, titleSize, UITheme::NEON_CYAN);

    m_playBtn.render();
    m_difficultyBtn.render();
    m_exitBtn.render();
}