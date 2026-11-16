#pragma once

class IScreen {
public:
    virtual ~IScreen() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render() const = 0;
};