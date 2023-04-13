#pragma once

#include "Component.h"
#include <memory>

#include <SDL.h>

class SpriteComponent : public Component
{
public:
    // (越低的order对应越深的背面)
    SpriteComponent(std::weak_ptr<Actor> owner, int drawOrder = 100);
    virtual ~SpriteComponent();

    virtual void Draw(SDL_Renderer *renderer);
    virtual void SetTexture(SDL_Texture *texture);

    virtual void AddComponent() override;

    virtual void RemoveComponent() override;

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

protected:
    SDL_Texture *mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};