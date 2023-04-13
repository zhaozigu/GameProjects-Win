#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <memory>

class AnimSpriteComponent : public SpriteComponent
{
public:
    AnimSpriteComponent(std::weak_ptr<Actor> owner, int drawOrder = 100);
    // 更新每帧动画 (被组件重写)
    virtual void Update(float deltaTime) override;
    // 设置用于动画的 texture
    virtual void SetAnimTextures(const std::vector<SDL_Texture *> &textures);
    // 设置/读取 动画 FPS
    float GetAnimFPS() const { return mAnimFPS; }
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
    // 动画里的所有texture
    std::vector<SDL_Texture *> mAnimTextures;
    // 当前显示的帧
    float mCurrFrame;
    // 动画帧率
    float mAnimFPS;
};
