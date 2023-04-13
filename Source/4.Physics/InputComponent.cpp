#include "InputComponent.h"

InputComponent::InputComponent(std::weak_ptr<class Actor> owner)
:MoveComponent(owner)
,mForwardKey(0)
,mBackKey(0)
,mClockwiseKey(0)
,mCounterClockwiseKey(0)
{
    
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
    // 计算方向速度
    float forwardSpeed = 0.0f;
    if (keyState[mForwardKey])
    {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (keyState[mBackKey])
    {
        forwardSpeed -= mMaxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);
    
    // 计算角速度
    float angularSpeed = 0.0f;
    if (keyState[mClockwiseKey])
    {
        angularSpeed += mMaxAngularSpeed;
    }
    if (keyState[mCounterClockwiseKey])
    {
        angularSpeed -= mMaxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
}

