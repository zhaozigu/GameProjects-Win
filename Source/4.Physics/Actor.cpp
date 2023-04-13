#include "Actor.h"
#include <algorithm>

#include "Game.h"
#include "Component.h"

Actor::Actor()
    : mState(State::EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(nullptr)
{
#ifdef _DEBUG
    SDL_Log("Actor()");
#endif // _DEBUG
}

void Actor::BindGame(GamePtr game)
{
    mGame = game;
    mGame->AddActor(GetSelf());
}

Actor::~Actor()
{
#ifdef _DEBUG
    SDL_Log("~Actor()");
#endif // _DEBUG
}

void Actor::Initialize(GamePtr game)
{
    BindGame(game);
}

void Actor::Update(float deltaTime)
{
    if (mState == State::EActive)
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

void Actor::AddComponent(std::shared_ptr<Component> &&component)
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

void Actor::RemoveComponent(std::shared_ptr<Component> &&component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}

void Actor::ProcessInput(const uint8_t* keyState)
{
    if (mState == State::EActive)
    {
        // 首先处理组件的输入
        for (auto comp : mComponents)
        {
            comp->ProcessInput(keyState);
        }
        
        ActorInput(keyState);
    }
}

void Actor::ActorInput(const uint8_t* keyState)
{
}
