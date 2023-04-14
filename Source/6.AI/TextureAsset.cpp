#include "TextureAsset.hpp"
#include "SDL_image.h"

ATexture_SDL::ATexture_SDL(SDL_Renderer *renderer)
    : renderer_(renderer)
{
}

ATexture_SDL::~ATexture_SDL()
{
    if (GetAsset())
    {
        SDL_DestroyTexture(GetAsset());
    }
}

bool ATexture_SDL::Initialize()
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool ATexture_SDL::LoadTexture(const char *filename)
{
    // 从文件中加载
    SDL_Surface *surf = IMG_Load(filename);

    if (!surf)
    {
        SDL_Log("failed to load image file %s", filename);
        return false;
    }

    // 从 surface 创建 texture
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer_, surf);
    SDL_FreeSurface(surf);
    if (!tex)
    {
        SDL_Log("%s surface conversion to texture failed! %s", filename, SDL_GetError());
        return false;
    }

    SetAsset(tex);
    return true;
}
