#include "GameplayScreen.h"
#include "GameApplication.h"
#include "UIConstants.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

GameplayScreen::GameplayScreen(GameApplication& app)
    : m_app(app),
      m_score(150),
      m_state(MockGameState::Playing),
      m_pauseBtn({ 675, 12, 110, 38 }, "PAUSE", { 90, 30, 150, 255 }, { 140, 50, 210, 255 }, 20),
      m_resumeBtn({ 290, 250, 220, 50 }, "RESUME", { 40, 160, 100, 255 }, { 60, 210, 130, 255 }),
      m_menuBtn({ 290, 320, 220, 50 }, "MAIN MENU", { 160, 40, 90, 255 }, { 210, 60, 120, 255 }),
      m_playAgainBtn({ 285, 250, 230, 50 }, "PLAY AGAIN", { 40, 160, 100, 255 }, { 60, 210, 130, 255 }),
      m_totalPathLength(0.0f),
      m_vortexPosition({ 280.0f, 420.0f }) {
    
    generateEllipticSpiralTrack();
    m_launcher = { { 450.0f, 300.0f }, 0.0f, MockBallColor::Red, MockBallColor::Blue };
    m_activeFiredBall.active = false;

    float ballDiameter = 30.0f;
    int ballCount = 18;
    for (int i = 0; i < ballCount; ++i) {
        MockBallColor c = static_cast<MockBallColor>(rand() % 5);
        m_trackBalls.push_back({ static_cast<float>(i * ballDiameter), c, 15.0f });
    }

    m_launcher.currentColor = getRandomAvailableColor();
    m_launcher.nextColor = getRandomAvailableColor();
}

void GameplayScreen::generateEllipticSpiralTrack() {
    m_detailedPath.clear();
    float cx = 400.0f;
    float cy = 300.0f;
    int steps = 500;
    for (int i = 0; i <= steps; ++i) {
        float t = (float)i / steps;
        float angle = t * 5.5f * 3.14159f;
        float rx = 320.0f * (1.0f - 0.7f * t);
        float ry = 220.0f * (1.0f - 0.7f * t);
        float x = cx + rx * cosf(angle);
        float y = cy + ry * sinf(angle);
        m_detailedPath.push_back({ x, y });
    }

    m_vortexPosition = m_detailedPath.back();

    m_pathSegmentLengths.clear();
    m_totalPathLength = 0.0f;
    for (size_t i = 0; i < m_detailedPath.size() - 1; ++i) {
        Vector2 p1 = m_detailedPath[i];
        Vector2 p2 = m_detailedPath[i + 1];
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dist = sqrtf(dx * dx + dy * dy);
        m_pathSegmentLengths.push_back(dist);
        m_totalPathLength += dist;
    }
}

Vector2 GameplayScreen::getPointAtDistance(float distance) const {
    if (m_detailedPath.empty()) return { 400.0f, 300.0f };
    if (distance <= 0.0f) return m_detailedPath.front();
    if (distance >= m_totalPathLength) return m_detailedPath.back();

    float accumulated = 0.0f;
    for (size_t i = 0; i < m_pathSegmentLengths.size(); ++i) {
        float segLen = m_pathSegmentLengths[i];
        if (accumulated + segLen >= distance) {
            float localDist = distance - accumulated;
            float t = localDist / segLen;
            Vector2 p1 = m_detailedPath[i];
            Vector2 p2 = m_detailedPath[i + 1];
            return {
                p1.x + (p2.x - p1.x) * t,
                p1.y + (p2.y - p1.y) * t
            };
        }
        accumulated += segLen;
    }
    return m_detailedPath.back();
}

MockBallColor GameplayScreen::getRandomAvailableColor() const {
    if (m_trackBalls.empty()) return MockBallColor::Red;
    std::vector<MockBallColor> availableColors;
    for (const auto& ball : m_trackBalls) {
        if (std::find(availableColors.begin(), availableColors.end(), ball.color) == availableColors.end()) {
            availableColors.push_back(ball.color);
        }
    }
    if (availableColors.empty()) return MockBallColor::Red;
    return availableColors[rand() % availableColors.size()];
}

void GameplayScreen::resolveMatches(size_t insertedIndex) {
    if (m_trackBalls.empty() || insertedIndex >= m_trackBalls.size()) return;

    MockBallColor col = m_trackBalls[insertedIndex].color;
    size_t left = insertedIndex;
    size_t right = insertedIndex;

    while (left > 0 && m_trackBalls[left - 1].color == col) {
        left--;
    }
    while (right < m_trackBalls.size() - 1 && m_trackBalls[right + 1].color == col) {
        right++;
    }

    int count = static_cast<int>(right - left + 1);
    if (count >= 3) {
        m_trackBalls.erase(m_trackBalls.begin() + left, m_trackBalls.begin() + right + 1);
        m_score += count * 50;

        float ballDiameter = 30.0f;
        for (size_t i = 0; i < m_trackBalls.size(); ++i) {
            if (i == 0) {
                if (m_trackBalls[0].distanceAlongPath < 0.0f) m_trackBalls[0].distanceAlongPath = 0.0f;
            } else {
                m_trackBalls[i].distanceAlongPath = m_trackBalls[i - 1].distanceAlongPath + ballDiameter;
            }
        }
    }
}

void GameplayScreen::update(float deltaTime) {
    if (m_state == MockGameState::Playing) {
        if (m_pauseBtn.isClicked()) {
            m_state = MockGameState::Paused;
            return;
        }

        if (m_trackBalls.empty()) {
            m_state = MockGameState::Victory;
            return;
        }

        float chainSpeed = 25.0f;
        for (auto& ball : m_trackBalls) {
            ball.distanceAlongPath += deltaTime * chainSpeed;
        }

        if (m_trackBalls.back().distanceAlongPath >= m_totalPathLength) {
            for (auto& ball : m_trackBalls) {
                ball.distanceAlongPath = m_totalPathLength;
            }
            m_state = MockGameState::GameOver;
            return;
        }

        Vector2 mouse = GetMousePosition();
        float dx = mouse.x - m_launcher.position.x;
        float dy = mouse.y - m_launcher.position.y;
        m_launcher.angleRadians = atan2f(dy, dx);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mouse.x < 670 || mouse.y > 55)) {
            if (!m_activeFiredBall.active) {
                m_activeFiredBall.active = true;
                m_activeFiredBall.position = m_launcher.position;
                m_activeFiredBall.color = m_launcher.currentColor;
                m_activeFiredBall.radius = 15.0f;

                float speed = 650.0f;
                m_activeFiredBall.velocity = {
                    cosf(m_launcher.angleRadians) * speed,
                    sinf(m_launcher.angleRadians) * speed
                };

                m_launcher.currentColor = m_launcher.nextColor;
                m_launcher.nextColor = getRandomAvailableColor();
            }
        }

        if (m_activeFiredBall.active) {
            m_activeFiredBall.position.x += m_activeFiredBall.velocity.x * deltaTime;
            m_activeFiredBall.position.y += m_activeFiredBall.velocity.y * deltaTime;

            if (m_activeFiredBall.position.x < 0 || m_activeFiredBall.position.x > UITheme::SCREEN_WIDTH ||
                m_activeFiredBall.position.y < 0 || m_activeFiredBall.position.y > UITheme::SCREEN_HEIGHT) {
                m_activeFiredBall.active = false;
            } else {
                for (size_t i = 0; i < m_trackBalls.size(); ++i) {
                    Vector2 ballPos = getPointAtDistance(m_trackBalls[i].distanceAlongPath);
                    float dxx = m_activeFiredBall.position.x - ballPos.x;
                    float dyy = m_activeFiredBall.position.y - ballPos.y;
                    float dist = sqrtf(dxx * dxx + dyy * dyy);

                    if (dist < (m_activeFiredBall.radius + m_trackBalls[i].radius + 5.0f)) {
                        m_activeFiredBall.active = false;

                        ChainBall newBall;
                        newBall.color = m_activeFiredBall.color;
                        newBall.radius = 15.0f;
                        newBall.distanceAlongPath = m_trackBalls[i].distanceAlongPath;

                        m_trackBalls.insert(m_trackBalls.begin() + i, newBall);

                        float ballDiameter = 30.0f;
                        for (size_t j = i + 1; j < m_trackBalls.size(); ++j) {
                            m_trackBalls[j].distanceAlongPath = m_trackBalls[j - 1].distanceAlongPath + ballDiameter;
                        }

                        resolveMatches(i);
                        break;
                    }
                }
            }
        }

        if (IsKeyPressed(KEY_P)) {
            m_state = MockGameState::Paused;
        }
    } 
    else if (m_state == MockGameState::Paused) {
        if (m_resumeBtn.isClicked()) m_state = MockGameState::Playing;
        else if (m_menuBtn.isClicked()) m_app.switchToMainMenu();
    } 
    else if (m_state == MockGameState::GameOver || m_state == MockGameState::Victory) {
        if (m_playAgainBtn.isClicked()) {
            m_state = MockGameState::Playing;
            m_score = 150;
            m_trackBalls.clear();
            float ballDiameter = 30.0f;
            int ballCount = 18;
            for (int i = 0; i < ballCount; ++i) {
                MockBallColor c = static_cast<MockBallColor>(rand() % 5);
                m_trackBalls.push_back({ static_cast<float>(i * ballDiameter), c, 15.0f });
            }
            m_launcher.currentColor = getRandomAvailableColor();
            m_launcher.nextColor = getRandomAvailableColor();
        } else if (m_menuBtn.isClicked()) {
            m_app.switchToMainMenu();
        }
    }
}

void GameplayScreen::renderTrack() const {
    if (m_detailedPath.size() < 2) return;

    for (size_t i = 0; i < m_detailedPath.size() - 1; ++i) {
        DrawLineEx(m_detailedPath[i], m_detailedPath[i + 1], 28.0f, UITheme::TRACK_OUTER);
        DrawLineEx(m_detailedPath[i], m_detailedPath[i + 1], 6.0f, UITheme::TRACK_INNER);
    }

    DrawCircle(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), 26.0f, BLACK);
    DrawCircleLines(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), 26.0f, UITheme::NEON_PINK);
    
    float holePulse = (sinf((float)GetTime() * 6.0f) + 1.0f) * 4.0f + 12.0f;
    DrawCircleLines(static_cast<int>(m_vortexPosition.x), static_cast<int>(m_vortexPosition.y), holePulse, UITheme::NEON_CYAN);
}

void GameplayScreen::renderBalls() const {
    const auto& assets = m_app.getAssets();
    for (const auto& ball : m_trackBalls) {
        Vector2 pos = getPointAtDistance(ball.distanceAlongPath);
        assets.drawBall(pos, ball.radius, ball.color);
    }

    if (m_activeFiredBall.active) {
        assets.drawBall(m_activeFiredBall.position, m_activeFiredBall.radius, m_activeFiredBall.color);
    }
}

void GameplayScreen::renderLauncher() const {
    const auto& assets = m_app.getAssets();
    assets.drawTurntable(m_launcher.position, m_launcher.angleRadians);
    assets.drawBall(m_launcher.position, 16.0f, m_launcher.currentColor);
    assets.drawBall({ m_launcher.position.x + 28.0f, m_launcher.position.y + 28.0f }, 9.0f, m_launcher.nextColor);
}

void GameplayScreen::renderOverlay(const char* title, Color titleColor, bool isPaused) const {
    DrawRectangle(0, 0, UITheme::SCREEN_WIDTH, UITheme::SCREEN_HEIGHT, Fade(BLACK, 0.7f));
    int w = MeasureText(title, 50);
    DrawText(title, (UITheme::SCREEN_WIDTH - w) / 2, 140, 50, titleColor);

    if (!isPaused) {
        const char* scoreTxt = TextFormat("FINAL SCORE: %d", m_score);
        DrawText(scoreTxt, (UITheme::SCREEN_WIDTH - MeasureText(scoreTxt, 26)) / 2, 205, 26, WHITE);
        m_playAgainBtn.render();
    } else {
        m_resumeBtn.render();
    }
    m_menuBtn.render();
}

void GameplayScreen::render() const {
    renderTrack();
    renderBalls();
    renderLauncher();

    DrawText(TextFormat("SCORE: %d", m_score), 20, 15, 26, WHITE);

    if (m_state == MockGameState::Playing) {
        m_pauseBtn.render();
    } else if (m_state == MockGameState::Paused) {
        renderOverlay("PAUSED", WHITE, true);
    } else if (m_state == MockGameState::GameOver) {
        renderOverlay("GAME OVER", RED, false);
    } else if (m_state == MockGameState::Victory) {
        renderOverlay("VICTORY!", GOLD, false);
    }
}