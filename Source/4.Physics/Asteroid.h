#pragma once
#include <memory>

#include "Actor.h"

class Asteroid : public Actor
{
public:
    Asteroid();

    virtual void Initialize(GamePtr game) override;
private:

    std::shared_ptr<class SpriteComponent> sc;
    std::shared_ptr<class MoveComponent> mc;
};

