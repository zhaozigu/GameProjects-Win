#pragma once
#include <vector>
#include <memory>

#include "Math.hpp"
#include "SpriteComponent.hpp"
class Actor;

class BGSpriteComponent : public SpriteComponent
{
public:
    // 背景，所以绘制的顺序默认值比较低
    BGSpriteComponent(std::weak_ptr<Actor> owner, int drawOrder = 10);
    // 从父类中 更新/绘制
    virtual void Update(float deltaTime) override;
    virtual void Draw(std::shared_ptr<RendererInterface> &&renderer) override;
    // 设置被用于背景的texture
    void SetBGTextures(const std::vector<std::shared_ptr<ITextureAsset>> &textures);
    // 读取/设置 屏幕大小和滚动速度
    void SetScreenSize(const Vector2 &size) { mScreenSize = size; }
    void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
    float GetScrollSpeed() const { return mScrollSpeed; }

private:
    // 封装背景图像以及偏移量
    struct BGTexture
    {
        std::shared_ptr<ITextureAsset> mTexture;
        Vector2 mOffset;
    };

    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};