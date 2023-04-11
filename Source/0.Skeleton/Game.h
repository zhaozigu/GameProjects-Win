// Game.h
#pragma once
#include <SDL.h>

class Game
{
public:
	Game();
	// 初始化游戏
	bool Initialize();
	// 运行游戏循环直到游戏结束
	void RunLoop();
	// 关闭游戏
	void Shutdown();
private:
	// 处理进程输入
	void ProcessInput();
	// 更新游戏
	void UpdateGame();
	// 生成输出
	void GenerateOutput();

	// 通过 SDL 创建窗体
	SDL_Window* mWindow;

	// 继续运行
	bool mIsRunning;
};
