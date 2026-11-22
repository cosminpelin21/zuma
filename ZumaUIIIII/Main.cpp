#define NOMINMAX
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "IGame.h"
#include "IGameObserver.h"

constexpr int GRID_W = 80;
constexpr int GRID_H = 25;
constexpr float WORLD_W = 800.0f;
constexpr float WORLD_H = 500.0f;

class ConsoleZumaUI : public IGameObserver {
private:
    HANDLE m_hOut;
    HANDLE m_hIn;
    int m_score = 0;
    GameState m_state = GameState::Ready;

    WORD getWinColor(BallColor color) const {
        switch (color) {
        case BallColor::Red:    return FOREGROUND_RED | FOREGROUND_INTENSITY;
        case BallColor::Blue:   return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case BallColor::Yellow: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case BallColor::Green:  return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case BallColor::Purple: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case BallColor::Pink:   return FOREGROUND_RED | FOREGROUND_BLUE;
        default:                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }

    int toGridX(float worldX) const {
        return std::clamp(static_cast<int>((worldX / WORLD_W) * GRID_W), 0, GRID_W - 1);
    }

    int toGridY(float worldY) const {
        return std::clamp(static_cast<int>((worldY / WORLD_H) * GRID_H), 2, GRID_H - 2);
    }

    void putChar(std::vector<CHAR_INFO>& buffer, int x, int y, char ch, WORD attr) const {
        if (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H) {
            buffer[y * GRID_W + x].Char.AsciiChar = ch;
            buffer[y * GRID_W + x].Attributes = attr;
        }
    }

    void putText(std::vector<CHAR_INFO>& buffer, int x, int y, const std::string& text, WORD attr) const {
        for (size_t i = 0; i < text.size(); ++i) {
            putChar(buffer, x + static_cast<int>(i), y, text[i], attr);
        }
    }

public:
    ConsoleZumaUI() {
        m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        m_hIn = GetStdHandle(STD_INPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo{};
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(m_hOut, &cursorInfo);

        DWORD mode = 0;
        GetConsoleMode(m_hIn, &mode);
        mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        SetConsoleMode(m_hIn, mode);
    }

    void onBoardUpdated() override {}
    void onScoreChanged(int newScore) override { m_score = newScore; }
    void onGameStateChanged(GameState newState) override { m_state = newState; }

    void render(const IGame& game) const {
        WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        std::vector<CHAR_INFO> buffer(GRID_W * GRID_H);

        for (auto& cell : buffer) {
            cell.Char.AsciiChar = ' ';
            cell.Attributes = gray;
        }

        putText(buffer, 1, 0, "ZUMA | Scor: " + std::to_string(m_score) +
            " | Click: Trage | Space: Schimba | R: Restart | ESC: Iesire", white);

        const auto path = game.getPathPoints();
        if (path.size() >= 2) {
            int yLine = toGridY(path[0].y);
            int xStart = toGridX(path.front().x);
            int xEnd = toGridX(path.back().x);
            for (int x = xStart; x <= xEnd; ++x) {
                putChar(buffer, x, yLine, '-', FOREGROUND_INTENSITY);
            }
            putChar(buffer, xEnd, yLine, 'X', FOREGROUND_RED | FOREGROUND_INTENSITY);
        }

        for (const auto& ball : game.getTrackBallsData()) {
            putChar(buffer, toGridX(ball.position.x), toGridY(ball.position.y), '0', getWinColor(ball.color));
        }

        for (const auto& ball : game.getFiredBallsData()) {
            putChar(buffer, toGridX(ball.position.x), toGridY(ball.position.y), '0', getWinColor(ball.color));
        }

        LauncherData launcher = game.getLauncherData();
        int lx = toGridX(launcher.position.x);
        int ly = toGridY(launcher.position.y);

        for (int step = 1; step <= 4; ++step) {
            int ax = lx + static_cast<int>(std::round(std::cos(launcher.angleRadians) * step * 2.0f));
            int ay = ly + static_cast<int>(std::round(std::sin(launcher.angleRadians) * step));
            putChar(buffer, ax, ay, '.', gray);
        }

        putChar(buffer, lx - 1, ly, '[', white);
        putChar(buffer, lx, ly, '0', getWinColor(launcher.currentColor));
        putChar(buffer, lx + 1, ly, ']', white);
        putText(buffer, lx + 3, ly, "Next:", gray);
        putChar(buffer, lx + 8, ly, '0', getWinColor(launcher.nextColor));

        if (m_state == GameState::GameOver) {
            putText(buffer, 25, 12, "=== GAME OVER! Apasa R pentru restart ===", FOREGROUND_RED | FOREGROUND_INTENSITY);
        }
        else if (m_state == GameState::Victory) {
            putText(buffer, 26, 12, "=== VICTORIE! Apasa R pentru restart ===", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        }

        SMALL_RECT rect{ 0, 0, GRID_W - 1, GRID_H - 1 };
        WriteConsoleOutputA(m_hOut, buffer.data(), { GRID_W, GRID_H }, { 0, 0 }, &rect);
    }

    bool handleInput(IGame& game) {
        DWORD numEvents = 0;
        GetNumberOfConsoleInputEvents(m_hIn, &numEvents);
        if (numEvents == 0) return true;

        std::vector<INPUT_RECORD> events(numEvents);
        DWORD readCount = 0;
        ReadConsoleInput(m_hIn, events.data(), numEvents, &readCount);

        for (DWORD i = 0; i < readCount; ++i) {
            if (events[i].EventType == MOUSE_EVENT) {
                const auto& me = events[i].Event.MouseEvent;
                Point2D worldTarget{
                    (static_cast<float>(me.dwMousePosition.X) / GRID_W) * WORLD_W,
                    (static_cast<float>(me.dwMousePosition.Y) / GRID_H) * WORLD_H
                };
                game.aimAt(worldTarget);

                if (me.dwEventFlags == 0 && (me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
                    game.shoot();
                }
            }
            else if (events[i].EventType == KEY_EVENT && events[i].Event.KeyEvent.bKeyDown) {
                WORD vk = events[i].Event.KeyEvent.wVirtualKeyCode;
                if (vk == VK_ESCAPE) return false;
                if (vk == VK_SPACE)  game.swapBalls();
                if (vk == 'R')       game.start();
            }
        }
        return true;
    }
};

int main() {
    auto game = createGame();
    ConsoleZumaUI ui;
    game->addObserver(&ui);
    game->start();

    const float dt = 0.033f;
    bool running = true;

    while (running) {
        running = ui.handleInput(*game);
        game->update(dt);
        ui.render(*game);
        Sleep(33);
    }

    game->removeObserver(&ui);
    return 0;
}