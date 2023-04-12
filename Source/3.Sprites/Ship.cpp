#include "Ship.h"

#include <memory>

#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship()
    : mRightSpeed(0.0f), mDownSpeed(0.0f)
{
}

void Ship::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);
    // 基于速度和增量时间更新位置
    Vector2 pos = GetPosition();
    pos.x += mRightSpeed * deltaTime;
    pos.y += mDownSpeed * deltaTime;

    // 限制在屏幕左半部分
    if (pos.x < 25.0f)
    {
        pos.x = 25.0f;
    }
    else if (pos.x > 500.0f)
    {
        pos.x = 500.0f;
    }
    if (pos.y < 25.0f)
    {
        pos.y = 25.0f;
    }
    else if (pos.y > 743.0f)
    {
        pos.y = 743.0f;
    }
    SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t *state)
{
    mRightSpeed = 0.0f;
    mDownSpeed = 0.0f;
    // 右/左
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        mRightSpeed += 250.0f;
    }
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        mRightSpeed -= 250.0f;
    }
    // 上/下
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
    {
        mDownSpeed += 300.0f;
    }
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    {
        mDownSpeed -= 300.0f;
    }
}

void Ship::Initialize(GamePtr game)
{
    Actor::Initialize(game);

    // 创建一个动画精灵组件
    std::shared_ptr<AnimSpriteComponent> asc = std::make_shared<AnimSpriteComponent>(GetSelf());
    std::vector<SDL_Texture*> anims = {
        GetGame()->GetTexture("Assets/Ship01.png"),
        GetGame()->GetTexture("Assets/Ship02.png"),
        GetGame()->GetTexture("Assets/Ship03.png"),
        GetGame()->GetTexture("Assets/Ship04.png"),
    };
    asc->SetAnimTextures(anims);
    asc->AddComponent();
}
