#pragma once
#include "Asset.hpp"
#include <optional>

typedef struct RendererRect
{
    int x, y;
    int w, h;
} RendererRect;

class RendererInterface
{
public:
    RendererInterface() {};

    virtual bool Initialize() { return false;  }

    virtual ~RendererInterface() {};

    virtual int DrawTexture(ITextureAsset& tex, const RendererRect& rect, double angle) = 0;

    virtual int DrawTextureSimple(ITextureAsset &tex, const std::optional<RendererRect>& src = std::nullopt, std::optional<RendererRect> dest = std::nullopt) = 0;
};