#include "MainMenuScreen.h"
#include "GameApplication.h"
#include "UIConstants.h"

MainMenuScreen::MainMenuScreen(GameApplication& app)
    : m_app(app),
    m_playBtn({ 275, 340, 250, 50 }, "PLAY", { 120, 40, 200, 255 }, { 170, 60, 255, 255 }),
    m_difficultyBtn({ 275, 410, 250, 50 }, "DIFFICULTY", { 90, 30, 160, 255 }, { 140, 50, 220, 255 }),
    m_exitBtn({ 275, 480, 250, 50 }, "EXIT", { 160, 30, 80, 255 }, { 220, 40, 110, 255 }) {
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
    const auto& assets = m_app.getAssets();
    if (assets.hasLogo()) {
        Texture2D logo = assets.getLogo();
        float scale = 0.23f;
        float w = logo.width * scale;
        float h = logo.height * scale;
        Vector2 pos = { (UITheme::SCREEN_WIDTH - w) / 2.0f, 34.0f };
        DrawTextureEx(logo, pos, 0.0f, scale, WHITE);
    }
    else {
        const char* title = "DISCO ZUMA";
        int titleSize = 68;
        int titleW = MeasureText(title, titleSize);
        DrawText(title, (UITheme::SCREEN_WIDTH - titleW) / 2 + 4, 104, titleSize, UITheme::NEON_PINK);
        DrawText(title, (UITheme::SCREEN_WIDTH - titleW) / 2, 100, titleSize, UITheme::NEON_CYAN);
    }

    m_playBtn.render();
    m_difficultyBtn.render();
    m_exitBtn.render();
}