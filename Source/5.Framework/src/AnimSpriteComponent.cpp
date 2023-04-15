#include "AnimSpriteComponent.hpp"

AnimSpriteComponent::AnimSpriteComponent(std::weak_ptr<Actor> owner, int drawOrder)
    : SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures.size() > 0)
    {
        // 基于帧速和增量时间更新当前帧
        mCurrFrame += mAnimFPS * deltaTime;

        // 如果需要，绕回动画开始处
        while (mCurrFrame >= mAnimTextures.size())
        {
            mCurrFrame -= mAnimTextures.size();
        }

        // 设置当前texture
        SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<std::shared_ptr<ITextureAsset>> &textures)
{
    mAnimTextures = textures;
    if (mAnimTextures.size() > 0)
    {
        // 激活texture为第一帧
        mCurrFrame = 0.0f;
        SetTexture(mAnimTextures[0]);
    }
}
