// Game.h
#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <SDL.h>

#include "Math.hpp"

class Actor;
class SpriteComponent;

// Game class
class Game
{
public:
	Game();

	~Game();

	// 初始化游戏
	bool Initialize();
	// 运行游戏循环直到游戏结束
	void RunLoop();
	// 关闭游戏
	void Shutdown();

public:
	void AddActor(std::shared_ptr<Actor> &&actor);

	void RemoveActor(std::shared_ptr<Actor> &&actor);

	void AddSprite(std::shared_ptr<SpriteComponent> &&sprite);

	void RemoveSprite(std::shared_ptr<SpriteComponent> &&sprite);

	SDL_Texture *GetTexture(const std::string &fileName);

private:
	void LoadData();

	void UnloadData();

	SDL_Texture *LoadTexture(const char *fileName);

	// 处理进程输入
	void ProcessInput();
	// 更新游戏
	void UpdateGame();
	// 生成输出
	void GenerateOutput();

	// 通过 SDL 创建窗体
	SDL_Window *mWindow;
	// 渲染器
	SDL_Renderer *mRenderer;
	// 继续运行
	bool mIsRunning;

	// 记录运行时间
	Uint32 mTicksCount;

	std::vector<std::shared_ptr<Actor>> mActors;
	std::vector<std::shared_ptr<Actor>> mPendingActors;
	std::vector<std::shared_ptr<SpriteComponent>> mSprites;

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	bool mUpdatingActors;

	std::shared_ptr<class Ship> mShip;
};
