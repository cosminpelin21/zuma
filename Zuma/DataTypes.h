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
};