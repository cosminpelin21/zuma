#pragma once
#include "raylib.h"
#include "Enums.h"

namespace UITheme {
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;

    constexpr Color BG_DARK = { 20, 10, 38, 255 };
    constexpr Color GRID_LINE = { 70, 30, 120, 80 };
    constexpr Color NEON_CYAN = { 0, 235, 255, 255 };
    constexpr Color NEON_PINK = { 255, 0, 170, 255 };
    constexpr Color TRACK_OUTER = { 45, 20, 80, 255 };
    constexpr Color TRACK_INNER = { 160, 80, 255, 180 };

    inline Color mapBallColor(BallColor color) {
        switch (color) {
        case BallColor::Red:    return { 255, 50, 70, 255 };
        case BallColor::Blue:   return { 0, 210, 255, 255 };
        case BallColor::Yellow: return { 255, 225, 40, 255 };
        case BallColor::Green:  return { 50, 235, 110, 255 };
        case BallColor::Purple: return { 180, 60, 255, 255 };
        case BallColor::Pink:   return { 255, 60, 190, 255 };
        default:                    return BLANK;
        }
    }
}