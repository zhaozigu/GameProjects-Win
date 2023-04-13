#include "Asteroid.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"


Asteroid::Asteroid()
{
}

void Asteroid::Initialize(GamePtr game)
{
    Actor::Initialize(game);

    // 初始化一个随机的位置和方向
    Vector2 randPos = Random::GetVector(Vector2::Zero,
        Vector2(1024.0f, 768.0f));
    SetPosition(randPos);

    sc = std::make_shared<SpriteComponent>(GetSelf());
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    sc->AddComponent();

    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    mc = std::make_shared<MoveComponent>(GetSelf());
    mc->SetForwardSpeed(150.0f);
    mc->AddComponent();

    game->AddAsteroid(std::dynamic_pointer_cast<Asteroid>(GetSelf()));
}
