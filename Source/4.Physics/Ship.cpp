#include "Ship.h"

#include <memory>

#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"

Ship::Ship()
    : Actor()
{
}

void Ship::UpdateActor(float deltaTime)
{
   // mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState)
{

}

void Ship::Initialize(GamePtr game)
{
    Actor::Initialize(game);

    // 创建一个动画精灵组件
    std::shared_ptr<AnimSpriteComponent> asc = std::make_shared<AnimSpriteComponent>(GetSelf());
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
