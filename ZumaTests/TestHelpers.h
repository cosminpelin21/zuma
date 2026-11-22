#pragma once
#include <gtest/gtest.h>
#include "DataTypes.h"

// Compara un punct cu coordonate asteptate, cu toleranta (pentru float)
inline void expectPointNear(Point2D p, float x, float y, float eps = 0.01f) {
    EXPECT_NEAR(p.x, x, eps);
    EXPECT_NEAR(p.y, y, eps);
}