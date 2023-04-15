#include "TextureAsset.hpp"
#include "RendererSDL.hpp"
#include "SDL_image.h"

ATexture_SDL::ATexture_SDL(const char *filename, RendererSDL * renderer)
    : renderer_(renderer)
{
    SDL_Surface *surf = IMG_Load(filename);
    if (!surf)
    {
        SDL_Log("failed to load image file %s", filename);
        return;
    }

    texture_ = SDL_CreateTextureFromSurface(renderer->GetSDLRenderer(), surf);
    SDL_FreeSurface(surf);

    if (!texture_)
    {
        SDL_Log("%s surface conversion to texture failed! %s", filename, SDL_GetError());
        return;
    }

    SDL_QueryTexture(texture_, nullptr, nullptr, &texWidth_, &texHeight_);

    isInit_ = true;
}

ATexture_SDL::~ATexture_SDL()
{
#ifdef _DEBUG
    SDL_Log("~ATexture_SDL()");
#endif // _DEBUG

    if (texture_)
    {
        SDL_DestroyTexture(texture_);
    }
}

bool ATexture_SDL::Initialize()
{
    return isInit_;
}