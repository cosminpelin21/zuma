#include "DifficultyScreen.h"
#include "GameApplication.h"
#include "UIConstants.h"

DifficultyScreen::DifficultyScreen(GameApplication& app)
    : m_app(app),
    m_easyBtn({ 260, 200, 280, 52 }, "EASY", { 30, 150, 90, 255 }, { 50, 200, 120, 255 }),
    m_mediumBtn({ 260, 270, 280, 52 }, "MEDIUM", { 180, 130, 20, 255 }, { 230, 170, 30, 255 }),
    m_hardBtn({ 260, 340, 280, 52 }, "HARD", { 180, 30, 60, 255 }, { 240, 50, 90, 255 }),
    m_backBtn({ 260, 440, 280, 45 }, "BACK", { 70, 70, 100, 255 }, { 110, 110, 150, 255 }, 20) {
}

void DifficultyScreen::update(float deltaTime) {
    if (m_easyBtn.isClicked()) {
        m_app.setDifficulty(UIDifficulty::Easy);
        m_app.switchToGameplay();
    }
    else if (m_mediumBtn.isClicked()) {
        m_app.setDifficulty(UIDifficulty::Medium);
        m_app.switchToGameplay();
    }
    else if (m_hardBtn.isClicked()) {
        m_app.setDifficulty(UIDifficulty::Hard);
        m_app.switchToGameplay();
    }
    else if (m_backBtn.isClicked()) {
        m_app.switchToMainMenu();
    }
}

void DifficultyScreen::render() const {
    const char* title = "SELECT DIFFICULTY";
    int titleW = MeasureText(title, 40);
    DrawText(title, (UITheme::SCREEN_WIDTH - titleW) / 2, 90, 40, WHITE);

    m_easyBtn.render();
    m_mediumBtn.render();
    m_hardBtn.render();
    m_backBtn.render();
}