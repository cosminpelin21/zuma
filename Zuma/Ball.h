#pragma once
#include "DataTypes.h"

class Ball {
private:
    int m_id;
    Point2D m_position;
    BallColor m_color;
    float m_radius;
    float m_distanceAlongPath;
    Point2D m_velocity;       

public:
    Ball(int id, Point2D position, BallColor color, float radius = 15.0f, float distance = 0.0f);

    int getId() const { return m_id; }
    Point2D getPosition() const { return m_position; }
    BallColor getColor() const { return m_color; }
    float getRadius() const { return m_radius; }
    float getDistance() const { return m_distanceAlongPath; }
    Point2D getVelocity() const { return m_velocity; }

    void setPosition(Point2D position);
    void setDistance(float distance);
    void setVelocity(Point2D velocity);
    void move(float dx, float dy);

    BallData toData() const;
};