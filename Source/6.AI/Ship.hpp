#pragma once

#include "Core/Actor.hpp"

class Ship : public Actor
{
public:
	Ship();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t *keyState) override;

	void Initialize(GamePtr game) override;

private:
	float mLaserCooldown;
};
