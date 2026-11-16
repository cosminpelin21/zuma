#pragma once
#include "IScreen.h"
#include "Button.h"

class GameApplication;

class DifficultyScreen : public IScreen {
private:
    GameApplication& m_app;
    Button m_easyBtn;
    Button m_mediumBtn;
    Button m_hardBtn;
    Button m_backBtn;

public:
    explicit DifficultyScreen(GameApplication& app);
    void update(float deltaTime) override;
    void render() const override;
};