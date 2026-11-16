#include "AssetManager.h"
#include <cmath>
#include <string>

AssetManager::AssetManager() : m_hasTurntable(false) {}

AssetManager::~AssetManager() {
    unloadAll();
}

void AssetManager::loadAll() {
    std::string basePath = "";
    std::string candidatePaths[] = {
        "assets/",
        "../assets/",
        "../../assets/",
        "../../../assets/"
    };

    for (const auto& path : candidatePaths) {
        if (FileExists((path + "ball_red.jpg").c_str()) || FileExists((path + "ball_red.png").c_str())) {
            basePath = path;
            break;
        }
    }

    struct Mapping {
        MockBallColor color;
        std::string baseName;
    };

    Mapping maps[] = {
        { MockBallColor::Red, "ball_red" },
        { MockBallColor::Blue, "ball_blue" },
        { MockBallColor::Yellow, "ball_yellow" },
        { MockBallColor::Green, "ball_green" },
        { MockBallColor::Purple, "ball_purple" },
        { MockBallColor::Pink, "ball_pink" }
    };

    for (const auto& m : maps) {
        std::string pathJpg = basePath + m.baseName + ".jpg";
        std::string pathPng = basePath + m.baseName + ".png";

        if (FileExists(pathJpg.c_str())) {
            m_ballTextures[m.color] = LoadTexture(pathJpg.c_str());
            m_hasBallTexture[m.color] = true;
        }
        else if (FileExists(pathPng.c_str())) {
            m_ballTextures[m.color] = LoadTexture(pathPng.c_str());
            m_hasBallTexture[m.color] = true;
        }
    }
}

void AssetManager::unloadAll() {
    for (auto& pair : m_ballTextures) {
        if (m_hasBallTexture[pair.first]) {
            UnloadTexture(pair.second);
        }
    }
    m_ballTextures.clear();
    m_hasBallTexture.clear();
}

void AssetManager::drawBall(Vector2 pos, float radius, MockBallColor color) const {
    if (color == MockBallColor::None) return;

    auto itTex = m_ballTextures.find(color);
    auto itHas = m_hasBallTexture.find(color);

    if (itTex != m_ballTextures.end() && itHas != m_hasBallTexture.end() && itHas->second) {
        Texture2D tex = itTex->second;
        Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
        Rectangle dst = { pos.x, pos.y, radius * 2.0f, radius * 2.0f };
        DrawTexturePro(tex, src, dst, { radius, radius }, 0.0f, WHITE);
    }
    else {
        Color tint = UITheme::mapBallColor(color);
        DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), radius, tint);
        DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), radius, WHITE);
    }
}

void AssetManager::drawTurntable(Vector2 pos, float angleRadians) const {
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 42.0f, { 40, 25, 70, 255 });
    DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), 42.0f, UITheme::NEON_CYAN);
    DrawLineEx(pos, { pos.x + cosf(angleRadians) * 48.0f, pos.y + sinf(angleRadians) * 48.0f }, 5.0f, { 255, 60, 180, 255 });
}