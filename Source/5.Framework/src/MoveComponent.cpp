#include "MoveComponent.hpp"
#include "Core/Actor.hpp"

MoveComponent::MoveComponent(std::weak_ptr<class Actor> owner, int updateOrder)
    : Component(owner, updateOrder), mAngularSpeed(0.0f), mForwardSpeed(0.0f)
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(float deltaTime)
{
    if (!Math::NearZero(mAngularSpeed))
    {
        if (auto owner = mOwner.lock())
        {
            float rot = owner->GetRotation();
            rot += mAngularSpeed * deltaTime;
            owner->SetRotation(rot);
        }
    }

    if (!Math::NearZero(mForwardSpeed))
    {
        if (auto owner = mOwner.lock())
        {
            Vector2 pos = owner->GetPosition();
            pos += owner->GetForward() * mForwardSpeed * deltaTime;

            // (Screen wrapping code only for asteroids)
            if (pos.x < 0.0f)
            {
                pos.x = 1022.0f;
            }
            else if (pos.x > 1024.0f)
            {
                pos.x = 2.0f;
            }

            if (pos.y < 0.0f)
            {
                pos.y = 766.0f;
            }
            else if (pos.y > 768.0f)
            {
                pos.y = 2.0f;
            }

            owner->SetPosition(pos);
        }
    }
}
