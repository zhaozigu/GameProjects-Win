#include "RendererSDL.hpp"
#include "TextureAsset.hpp"

RendererSDL::RendererSDL(SDL_Window *window)
	: window_(window)
{
}

bool RendererSDL::Initialize()
{
	renderer_ = SDL_CreateRenderer(
		window_,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer_)
	{
		SDL_Log("failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

RendererSDL::~RendererSDL()
{
#ifdef _DEBUG
	SDL_Log("~RendererSDL()");
#endif // _DEBUG
	if (renderer_)
	{
		DestoryRenderer();
	}
}

void RendererSDL::DestoryRenderer()
{
	if (renderer_)
	{
		SDL_DestroyRenderer(renderer_);
	}
}

static SDL_Rect CreateRect(const RendererRect &r)
{
	return { .x = r.x, .y = r.y, .w = r.w, .h = r.h };
}

int RendererSDL::DrawTexture(ITextureAsset &tex, const RendererRect &rect, double angle)
{
	SDL_Rect r = CreateRect(rect);

	// 绘制（必须把弧度转换成角度，并且调整方向）
	return SDL_RenderCopyEx(renderer_,
							dynamic_cast<ATexture_SDL &>(tex).GetTexture(),
							nullptr,
							&r,
							angle,
							nullptr,
							SDL_FLIP_NONE);
}

int RendererSDL::DrawTextureSimple(ITextureAsset &tex, const std::optional<RendererRect> &src, std::optional<RendererRect> dest)
{
	SDL_Rect srcrect = src.has_value() ? CreateRect(src.value()) : SDL_Rect{};
	SDL_Rect destrect = dest.has_value() ? CreateRect(dest.value()) : SDL_Rect{};

	return SDL_RenderCopy(renderer_, dynamic_cast<ATexture_SDL &>(tex).GetTexture(),
						  src.has_value() ? &srcrect : nullptr,
						  dest.has_value() ? &destrect : nullptr);
}

void RendererSDL::Present()
{
	SDL_RenderPresent(renderer_);
}

void RendererSDL::Clear()
{
	SDL_RenderClear(renderer_);
}

void RendererSDL::RenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}
