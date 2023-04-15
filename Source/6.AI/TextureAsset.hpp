#pragma once
#include <memory>
#include <optional>
#include "SDL.h"
#include "Asset.hpp"
#include "Game.hpp"

class ATexture_SDL : public ITextureAsset
{
public:
    ATexture_SDL(const char *filename, class RendererSDL *mRenderer);

    virtual ~ATexture_SDL();

    virtual bool Initialize() override;

    virtual SDL_Texture *GetTexture() { return texture_; };

protected:
    class RendererSDL *renderer_;

    SDL_Texture *texture_ = nullptr;
    bool isInit_ = false;
};

inline std::shared_ptr<ITextureAsset> GetTextureAsset(std::shared_ptr<ATexture_SDL> texture)
{
    return std::dynamic_pointer_cast<ITextureAsset>(texture);
}
