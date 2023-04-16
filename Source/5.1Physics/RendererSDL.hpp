#pragma once
#include "RendererInterface.hpp"
#include "SDL.h"

class RendererSDL : public RendererInterface
{
public:
    RendererSDL(SDL_Window *optWindow);

    virtual bool Initialize() override;

    virtual ~RendererSDL();

    virtual void DestoryRenderer();

    virtual int DrawTexture(ITextureAsset &tex, const RendererRect &rect, double angle) override;

    virtual int DrawTextureSimple(ITextureAsset &tex, const std::optional<RendererRect>& src = std::nullopt, std::optional<RendererRect> dest = std::nullopt) override;

    virtual SDL_Renderer *GetSDLRenderer()
    {
        return renderer_;
    }

    virtual void Present();

    virtual void Clear();

    virtual void RenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

protected:
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
};