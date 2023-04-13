#include "Game.hpp"
#include <optional>
#include <SDL.h>

using SDL_WinPtr = SDL_Window *;

class Game::Impl
{
public:
	std::optional<SDL_WinPtr> optWindow;
	bool mIsRunning = true;
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

	return true;
}

bool Game::Initialize()
{
	return Initialize("Game", 100, 100, 1024, 768);
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
	SDL_DestroyWindow(impl->optWindow.value());
	SDL_Quit();
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
