#include "Ship.h"

#include <memory>

#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"


Ship::Ship()
    : Actor()
    , mLaserCooldown(0.0f)
    , mGame(nullptr)
{
}

void Ship::UpdateActor(float deltaTime)
{
   mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState)
{
    if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
    {
        // 创建激光
        auto laser = std::make_shared<Laser>();
        laser->Initialize(mGame);
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        // 设定冷却期
        mLaserCooldown = 0.5f;
    }
}

void Ship::Initialize(GamePtr game)
{
    Actor::Initialize(game);

    mGame = game;

    // 创建一个动画精灵组件
    std::shared_ptr<AnimSpriteComponent> asc = std::make_shared<AnimSpriteComponent>(GetSelf(), 150);
    asc->SetTexture(game->GetTexture("Assets/Ship.png"));
    asc->AddComponent();

    std::shared_ptr<InputComponent> ic = std::make_shared<InputComponent>(GetSelf());
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(300.0f);
    ic->SetMaxAngularSpeed(Math::TwoPi);
    ic->AddComponent();
}
