#include "Laser.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Laser::Laser()
    : mDeathTimer(1.0f)
{
#ifdef _DEBUG
    SDL_Log("Laser()");
#endif // _DEBUG
}

Laser::~Laser()
{
#ifdef _DEBUG
    SDL_Log("~Laser()");
#endif // _DEBUG
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
        for (auto &ast : GetGame()->GetAsteroids())
        {
            if (Intersect(*mCircle, *(ast->GetCircle())))
            {
                // 消灭小行星
                SetState(State::EDead);
                ast->SetState(State::EDead);
                GetGame()->RemoveAsteroid(move(ast));
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
    sc->SetTexture(game->GetTexture("Assets/Laser.png"));
    sc->AddComponent();

    mc = std::make_shared<MoveComponent>(GetSelf());
    mc->SetForwardSpeed(800.0f);
    mc->AddComponent();

    mCircle = std::make_shared<CircleComponent>(GetSelf());
    mCircle->SetRadius(11.0f);
    mCircle->AddComponent();
}
