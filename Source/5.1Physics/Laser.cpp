#include "Laser.hpp"
#include "CircleComponent.hpp"
#include "Game.hpp"
#include "Asteroid.hpp"
#include "SpriteComponent.hpp"
#include "MoveComponent.hpp"
#include "TextureAsset.hpp"

Laser::Laser()
    : mDeathTimer(1.0f)
{
}

Laser::~Laser()
{
    GetGame()->RemoveSprite(move(sc));
}

void Laser::UpdateActor(float deltaTime)
{
    // 超时的话，激光就已经衰退了
    mDeathTimer -= deltaTime;
    if (mDeathTimer <= 0.0f)
    {
        SetState(State::EDead);
    }
    else
    {
        for (auto &ast : Game::CastPtr(GetGame())->GetAsteroids())
        {
            if (Intersect(*mCircle, *(ast->GetCircle())))
            {
                // 消灭小行星
                SetState(State::EDead);
                ast->SetState(State::EDead);
                Game::CastPtr(GetGame())->RemoveAsteroid(std::move(ast));
                break;
            }
        }
    }
}

void Laser::Initialize(GamePtr game)
{
    Actor::Initialize(game);

    std::shared_ptr<class MoveComponent> mc;

    sc = std::make_shared<SpriteComponent>(GetSelf());
    auto temp = GetTextureAsset(Game::CastPtr(game)->GetTexture("Assets/Laser.png"));
    sc->SetTexture(temp);
    sc->AddComponent();

    mc = std::make_shared<MoveComponent>(GetSelf());
    mc->SetForwardSpeed(800.0f);
    mc->AddComponent();

    mCircle = std::make_shared<CircleComponent>(GetSelf());
    mCircle->SetRadius(11.0f);
    mCircle->AddComponent();
}
