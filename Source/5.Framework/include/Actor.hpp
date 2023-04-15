#pragma once

#include <vector>
#include <memory>
#include "Math.hpp"

class GameInterface;
class Component;

using GamePtr = GameInterface *;
using SharedComp = std::shared_ptr<Component>;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
    enum class State
    {
        EActive,
        EPaused,
        EDead
    };

    Actor();
    virtual ~Actor();

    virtual void Initialize(GamePtr game);

    virtual std::shared_ptr<Actor> GetSelf()
    {
        return shared_from_this();
    }

    virtual void BindGame(GamePtr game);

    // 从 Game 调用 Update 函数
    virtual void Update(float deltaTime);

    // 更新属于该actor的所有组件 (不被继承重写)
    virtual void UpdateComponents(float deltaTime);

    // actor特有更新代码
    virtual void UpdateActor(float deltaTime);

    // Getters/setters
    virtual const Vector2 &GetPosition() const { return mPosition; }
    virtual void SetPosition(const Vector2 &pos) { mPosition = pos; }
    virtual float GetScale() const { return mScale; }
    virtual void SetScale(float scale) { mScale = scale; }
    virtual float GetRotation() const { return mRotation; }
    virtual void SetRotation(float rotation) { mRotation = rotation; }

    virtual Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

    virtual State GetState() const { return mState; }
    virtual void SetState(State state) { mState = state; }
    virtual GamePtr GetGame() { return mGame; }

    // 添加/移除 组件
    virtual void AddComponent(SharedComp &&component);
    virtual void RemoveComponent(SharedComp &&component);

    // 被 Game 调用的的 ProcessInput
    virtual void ProcessInput(const uint8_t *keyState);

    // 特定 Actor 输入
    virtual void ActorInput(const uint8_t *keyState);

protected:
    // Actor的状态
    State mState;

    // 变形
    Vector2 mPosition;
    float mScale;
    float mRotation;

    std::vector<SharedComp> mComponents;
    GamePtr mGame;
};
