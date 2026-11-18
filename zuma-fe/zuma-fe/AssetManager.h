#pragma once
#include "raylib.h"
#include "UIConstants.h"
#include <map>

struct MockBallData {
    Vector2 position;
    float radius;
    MockBallColor color;
};

struct MockLauncherData {
    Vector2 position;
    float angleRadians;
    MockBallColor currentColor;
    MockBallColor nextColor;
};

class AssetManager {
private:
    Texture2D m_turntableBase;
    bool m_hasTurntable;
    Texture2D m_logoTexture;
    bool m_hasLogo;
    std::map<MockBallColor, Texture2D> m_ballTextures;
    std::map<MockBallColor, bool> m_hasBallTexture;

public:
    AssetManager();
    ~AssetManager();

    void loadAll();
    void unloadAll();

    void drawBall(Vector2 pos, float radius, MockBallColor color) const;
    void drawTurntable(Vector2 pos, float angleRadians) const;
    const Texture2D& getLogo() const { return m_logoTexture; }
    bool hasLogo() const { return m_hasLogo; }
};