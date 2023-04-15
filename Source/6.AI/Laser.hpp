#pragma once
#include "Core/Actor.hpp"

class Laser : public Actor
{
public:
    Laser();

    ~Laser();

    void UpdateActor(float deltaTime) override;

    void Initialize(GamePtr game) override;

private:
    float mDeathTimer;
    std::shared_ptr<class CircleComponent> mCircle;
    std::shared_ptr<class SpriteComponent> sc;
};