#pragma once

enum class BallColor {
    Red,
    Blue,
    Yellow,
    Green,
    Purple,
    None
};

enum class GameState {
    Ready,
    Playing,
    Paused,
    GameOver,
    Victory
};