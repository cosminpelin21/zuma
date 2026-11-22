#pragma once

enum class BallColor {
    Red,
    Blue,
    Yellow,
    Green,
    Purple,
    Pink,
    None
};

enum class GameState {
    Ready,
    Playing,
    Paused,
    GameOver,
    Victory
};

enum class Difficulty 
{ Easy, Medium, Hard };