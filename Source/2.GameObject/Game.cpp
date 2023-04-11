#include "Game.h"
#include "Actor.h"
#include "Component.h"

#include <iostream>

Game::Game()
	: mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mTicksCount(0), mUpdatingActors(false)
{
}

Game::~Game()
{
	std::cerr << "~Game()" << std::endl;
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
		"GameObject", // 标题
		100,		  // 窗体左上角的 x 坐标
		100,		  // 窗体左上角的 y 坐标
		1024,		  // 窗体宽度
		768,		  // 窗体高度
		0			  // 标志位
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

	return true;
}

void Game::Shutdown()
{
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
}

void Game::GenerateOutput()
{
	// 设置 Tiffany 蓝
	SDL_SetRenderDrawColor(
		mRenderer,
		129, // R
		216, // G
		209, // B
		255	 // A
	);
	// 清理后缓冲区
	SDL_RenderClear(mRenderer);

	// 设置绘制颜色
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	// 交换前后缓冲区
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