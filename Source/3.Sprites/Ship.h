#pragma once

#include "Actor.h"

class Ship : public Actor
{
public:
  Ship();
  void UpdateActor(float deltaTime) override;

  void ProcessKeyboard(const uint8_t* state);
  float GetRightSpeed() const { return mRightSpeed; }
  float GetDownSpeed() const { return mDownSpeed; }

  void Initialize(GamePtr game) override;

private:
  float mRightSpeed;
  float mDownSpeed;
};
