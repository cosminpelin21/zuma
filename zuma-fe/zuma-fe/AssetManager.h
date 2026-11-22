#pragma once
#include "raylib.h"
#include "UIConstants.h"
#include <map>
#include "Enums.h"

class AssetManager {
private:
    Texture2D m_turntableBase;
    bool m_hasTurntable;
    Texture2D m_logoTexture;
    bool m_hasLogo;
    std::map<BallColor, Texture2D> m_ballTextures;
    std::map<BallColor, bool> m_hasBallTexture;

public:
    AssetManager();
    ~AssetManager();

    void loadAll();
    void unloadAll();

    void drawBall(Vector2 pos, float radius, BallColor color) const;
    void drawTurntable(Vector2 pos, float angleRadians) const;
    const Texture2D& getLogo() const { return m_logoTexture; }
    bool hasLogo() const { return m_hasLogo; }
};