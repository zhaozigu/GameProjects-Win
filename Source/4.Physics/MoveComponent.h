#pragma once

#include "Component.h"

class MoveComponent : public Component
{
public:
    // 值低者优先更新
    MoveComponent(std::weak_ptr<class Actor> owner, int updateOrder = 10);

    virtual ~MoveComponent();

    virtual void Update(float deltaTime) override;

    float GetAngularSpeed() const { return mAngularSpeed; }
    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
    // 控制旋转
    float mAngularSpeed;
    // 控制方向移动
    float mForwardSpeed;
};
