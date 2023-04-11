#include "Actor.h"
#include <iostream>
#include <algorithm>

#include "Game.h"
#include "Component.h"

Actor::Actor()
    : mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(nullptr)
{
}

void Actor::BindGame(GamePtr game)
{
    mGame = game;
    mGame->AddActor(GetSelf());
}

Actor::~Actor()
{
    std::cerr << "~Actor" << std::endl;
}

void Actor::Update(float deltaTime)
{
    if (mState == EActive)
    {
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(float deltaTime)
{
    for (auto &comp : mComponents)
    {
        comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(std::shared_ptr<Component>&& component)
{
    // 插入排序：在一个已经排序好的向量中找到插入点
    // (找到第一个比我大的地方)
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (;
         iter != mComponents.end();
         ++iter)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }

    // 在迭代器之前插入元素
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(std::shared_ptr<Component>&& component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}