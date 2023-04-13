#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(std::weak_ptr<Actor> owner)
    : Component(owner), mRadius(0.0f)
{
}

const Vector2 &CircleComponent::GetCenter() const
{
    if (auto owner = mOwner.lock())
    {
        return owner->GetPosition();
    }
    return Vector2::Zero;
}

float CircleComponent::GetRadius() const
{
    if (auto owner = mOwner.lock())
    {
        return owner->GetScale() * mRadius;
    }
    return 0.0f;
}

bool Intersect(const CircleComponent &a, const CircleComponent &b)
{
    // 计算距离的平方
    Vector2 diff = a.GetCenter() - b.GetCenter();
    float distSq = diff.LengthSq();

    // 计算半径之和，再平方
    float radiiSq = a.GetRadius() + b.GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}
