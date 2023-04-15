#include "BGSpriteComponent.hpp"
#include "Actor.hpp"

BGSpriteComponent::BGSpriteComponent(std::weak_ptr<Actor> owner, int drawOrder)
    : SpriteComponent(owner, drawOrder), mScrollSpeed(0.0f)
{
}

void BGSpriteComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);
    for (auto &bg : mBGTextures)
    {
        // 更新x偏移
        bg.mOffset.x += mScrollSpeed * deltaTime;
        // 如果完全偏离屏幕，重置偏移
        if (bg.mOffset.x < -mScreenSize.x)
        {
            bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
        }
    }
}

void BGSpriteComponent::Draw(std::shared_ptr<RendererInterface> &&renderer)
{
    // 绘制每个背景 texture
    for (auto &bg : mBGTextures)
    {
        RendererRect r;
        // 设定屏幕大小尺寸
        r.w = static_cast<int>(mScreenSize.x);
        r.h = static_cast<int>(mScreenSize.y);

        if (auto owner = mOwner.lock())
        {
            // 居中放置
            r.x = static_cast<int>(owner->GetPosition().x - r.w / 2.0 + bg.mOffset.x);
            r.y = static_cast<int>(owner->GetPosition().y - r.h / 2.0 + bg.mOffset.y);
        }

        renderer->DrawTextureSimple(*bg.mTexture.get(), std::nullopt, r);
    }
}

void BGSpriteComponent::SetBGTextures(const std::vector<std::shared_ptr<ITextureAsset>> &textures)
{
    int count = 0;
    for (auto &tex : textures)
    {
        BGTexture temp;
        temp.mTexture = tex;
        // texture大小是屏幕宽度
        temp.mOffset.x = count * mScreenSize.x;
        temp.mOffset.y = 0;
        mBGTextures.emplace_back(temp);
        count++;
    }
}