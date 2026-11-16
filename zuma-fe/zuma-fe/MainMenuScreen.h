#pragma once
#include "IScreen.h"
#include "Button.h"

class GameApplication;

class MainMenuScreen : public IScreen {
private:
    GameApplication& m_app;
    Button m_playBtn;
    Button m_difficultyBtn;
    Button m_exitBtn;

public:
    explicit MainMenuScreen(GameApplication& app);
    void update(float deltaTime) override;
    void render() const override;
};