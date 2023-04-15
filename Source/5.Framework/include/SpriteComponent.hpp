#pragma once

#include "Core/Component.hpp"
#include "Asset.hpp"
#include "RendererInterface.hpp"
#include <memory>

class SpriteComponent : public Component
{
public:
    // (越低的order对应越深的背面)
    SpriteComponent(std::weak_ptr<Actor> owner, int drawOrder = 100);
    virtual ~SpriteComponent();

    virtual void Draw(std::shared_ptr<RendererInterface>&& renderer);
    virtual void SetTexture(std::shared_ptr<ITextureAsset>& texture);

    virtual void AddComponent() override;

    virtual void RemoveComponent() override;

    virtual int GetDrawOrder() const { return mDrawOrder; }

protected:
    std::shared_ptr<ITextureAsset> mTexture;
    int mDrawOrder;
};
