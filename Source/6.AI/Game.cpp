#include "Game.hpp"
#include "TextureAsset.hpp"

#include <optional>
#include <SDL.h>

using SDL_WinPtr = SDL_Window *;

class Game::Impl
{
public:
	std::optional<SDL_WinPtr> optWindow;
	SDL_Renderer *renderer = nullptr;
	bool mIsRunning = true;
	AssetManager manager;
};

Game::Game()
	: impl(std::make_unique<Game::Impl>())
{
}

Game::~Game() {}

bool Game::Initialize(const std::string &windowsName, int x, int y, int w, int h)
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	impl->optWindow = SDL_CreateWindow(
		windowsName.c_str(),
		x,
		y,
		w,
		h,
		0);

	if (!impl->optWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// 创建渲染器
	impl->renderer = SDL_CreateRenderer(
		impl->optWindow.value(),
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!impl->renderer)
	{
		SDL_Log("failed to create renderer: %s", SDL_GetError());
		return false;
	}

	auto tempTex = std::make_shared<ATexture_SDL>(impl->renderer);
	if (tempTex->Initialize() && tempTex->LoadTexture("Assets/test.png"))
	{
		impl->manager.AddResource("test", AssetManager::AssetData{AssetType::Texture, std::move(tempTex)});
		SDL_RenderCopy(impl->renderer, dynamic_cast<ATexture_SDL *>(impl->manager.GetResource("test").resource.get())->GetAsset(), 0, 0);
	}
	else
	{
		return false;
	}
	return true;
}

bool Game::Initialize()
{
	return Initialize("AI Tower defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768);
}

void Game::RunLoop()
{
	while (impl->mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(impl->renderer);
	SDL_DestroyWindow(impl->optWindow.value());
	SDL_Quit();
}

void Game::AddActor(SharedActor &&actor)
{
}

void Game::RemoveActor(SharedActor &&actor)
{
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			impl->mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		impl->mIsRunning = false;
	}
}

void Game::UpdateGame()
{
}

void Game::GenerateOutput()
{
}
