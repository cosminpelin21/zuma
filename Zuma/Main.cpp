//#include <iostream>
//#include <string>
//#include "Game.h"
//[]
//std::string ballColorToString(BallColor color) 
//{
//    switch (color) {
//    case BallColor::Red:    return "Red";
//    case BallColor::Blue:   return "Blue";
//    case BallColor::Yellow: return "Yellow";
//    case BallColor::Green:  return "Green";
//    case BallColor::Purple: return "Purple";
//    case BallColor::Pink: return "Pink";
//    case BallColor::None:   return "None";
//    default:                return "Unknown";
//    }
//}
//
//int main() 
//{
//    Game game;
//    game.start();
//
//    std::cout << "=== Zuma Game Initialized ===\n";
//    std::cout << "Initial ball count: " << game.getBalls().size() << "\n\n";
//
//    game.update(1.0f);
//
//    std::cout << "=== Ball States After 1.0s Update ===\n";
//    const auto activeBalls = game.getBallsData();
//    for (const auto& ball : activeBalls) {
//        std::cout << "Ball ID: " << ball.id
//            << " | Position: (" << ball.position.x << ", " << ball.position.y << ")"
//            << " | Color: " << ballColorToString(ball.color) << "\n";
//    }
//
//    return 0;
//}