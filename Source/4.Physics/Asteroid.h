#pragma once
#include <memory>

#include "Actor.h"

class Asteroid : public Actor
{
public:
    Asteroid();
    ~Asteroid();

    virtual void Initialize(GamePtr game) override;

    std::shared_ptr<class CircleComponent> &GetCircle() { return mCircle; }

private:
    std::shared_ptr<class CircleComponent> mCircle;
    std::shared_ptr<class SpriteComponent> sc;
};
