#pragma once
#include "IScreen.h"
#include "Button.h"
#include "AssetManager.h"
#include <vector>

class GameApplication;

struct ChainBall {
    float distanceAlongPath;
    MockBallColor color;
    float radius;
};

struct FiredBall {
    Vector2 position;
    Vector2 velocity;
    MockBallColor color;
    float radius;
    bool active;
};

class GameplayScreen : public IScreen {
private:
    GameApplication& m_app;
    int m_score;
    MockGameState m_state;

    Button m_pauseBtn;
    Button m_resumeBtn;
    Button m_menuBtn;
    Button m_playAgainBtn;

    std::vector<Vector2> m_detailedPath;
    std::vector<float> m_pathSegmentLengths;
    float m_totalPathLength;

    std::vector<ChainBall> m_trackBalls;
    FiredBall m_activeFiredBall;
    MockLauncherData m_launcher;
    Vector2 m_vortexPosition;

    void generateEllipticSpiralTrack();
    Vector2 getPointAtDistance(float distance) const;
    void renderTrack() const;
    void renderBalls() const;
    void renderLauncher() const;
    void renderOverlay(const char* title, Color titleColor, bool isPaused) const;
    void resolveMatches(size_t insertedIndex);
    MockBallColor getRandomAvailableColor() const;

public:
    explicit GameplayScreen(GameApplication& app);
    void update(float deltaTime) override;
    void render() const override;
};