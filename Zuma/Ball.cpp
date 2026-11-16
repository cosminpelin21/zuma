#include "Ball.h"

Ball::Ball(int id, Point2D position, BallColor color, float radius, float distance)
    : m_id(id), m_position(position), m_color(color), m_radius(radius),
    m_distanceAlongPath(distance), m_velocity{ 0.0f, 0.0f } {
}

void Ball::setPosition(Point2D position) {
    m_position = position;
}

void Ball::setDistance(float distance) {
    m_distanceAlongPath = distance;
}

void Ball::setVelocity(Point2D velocity) {
    m_velocity = velocity;
}

void Ball::move(float dx, float dy) {
    m_position.x += dx;
    m_position.y += dy;
}

BallData Ball::toData() const {
    return { m_id, m_position, m_color, m_radius };
}