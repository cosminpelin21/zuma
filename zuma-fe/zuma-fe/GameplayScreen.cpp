#include "GameplayScreen.h"
#include "GameApplication.h"
#include "UIConstants.h"
#include <cmath>

GameplayScreen::GameplayScreen(GameApplication& app)
    : m_app(app),
    m_game(createGame()),
    m_pauseBtn({ 675, 12, 110, 38 }, "PAUSE", { 90, 30, 150, 255 }, { 140, 50, 210, 255 }, 20),
    m_resumeBtn({ 290, 250, 220, 50 }, "RESUME", { 40, 160, 100, 255 }, { 60, 210, 130, 255 }),
    m_menuBtn({ 290, 320, 220, 50 }, "MAIN MENU", { 160, 40, 90, 255 }, { 210, 60, 120, 255 }),
    m_playAgainBtn({ 285, 250, 230, 50 }, "PLAY AGAIN", { 40, 160, 100, 255 }, { 60, 210, 130, 255 }) {

    m_game->addObserver(this);
    m_game->setDifficulty(m_app.getDifficulty());
    m_game->start();

    const auto path = m_game->getPathPoints();
    if (!path.empty()) {
        m_vortexPosition = { path.back().x, path.back().y };
    }
}

GameplayScreen::~GameplayScreen() {
    if (m_game) {
        m_game->removeObserver(this);
    }
}

void GameplayScreen::onBoardUpdated() {
}

void GameplayScreen::onScoreChanged(int newScore) {
    m_displayedScore = newScore;
}

void GameplayScreen::onGameStateChanged(GameState newState) {
    m_currentState = newState;
}

void GameplayScreen::update(float deltaTime) {
    if (m_currentState == GameState::Playing) {
        if (m_pauseBtn.isClicked() || IsKeyPressed(KEY_P)) {
            m_game->pause();
            return;
        }

        Vector2 mouse = GetMousePosition();
        m_game->aimAt({ mouse.x, mouse.y });

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mouse.x < 670 || mouse.y > 55)) {
            m_game->shoot();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_SPACE)) {
            m_game->swapBalls();
        }

        m_game->update(deltaTime);
    }
    else if (m_currentState == GameState::Paused) {
        if (m_resumeBtn.isClicked()) m_game->resume();
        else if (m_menuBtn.isClicked()) m_app.switchToMainMenu();
    }
    else if (m_currentState == GameState::GameOver || m_currentState == GameState::Victory) {
        if (m_playAgainBtn.isClicked()) {
            m_game->start();
        }
        else if (m_menuBtn.isClicked()) {
            m_app.switchToMainMenu();
        }
    }
}

void GameplayScreen::renderTrack() const {
    const auto path = m_game->getPathPoints();
    if (path.size() < 2) return;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        Vector2 p1 = { path[i].x, path[i].y };
        Vector2 p2 = { path[i + 1].x, path[i + 1].y };
        DrawLineEx(p1, p2, 28.0f, UITheme::TRACK_OUTER);
        DrawLineEx(p1, p2, 6.0f, UITheme::TRACK_INNER);
    }

    DrawCircle(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), 26.0f, BLACK);
    DrawCircleLines(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), 26.0f, UITheme::NEON_PINK);

    float holePulse = (sinf((float)GetTime() * 6.0f) + 1.0f) * 4.0f + 12.0f;
    DrawCircleLines(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), holePulse, UITheme::NEON_CYAN);
}

void GameplayScreen::renderBalls() const {
    const auto& assets = m_app.getAssets();

    for (const auto& ball : m_game->getTrackBallsData()) {
        assets.drawBall({ ball.position.x, ball.position.y }, ball.radius, ball.color);
    }

    for (const auto& ball : m_game->getFiredBallsData()) {
        assets.drawBall({ ball.position.x, ball.position.y }, ball.radius, ball.color);
    }
}

void GameplayScreen::renderLauncher() const {
    const auto& assets = m_app.getAssets();
    LauncherData launcher = m_game->getLauncherData();
    Vector2 pos = { launcher.position.x, launcher.position.y };

    assets.drawTurntable(pos, launcher.angleRadians);
    assets.drawBall(pos, 16.0f, launcher.currentColor);
    assets.drawBall({ pos.x + 28.0f, pos.y + 28.0f }, 9.0f, launcher.nextColor);
}

void GameplayScreen::renderOverlay(const char* title, Color titleColor, bool isPaused) const {
    DrawRectangle(0, 0, UITheme::SCREEN_WIDTH, UITheme::SCREEN_HEIGHT, Fade(BLACK, 0.7f));
    int w = MeasureText(title, 50);
    DrawText(title, (UITheme::SCREEN_WIDTH - w) / 2, 140, 50, titleColor);

    if (!isPaused) {
        const char* scoreTxt = TextFormat("FINAL SCORE: %d", m_displayedScore);
        DrawText(scoreTxt, (UITheme::SCREEN_WIDTH - MeasureText(scoreTxt, 26)) / 2, 205, 26, WHITE);
        m_playAgainBtn.render();
    }
    else {
        m_resumeBtn.render();
    }
    m_menuBtn.render();
}

void GameplayScreen::render() const {
    renderTrack();
    renderBalls();
    renderLauncher();

    DrawText(TextFormat("SCORE: %d", m_displayedScore), 20, 15, 26, WHITE);

    if (m_currentState == GameState::Playing) {
        m_pauseBtn.render();
    }
    else if (m_currentState == GameState::Paused) {
        renderOverlay("PAUSED", WHITE, true);
    }
    else if (m_currentState == GameState::GameOver) {
        renderOverlay("GAME OVER", RED, false);
    }
    else if (m_currentState == GameState::Victory) {
        renderOverlay("VICTORY!", GOLD, false);
    }
}