#include "Component.h"
#include "Actor.h"

Component::Component(std::weak_ptr<Actor> owner, int updateOrder)
	: mOwner(owner), mUpdateOrder(updateOrder)
{
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

Component::~Component()
{

}

void Component::Update(float deltaTime)
{
}