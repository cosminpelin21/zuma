#include "Ball.h"

Ball::Ball(int id, Point2D position, BallColor color, float radius)
    : m_id(id), m_position(position), m_color(color), m_radius(radius) 
{}

void Ball::setPosition(Point2D position) 
{
    m_position = position;
}

void Ball::move(float dx, float dy) 
{
    m_position.x += dx;
    m_position.y += dy;
}