#pragma once
#include "DataTypes.h"

class Ball {
private:
    int m_id;
    Point2D m_position;
    BallColor m_color;
    float m_radius;

public:
    Ball(int id, Point2D position, BallColor color, float radius = 15.0f);

    int getId() const 
    { 
        return m_id;
    }

    Point2D getPosition() const 
    { 
        return m_position; 
    }

    BallColor getColor() const 
    { 
        return m_color; 
    }

    float getRadius() const 
    { 
        return m_radius; 
    }

    void setPosition(Point2D position);
    void move(float dx, float dy);
};