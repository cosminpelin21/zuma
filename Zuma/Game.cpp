#include "Game.h"
#include <cmath>
#include <algorithm>

namespace {
    constexpr float BALL_RADIUS = 15.0f;
    constexpr float BALL_DIAMETER = BALL_RADIUS * 2.0f;
    constexpr float TRACK_SPEED = 45.0f;
    constexpr float SHOT_SPEED = 550.0f;
    constexpr int INITIAL_BALL_COUNT = 15;
    constexpr int POINTS_PER_BALL = 10;

    float distanceBetween(Point2D a, Point2D b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }
}

std::unique_ptr<IGame> createGame() {
    return std::make_unique<Game>();
}

Game::Game()
    : m_state(GameState::Ready),
    m_score(0),
    m_nextBallId(1),
    m_totalPathLength(0.0f),
    m_rng(std::random_device{}()) {
    m_launcher.position = { 400.0f, 300.0f };
    initDefaultPath();
}

void Game::initDefaultPath() {
    // Traseu simplu in forma de spirala rectangulara (fereastra 800x600)
    m_path = {
        { 50.0f,  50.0f },
        { 750.0f, 50.0f },
        { 750.0f, 550.0f },
        { 50.0f,  550.0f },
        { 50.0f,  150.0f },
        { 650.0f, 150.0f },
        { 650.0f, 450.0f },
        { 180.0f, 450.0f },
        { 180.0f, 300.0f } // Punctul final (craniul / gaura)
    };

    m_totalPathLength = 0.0f;
    for (size_t i = 1; i < m_path.size(); ++i) {
        m_totalPathLength += distanceBetween(m_path[i - 1], m_path[i]);
    }
}

Point2D Game::getPointAtDistance(float distance) const {
    if (m_path.empty()) return { 0.0f, 0.0f };
    if (distance <= 0.0f) return m_path.front();

    float accumulated = 0.0f;
    for (size_t i = 1; i < m_path.size(); ++i) {
        float segLen = distanceBetween(m_path[i - 1], m_path[i]);
        if (accumulated + segLen >= distance) {
            float t = (segLen > 0.0f) ? (distance - accumulated) / segLen : 0.0f;
            return {
                m_path[i - 1].x + t * (m_path[i].x - m_path[i - 1].x),
                m_path[i - 1].y + t * (m_path[i].y - m_path[i - 1].y)
            };
        }
        accumulated += segLen;
    }
    return m_path.back();
}

BallColor Game::generateRandomColor() {
    std::uniform_int_distribution<int> dist(0, 3); // 4 culori de baza pentru match-uri mai usoare
    return static_cast<BallColor>(dist(m_rng));
}

void Game::start() {
    m_score = 0;
    m_nextBallId = 1;
    m_trackBalls.clear();
    m_firedBalls.clear();

    // Generam sirul initial de bile (index 0 este prima bila, cea mai avansata pe traseu)
    for (int i = 0; i < INITIAL_BALL_COUNT; ++i) {
        float dist = static_cast<float>(INITIAL_BALL_COUNT - 1 - i) * BALL_DIAMETER;
        BallColor color = generateRandomColor();
        m_trackBalls.emplace_back(m_nextBallId++, getPointAtDistance(dist), color, BALL_RADIUS, dist);
    }

    m_launcher.currentColor = generateRandomColor();
    m_launcher.nextColor = generateRandomColor();

    setState(GameState::Playing);
    notifyScoreChanged();
    notifyBoardUpdated();
}

void Game::pause() {
    if (m_state == GameState::Playing) {
        setState(GameState::Paused);
    }
}

void Game::resume() {
    if (m_state == GameState::Paused) {
        setState(GameState::Playing);
    }
}

void Game::aimAt(Point2D target) {
    float dx = target.x - m_launcher.position.x;
    float dy = target.y - m_launcher.position.y;
    m_launcher.angleRadians = std::atan2(dy, dx);
}

void Game::shoot() {
    if (m_state != GameState::Playing || m_launcher.currentColor == BallColor::None) {
        return;
    }

    Ball projectile(m_nextBallId++, m_launcher.position, m_launcher.currentColor, BALL_RADIUS);
    projectile.setVelocity({
        std::cos(m_launcher.angleRadians) * SHOT_SPEED,
        std::sin(m_launcher.angleRadians) * SHOT_SPEED
        });
    m_firedBalls.push_back(projectile);

    m_launcher.currentColor = m_launcher.nextColor;
    m_launcher.nextColor = generateRandomColor();
}

void Game::swapBalls() {
    if (m_state == GameState::Playing) {
        std::swap(m_launcher.currentColor, m_launcher.nextColor);
        notifyBoardUpdated();
    }
}

void Game::update(float deltaTime) {
    if (m_state != GameState::Playing) {
        return;
    }

    updateTrackBalls(deltaTime);
    updateFiredBalls(deltaTime);
    checkCollisions();

    if (m_trackBalls.empty() && m_state == GameState::Playing) {
        setState(GameState::Victory);
    }

    notifyBoardUpdated();
}

void Game::updateTrackBalls(float deltaTime) {
    if (m_trackBalls.empty()) return;

    // Ultima bila (coada) impinge tot sirul in fata
    float backDist = m_trackBalls.back().getDistance() + TRACK_SPEED * deltaTime;
    m_trackBalls.back().setDistance(backDist);
    m_trackBalls.back().setPosition(getPointAtDistance(backDist));

    for (int i = static_cast<int>(m_trackBalls.size()) - 2; i >= 0; --i) {
        float minAllowedDist = m_trackBalls[i + 1].getDistance() + BALL_DIAMETER;
        float currentDist = std::max(m_trackBalls[i].getDistance(), minAllowedDist);
        m_trackBalls[i].setDistance(currentDist);
        m_trackBalls[i].setPosition(getPointAtDistance(currentDist));
    }

    if (m_trackBalls.front().getDistance() >= m_totalPathLength) {
        setState(GameState::GameOver);
    }
}

void Game::updateFiredBalls(float deltaTime) {
    for (auto& ball : m_firedBalls) {
        Point2D vel = ball.getVelocity();
        ball.move(vel.x * deltaTime, vel.y * deltaTime);
    }

    // Eliminam bilele iesite mult in afara ecranului
    m_firedBalls.erase(
        std::remove_if(m_firedBalls.begin(), m_firedBalls.end(), [](const Ball& b) {
            Point2D p = b.getPosition();
            return p.x < -100.0f || p.x > 900.0f || p.y < -100.0f || p.y > 700.0f;
            }),
        m_firedBalls.end()
    );
}

void Game::checkCollisions() {
    for (auto it = m_firedBalls.begin(); it != m_firedBalls.end(); ) {
        bool hit = false;
        for (size_t i = 0; i < m_trackBalls.size(); ++i) {
            if (distanceBetween(it->getPosition(), m_trackBalls[i].getPosition()) < BALL_DIAMETER * 0.9f) {
                Ball hitBall = *it;
                it = m_firedBalls.erase(it);
                insertBallIntoTrack(i, hitBall);
                hit = true;
                break;
            }
        }
        if (!hit) {
            ++it;
        }
    }
}

void Game::insertBallIntoTrack(size_t index, const Ball& firedBall) {
    float insertDist = m_trackBalls[index].getDistance();
    Ball newBall(firedBall.getId(), getPointAtDistance(insertDist), firedBall.getColor(), BALL_RADIUS, insertDist);

    m_trackBalls.insert(m_trackBalls.begin() + index, newBall);

    // Impingem bilele din fata (de la index-1 spre 0) ca sa faca loc bilei inserate
    for (int i = static_cast<int>(index) - 1; i >= 0; --i) {
        float neededDist = m_trackBalls[i + 1].getDistance() + BALL_DIAMETER;
        if (m_trackBalls[i].getDistance() < neededDist) {
            m_trackBalls[i].setDistance(neededDist);
            m_trackBalls[i].setPosition(getPointAtDistance(neededDist));
        }
    }

    checkAndRemoveMatches(index);
}

void Game::checkAndRemoveMatches(size_t insertedIndex) {
    if (insertedIndex >= m_trackBalls.size()) return;

    BallColor targetColor = m_trackBalls[insertedIndex].getColor();
    int left = static_cast<int>(insertedIndex);
    int right = static_cast<int>(insertedIndex);

    while (left - 1 >= 0 && m_trackBalls[left - 1].getColor() == targetColor) {
        --left;
    }
    while (right + 1 < static_cast<int>(m_trackBalls.size()) && m_trackBalls[right + 1].getColor() == targetColor) {
        ++right;
    }

    int count = right - left + 1;
    if (count >= 3) {
        m_trackBalls.erase(m_trackBalls.begin() + left, m_trackBalls.begin() + right + 1);
        addScore(count * POINTS_PER_BALL);
    }
}

std::vector<BallData> Game::getTrackBallsData() const {
    std::vector<BallData> result;
    result.reserve(m_trackBalls.size());
    for (const auto& ball : m_trackBalls) {
        result.push_back(ball.toData());
    }
    return result;
}

std::vector<BallData> Game::getFiredBallsData() const {
    std::vector<BallData> result;
    result.reserve(m_firedBalls.size());
    for (const auto& ball : m_firedBalls) {
        result.push_back(ball.toData());
    }
    return result;
}

void Game::addObserver(IGameObserver* observer) {
    if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end()) {
        m_observers.push_back(observer);
    }
}

void Game::removeObserver(IGameObserver* observer) {
    m_observers.erase(
        std::remove(m_observers.begin(), m_observers.end(), observer),
        m_observers.end()
    );
}

void Game::setState(GameState newState) {
    if (m_state != newState) {
        m_state = newState;
        notifyStateChanged();
    }
}

void Game::addScore(int points) {
    if (points > 0) {
        m_score += points;
        notifyScoreChanged();
    }
}

void Game::notifyBoardUpdated() {
    for (auto* obs : m_observers) {
        obs->onBoardUpdated();
    }
}

void Game::notifyScoreChanged() {
    for (auto* obs : m_observers) {
        obs->onScoreChanged(m_score);
    }
}

void Game::notifyStateChanged() {
    for (auto* obs : m_observers) {
        obs->onGameStateChanged(m_state);
    }
}