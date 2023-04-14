/// \file Game.h
/// \brief Game class header file

#pragma once
#include "GameInterface.hpp"

#include <string>
#include <memory>

/// \class Game
/// \brief Game class derived from GameInterface
///
/// This is a concrete implementation of the GameInterface class.
class Game final : public GameInterface
{
public:
	Game();

	~Game();

	/// \brief Initialize the game
	/// \return Returns true if initialization is successful, false otherwise
	/// \note Initialize("Game", 100, 100, 1024, 768);
	bool Initialize() override;

	/// \brief Initialize the game with custom window parameters
	/// \param windowsName The name of the window to be created
	/// \param x The X position of the window on the screen
	/// \param y The Y position of the window on the screen
	/// \param w The width of the window
	/// \param h The height of the window
	/// \return Returns true if initialization is successful, false otherwise
	bool Initialize(const std::string &windowsName, int x, int y, int w, int h);

	/// \brief Run the game loop until the game ends
	void RunLoop() override;

	/// \brief Shut down the game
	void Shutdown() override;

	void AddActor(SharedActor&& actor) override;

	void RemoveActor(SharedActor&& actor) override;

private:
	/// \brief Process input
	/// This method should handle any input processing required by the game.
	void ProcessInput() override;

	/// \brief Update the game
	/// This method should handle the game update logic.
	void UpdateGame() override;

	/// \brief Generate output
	/// This method should handle generating the output (e.g., rendering) for the game.
	void GenerateOutput() override;

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};
