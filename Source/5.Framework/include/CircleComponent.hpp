#pragma once

#include "Core/Component.hpp"
#include "Core/Math.hpp"

class Actor;

class CircleComponent : public Component
{
public:
    CircleComponent(std::weak_ptr<Actor> owner);

    void SetRadius(float radius) { mRadius = radius; }
    float GetRadius() const;

    const Vector2 &GetCenter() const;

private:
    float mRadius;
};

bool Intersect(const CircleComponent &a, const CircleComponent &b);