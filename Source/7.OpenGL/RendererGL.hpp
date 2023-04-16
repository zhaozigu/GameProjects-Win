#pragma once

#include "RendererInterface.hpp"
#include "SDL.h"
#include "GLShader.hpp"

class RendererGL : public RendererInterface
{
public:
    RendererGL(SDL_Window *window);

    virtual bool Initialize() override;

    virtual ~RendererGL();

    virtual void DestoryRenderer();

    virtual int DrawTexture(ITextureAsset &tex, const RendererRect &rect, double angle) override;

    virtual int DrawTextureSimple(ITextureAsset &tex, const std::optional<RendererRect> &src = std::nullopt, std::optional<RendererRect> dest = std::nullopt) override;

    virtual void Present();

    virtual void Clear();

    virtual void RenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    virtual void RenderColorFloat(float r, float g, float b, float a);

    virtual bool LoadShaders(const std::string &vertName, const std::string &fragName);

    virtual void DrawElements(size_t count);

protected:
    SDL_Window *window_ = nullptr;
    SDL_GLContext context_ = nullptr;

    GLShader mSpriteShader;
};
