#include "Game.hpp"
#include "TextureAsset.hpp"
#include "AssetManager.hpp"
#include "RendererSDL.hpp"
#include "SpriteComponent.hpp"
#include "BGSpriteComponent.hpp"
#include "Actor.hpp"
#include "Ship.hpp"

#include <SDL.h>

class Game::Impl
{
public:
	SDL_Window *optWindow;
	std::shared_ptr<RendererSDL> renderer;
	bool mIsRunning = true;
	AssetManager manager;
	std::shared_ptr<Ship> mShip;
	Uint32 mTicksCount;
	bool mUpdatingActors = false;

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
	impl->mShip = std::make_shared<Ship>();
	impl->mShip->Initialize(this);
	impl->mShip->SetPosition(Vector2(100.0f, 384.0f));
	impl->mShip->SetScale(1.5f);

	std::shared_ptr<Actor> temp = std::make_shared<Actor>();
	temp->Initialize(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	 std::shared_ptr<class BGSpriteComponent> bg = std::make_shared<BGSpriteComponent>(temp);
	 bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	 std::vector<std::shared_ptr<ITextureAsset>> bgtexs = {
	 	GetTexture("Assets/Farback01.png"),
	 	GetTexture("Assets/Farback02.png") };
	 bg->SetBGTextures(bgtexs);
	 bg->SetScrollSpeed(-100.0f);
	 bg->AddComponent();

	 // 创建一个更近的背景
	 bg = std::make_shared<BGSpriteComponent>(temp, 50);
	 bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	 bgtexs = {
	 	GetTexture("Assets/Stars.png"),
	 	GetTexture("Assets/Stars.png") };
	 bg->SetBGTextures(bgtexs);
	 bg->SetScrollSpeed(-200.0f);
	 bg->AddComponent();
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

	impl->renderer = std::make_shared<RendererSDL>(impl->optWindow);

	if (!impl->renderer->Initialize())
	{
		SDL_Log("failed to create renderer: %s", SDL_GetError());
		return false;
	}

	LoadData();
	impl->mTicksCount = SDL_GetTicks();
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
	impl->renderer->DestoryRenderer();
	SDL_DestroyWindow(impl->optWindow);
	SDL_Quit();
}


void Game::AddActor(std::shared_ptr<Actor>&& actor)
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

void Game::RemoveActor(std::shared_ptr<Actor>&& actor)
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

void Game::AddSprite(std::shared_ptr<SpriteComponent>&& sprite)
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

void Game::RemoveSprite(std::shared_ptr<SpriteComponent>&& sprite)
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

	impl->mShip->ProcessKeyboard(state);
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
	impl->renderer->Clear();

	// 绘制所有精灵组件
	for (auto& sprite : impl->mSprites)
	{
		sprite->Draw(std::dynamic_pointer_cast<RendererInterface>(impl->renderer));
	}

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

std::shared_ptr<ATexture_SDL> Game::GetTexture(const std::string& filename)
{
	auto iter = impl->manager.GetAsset(filename);
	if (iter->GetResourceType() == ResourceType::Texture)
	{
		return std::dynamic_pointer_cast<ATexture_SDL>(iter->GetResource());
	}
	else
	{
		auto tex = std::make_shared<ATexture_SDL>(filename.c_str(), impl->renderer.get());
		impl->manager.AddAsset(filename, std::make_shared<Resource>(ResourceType::Texture, std::move(std::dynamic_pointer_cast<Asset>(tex))));
		return tex;
	}
}
