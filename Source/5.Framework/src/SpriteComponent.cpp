#include "SpriteComponent.hpp"
#include "Core/GameInterface.hpp"
#include "Core/Actor.hpp"

SpriteComponent::SpriteComponent(std::weak_ptr<Actor> owner, int drawOrder)
    : Component(owner), mTexture(nullptr), mDrawOrder(drawOrder)
{
}

SpriteComponent::~SpriteComponent()
{
    RemoveComponent();
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

void SpriteComponent::Draw(std::shared_ptr<RendererInterface> &&renderer)
{
    if (mTexture)
    {
        RendererRect r;
        auto owner = mOwner.lock();
        // 根据比例放缩
        r.w = static_cast<int>(mTexture->GetTexWidth() * owner->GetScale());
        r.h = static_cast<int>(mTexture->GetTexHeight() * owner->GetScale());
        // 居中显示
        r.x = static_cast<int>(owner->GetPosition().x - r.w / 2.0);
        r.y = static_cast<int>(owner->GetPosition().y - r.h / 2.0);

        renderer->DrawTexture(*mTexture.get(), r, -Math::ToDegrees(owner->GetRotation()));
    }
}

void SpriteComponent::SetTexture(std::shared_ptr<ITextureAsset> &texture)
{
    mTexture = texture;
}