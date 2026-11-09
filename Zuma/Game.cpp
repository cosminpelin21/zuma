#include "Game.h"

Game::Game()
    : m_state(GameState::Ready), m_score(0), m_nextBallId(1) 
{}

void Game::start() {
    m_state = GameState::Playing;
    m_score = 0;
    m_balls.clear();

    spawnBall({ 0.0f, 200.0f }, BallColor::Red);
    spawnBall({ 35.0f, 200.0f }, BallColor::Blue);
    spawnBall({ 70.0f, 200.0f }, BallColor::Yellow);
}

void Game::pause() 
{
    if (m_state == GameState::Playing) {
        m_state = GameState::Paused;
    }
}

void Game::resume() 
{
    if (m_state == GameState::Paused) {
        m_state = GameState::Playing;
    }
}

void Game::spawnBall(Point2D position, BallColor color) 
{
    m_balls.emplace_back(m_nextBallId++, position, color);
}

void Game::update(float deltaTime) 
{
    if (m_state != GameState::Playing) {
        return;
    }

    float speed = 60.0f;
    for (auto& ball : m_balls) {
        ball.move(speed * deltaTime, 0.0f);
    }
}

std::vector<BallData> Game::getBallsData() const 
{
    std::vector<BallData> result;
    result.reserve(m_balls.size());
    for (const auto& ball : m_balls) {
        result.push_back({ ball.getId(), ball.getPosition(), ball.getColor() });
    }
    return result;
}