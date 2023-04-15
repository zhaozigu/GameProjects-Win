#include "Core/Component.hpp"
#include "Core/Actor.hpp"

Component::Component(std::weak_ptr<Actor> owner, int updateOrder)
    : mOwner(owner), mUpdateOrder(updateOrder)
{
}

std::shared_ptr<Component> Component::GetSelf()
{
    return shared_from_this();
}

void Component::AddComponent()
{
    if (auto owner = mOwner.lock())
    {
        owner->AddComponent(GetSelf());
    }
}

void Component::RemoveComponent()
{
    if (auto owner = mOwner.lock())
    {
        owner->RemoveComponent(GetSelf());
    }
}