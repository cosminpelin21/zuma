#pragma once
#include "Enums.h"

struct Point2D {
    float x = 0.0f;
    float y = 0.0f;
};

struct BallData {
    int id = 0;
    Point2D position;
    BallColor color = BallColor::None;
    float radius = 15.0f;
};

struct LauncherData {
    Point2D position{ 400.0f, 300.0f };
    float angleRadians = 0.0f;
    BallColor currentColor = BallColor::None;
    BallColor nextColor = BallColor::None;
};