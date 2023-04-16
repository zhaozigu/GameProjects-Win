#include "Game.hpp"
#include "AssetManager.hpp"
#include "SpriteComponent.hpp"
#include "BGSpriteComponent.hpp"
#include "Core/Actor.hpp"
#include "RendererGL.hpp"
#include "VertexArray.hpp"

#include <vector>
#include <SDL.h>

class Game::Impl
{
public:
	SDL_Window *window;
	bool mIsRunning = true;
	AssetManager manager;
	Uint32 mTicksCount;
	bool mUpdatingActors = false;
	std::unique_ptr<VertexArray> mSpriteVerts;
	std::shared_ptr<RendererGL> renderer;

	std::vector<std::shared_ptr<Actor>> mActors;
	std::vector<std::shared_ptr<Actor>> mPendingActors;
	std::vector<std::shared_ptr<SpriteComponent>> mSprites;
};

Game::Game()
	: impl(std::make_unique<Game::Impl>())
{
}

void Game::LoadData()
{
}

void Game::CreateSpriteVerts()
{
	std::vector<float> vertexBuffer = {
		-0.5f, 0.5f, 0.0f, // 0
		0.5f, 0.5f, 0.0f,  // 1
		0.5f, -0.5f, 0.0f, // 2
		-0.5f, -0.5f, 0.0f // 3
	};

	std::vector<unsigned int> indexBuffer = {
		0, 1, 2,
		2, 3, 0};

	impl->mSpriteVerts = std::make_unique<VertexArray>(vertexBuffer, indexBuffer);
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

	impl->window = SDL_CreateWindow(
		windowsName.c_str(),
		x,
		y,
		w,
		h,
		SDL_WINDOW_OPENGL);

	if (!impl->window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	impl->renderer = std::make_shared<RendererGL>(impl->window);
	if (!impl->renderer->Initialize())
	{
		SDL_Log("Failed to create gl renderer");
		return false;
	}

	LoadData();
	CreateSpriteVerts();

	if (!impl->renderer->LoadShaders("Shaders/Basic.vert", "Shaders/Basic.frag"))
	{
		SDL_Log("failed to load shaders");
		return false;
	}

	impl->mTicksCount = SDL_GetTicks();
	return true;
}

bool Game::Initialize()
{
	return Initialize("OpenGL Environment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768);
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
	SDL_DestroyWindow(impl->window);
	SDL_Quit();
}

void Game::AddActor(std::shared_ptr<Actor> &&actor)
{
	// 如果正在更新actor，就添加到待处理列表里
	if (impl->mUpdatingActors)
	{
		impl->mPendingActors.emplace_back(actor);
	}
	else
	{
		impl->mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(std::shared_ptr<Actor> &&actor)
{
	// 是否在待定actor中
	auto iter = std::find(impl->mPendingActors.begin(), impl->mPendingActors.end(), actor);
	if (iter != impl->mPendingActors.end())
	{
		// 交换到尾部（避免复制)
		std::iter_swap(iter, impl->mPendingActors.end() - 1);
		impl->mPendingActors.pop_back();
	}

	// 是否在 actor中
	iter = std::find(impl->mActors.begin(), impl->mActors.end(), actor);
	if (iter != impl->mActors.end())
	{
		// 交换到尾部（避免复制)
		std::iter_swap(iter, impl->mActors.end() - 1);
		impl->mActors.pop_back();
	}
}

void Game::AddSprite(std::shared_ptr<SpriteComponent> &&sprite)
{
	// 在有序向量中找到插入点
	// (第一个比传入的sprite的order顺序大的元素)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = impl->mSprites.begin();
	for (;
		 iter != impl->mSprites.end();
		 ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// 在迭代器之前插入
	impl->mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(std::shared_ptr<SpriteComponent> &&sprite)
{
	// (不能交换，不然顺序就没了)
	auto iter = std::find(impl->mSprites.begin(), impl->mSprites.end(), sprite);
	if (iter != impl->mSprites.end())
	{
		impl->mSprites.erase(iter);
	}
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

	impl->mUpdatingActors = true;
	for (auto actor : impl->mActors)
	{
		actor->ProcessInput(state);
	}
	impl->mUpdatingActors = false;
}

void Game::UpdateGame()
{
	// 等到与上一帧间隔 16ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), impl->mTicksCount + 16))
		;

	// 增量时间是上一帧到现在的时间差
	// (转换成秒)
	float deltaTime = (SDL_GetTicks() - impl->mTicksCount) / 1000.0f;

	// 更新运行时间(为下一帧)
	impl->mTicksCount = SDL_GetTicks();
	// 固定增量时间最大值
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	impl->mTicksCount = SDL_GetTicks();

	// 更新所有actor
	impl->mUpdatingActors = true;
	for (auto &actor : impl->mActors)
	{
		actor->Update(deltaTime);
	}
	impl->mUpdatingActors = false;

	// 移动待定actor到mActors
	for (auto &pending : impl->mPendingActors)
	{
		impl->mActors.emplace_back(pending);
	}
	impl->mPendingActors.clear();

	RemoveDeadActors(impl->mActors);
}

void Game::GenerateOutput()
{
	impl->renderer->RenderColorFloat(0.86f, 0.86f, 0.86f, 1.0f);
	impl->renderer->Clear();
	impl->mSpriteVerts->SetActive();

	impl->renderer->DrawElements(impl->mSpriteVerts->GetNumIndices());

	impl->renderer->Present();
}

void Game::RemoveDeadActors(std::vector<std::shared_ptr<Actor>> &actors)
{
	auto new_end = std::remove_if(impl->mActors.begin(), impl->mActors.end(), [](const std::shared_ptr<Actor> &actor)
								  { return actor->GetState() == Actor::State::EDead; });

	if (new_end != impl->mActors.end())
	{
		// 使用 erase 一次性删除所有符合条件的元素
		impl->mActors.erase(new_end, impl->mActors.end());
	}
}
