#include <gtest/gtest.h>
#include "Ball.h"
#include "TestHelpers.h"

TEST(BallTest, ConstructorStoresValues) {
    Ball b(7, { 10.0f, 20.0f }, BallColor::Red, 12.0f, 5.0f);

    EXPECT_EQ(b.getId(), 7);
    expectPointNear(b.getPosition(), 10.0f, 20.0f);
    EXPECT_EQ(b.getColor(), BallColor::Red);
    EXPECT_FLOAT_EQ(b.getRadius(), 12.0f);
    EXPECT_FLOAT_EQ(b.getDistance(), 5.0f);
}

TEST(BallTest, DefaultsAreApplied) {
    Ball b(1, { 0.0f, 0.0f }, BallColor::Blue);

    EXPECT_FLOAT_EQ(b.getRadius(), 15.0f);
    EXPECT_FLOAT_EQ(b.getDistance(), 0.0f);
    expectPointNear(b.getVelocity(), 0.0f, 0.0f);
}

TEST(BallTest, SettersUpdateValues) {
    Ball b(1, { 0.0f, 0.0f }, BallColor::Green);

    b.setPosition({ 3.0f, 4.0f });
    b.setDistance(99.0f);
    b.setVelocity({ 1.0f, -2.0f });

    expectPointNear(b.getPosition(), 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(b.getDistance(), 99.0f);
    expectPointNear(b.getVelocity(), 1.0f, -2.0f);
}

TEST(BallTest, MoveAddsOffsetToPosition) {
    Ball b(1, { 10.0f, 10.0f }, BallColor::Yellow);

    b.move(5.0f, -3.0f);

    expectPointNear(b.getPosition(), 15.0f, 7.0f);
}

TEST(BallTest, ToDataCopiesFields) {
    Ball b(42, { 1.0f, 2.0f }, BallColor::Purple, 20.0f);

    BallData d = b.toData();

    EXPECT_EQ(d.id, 42);
    expectPointNear(d.position, 1.0f, 2.0f);
    EXPECT_EQ(d.color, BallColor::Purple);
    EXPECT_FLOAT_EQ(d.radius, 20.0f);
}