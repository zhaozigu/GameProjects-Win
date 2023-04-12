#include "Game.h"
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "SDL_image.h"
#include "Ship.h"

Game::Game()
	: mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mTicksCount(0), mUpdatingActors(false)
{
}

Game::~Game()
{
#ifdef _DEBUG
	SDL_Log("~Game()");
#endif // _DEBUG
}

bool Game::Initialize()
{
	// 初始化 SDL 库
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("不能初始化 SDL: %s", SDL_GetError());
		return false;
	}

	// 创建 SDL 窗体
	mWindow = SDL_CreateWindow(
		"Sprites", // 标题
		100,	   // 窗体左上角的 x 坐标
		100,	   // 窗体左上角的 y 坐标
		1024,	   // 窗体宽度
		768,	   // 窗体高度
		0		   // 标志位
	);

	if (!mWindow)
	{
		SDL_Log("创建窗体失败: %s", SDL_GetError());
		return false;
	}

	// 创建渲染器
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("创建渲染器失败: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("不能初始化SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	mTicksCount = SDL_GetTicks();
	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::LoadData()
{
	mShip = std::make_shared<Ship>();
	mShip->Initialize(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	std::shared_ptr<Actor> temp = std::make_shared<Actor>();
	temp->Initialize(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	std::shared_ptr<class BGSpriteComponent> bg = std::make_shared<BGSpriteComponent>(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture *> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	bg->AddComponent();

	// 创建一个更近的背景
	bg = std::make_shared<BGSpriteComponent>(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
	bg->AddComponent();
}

void Game::UnloadData()
{
	// 销毁texture
	for (auto &i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture *Game::LoadTexture(const char *fileName)
{
	// 从文件中加载
	SDL_Surface *surf = IMG_Load(fileName);

	if (!surf)
	{
		SDL_Log("加载图像文件 %s 失败", fileName);
		return nullptr;
	}

	// 从 surface 创建 texture
	SDL_Texture *tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("%s surface 转换到 texture 失败!", fileName);
		return nullptr;
	}
	return tex;
}

void Game::ProcessInput()
{
	SDL_Event event;

	// 有 event 在队列就一直循环
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	// 获取键盘的状态
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// 如果按了 Esc，结束循环
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// 处理飞船的输入
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// 等到与上一帧间隔 16ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// 增量时间是上一帧到现在的时间差
	// (转换成秒)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// 更新运行时间(为下一帧)
	mTicksCount = SDL_GetTicks();
	// 固定增量时间最大值
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	// 更新所有actor
	mUpdatingActors = true;
	for (auto &actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 移动待定actor到mActors
	for (auto &pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 添加 dead actor 到临时向量
	std::vector<std::shared_ptr<Actor>> deadActors;
	for (auto &actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// 绘制所有精灵组件
	for (auto &sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(std::shared_ptr<Actor> &&actor)
{
	// 如果正在更新actor，就添加到待处理列表里
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(std::shared_ptr<Actor> &&actor)
{
	// 是否在待定actor中
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 交换到尾部（避免复制)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// 是否在 actor中
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 交换到尾部（避免复制)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(std::shared_ptr<SpriteComponent> &&sprite)
{
	// 在有序向量中找到插入点
	// (第一个比传入的sprite的order顺序大的元素)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		 iter != mSprites.end();
		 ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// 在迭代器之前插入
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(std::shared_ptr<SpriteComponent> &&sprite)
{
	// (不能交换，不然顺序就没了)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture *Game::GetTexture(const std::string &fileName)
{
	SDL_Texture *tex = nullptr;
	// texture是否已经存在？
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// 从文件中加载
		SDL_Surface *surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("加载 texture 文件 %s 失败", fileName.c_str());
			return nullptr;
		}

		// 从 surface 中创建 textures
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("无法把 %s 从surface转化到texture", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
