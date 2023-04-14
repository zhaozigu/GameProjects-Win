#pragma once
#include <memory>
#include <optional>
#include "SDL.h"
#include "Asset.hpp"

class ATexture_SDL;

template <>
struct asset_traits<AssetType::Texture>
{
    using asset_type = SDL_Texture *;
};

class ATexture_SDL : public Asset<AssetType::Texture>
{
public:
    ATexture_SDL(std::optional<SDL_Renderer *> &mRenderer);

    virtual ~ATexture_SDL();

    virtual bool Initialize() override;

    virtual bool LoadTexture(const char *filename);

protected:
    std::optional<SDL_Renderer *> renderer_;
};
