#include "Asteroid.hpp"
#include "Core/Random.hpp"
#include "SpriteComponent.hpp"
#include "MoveComponent.hpp"
#include "CircleComponent.hpp"
#include "Game.hpp"
#include "TextureAsset.hpp"

Asteroid::Asteroid()
{
}

Asteroid::~Asteroid()
{
    GetGame()->RemoveSprite(move(sc));
}

void Asteroid::Initialize(GamePtr game)
{
    Actor::Initialize(game);
    std::shared_ptr<class MoveComponent> mc;

    // 初始化一个随机的位置和方向
    Vector2 randPos = Random::GetVector(Vector2::Zero,
                                        Vector2(1024.0f, 768.0f));
    SetPosition(randPos);

    sc = std::make_shared<SpriteComponent>(GetSelf());
    auto temp = GetTextureAsset(Game::CastPtr(game)->GetTexture("Assets/Asteroid.png"));
    sc->SetTexture(temp);
    sc->AddComponent();

    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    mc = std::make_shared<MoveComponent>(GetSelf());
    mc->SetForwardSpeed(150.0f);
    mc->AddComponent();

    mCircle = std::make_shared<CircleComponent>(GetSelf());
    mCircle->SetRadius(40.0f);
    mCircle->AddComponent();

    Game::CastPtr(game)->AddAsteroid(std::dynamic_pointer_cast<Asteroid>(GetSelf()));
}
