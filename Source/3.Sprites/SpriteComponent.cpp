#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(std::weak_ptr<Actor> owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
#ifdef _DEBUG
    SDL_Log("SpriteComponent()");
#endif _DEBUG
}

SpriteComponent::~SpriteComponent()
{
#ifdef _DEBUG
  SDL_Log("~SpriteComponent()");
#endif _DEBUG
}

void SpriteComponent::RemoveComponent()
{
  if (auto owner = mOwner.lock())
  {
    owner->GetGame()->RemoveSprite(std::dynamic_pointer_cast<SpriteComponent>(GetSelf()));
    owner->RemoveComponent(GetSelf());
  }
}

void SpriteComponent::AddComponent()
{
  if (auto owner = mOwner.lock())
  {
    owner->GetGame()->AddSprite(std::dynamic_pointer_cast<SpriteComponent>(GetSelf()));
    owner->AddComponent(GetSelf());
  }
}

void SpriteComponent::Draw(SDL_Renderer *renderer)
{
  if (mTexture)
  {
    SDL_Rect r;
    auto owner = mOwner.lock();
    // 根据比例放缩
    r.w = static_cast<int>(mTexWidth * owner->GetScale());
    r.h = static_cast<int>(mTexHeight * owner->GetScale());
    // 居中显示
    r.x = static_cast<int>(owner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(owner->GetPosition().y - r.h / 2);

    // 绘制（必须把弧度转换成角度，并且调整方向）
    SDL_RenderCopyEx(renderer,
                     mTexture,
                     nullptr,
                     &r,
                     -Math::ToDegrees(owner->GetRotation()),
                     nullptr,
                     SDL_FLIP_NONE);
  }
}

void SpriteComponent::SetTexture(SDL_Texture *texture)
{
  mTexture = texture;
  // 设置 宽度/高度
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}