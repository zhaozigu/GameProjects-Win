#pragma once
#include <vector>
#include <memory>
#include "Math.hpp"

class Game;
class Component;

using GamePtr = Game *;

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

    // 从 Game 调用 Update 函数 (不被继承重写)
    void Update(float deltaTime);
    // 更新属于该actor的所有组件 (不被继承重写)
    void UpdateComponents(float deltaTime);
    // actor特有更新代码 (可重写)
    virtual void UpdateActor(float deltaTime);

    // Getters/setters
    const Vector2 &GetPosition() const { return mPosition; }
    void SetPosition(const Vector2 &pos) { mPosition = pos; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; }
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }

    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }
    virtual GamePtr GetGame() { return mGame; }

    // 添加/移除 组件
    virtual void AddComponent(std::shared_ptr<Component> &&component);
    virtual void RemoveComponent(std::shared_ptr<Component> &&component);

private:
    // Actor的状态
    State mState;

    // 变形
    Vector2 mPosition;
    float mScale;
    float mRotation;

    std::vector<std::shared_ptr<Component>> mComponents;
    GamePtr mGame;
};