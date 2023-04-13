/// \file GameInterface.h
/// \brief Game interface class header file

#pragma once

/// \class GameInterface
/// \brief Game interface class
///
/// This is an abstract class that defines the basic interface for a game, including initialization, running the loop, and shutting down.
class GameInterface
{
public:
    /// \brief Constructor
    GameInterface(){};

    /// \brief Initialize the game
    /// \return Returns true if initialization is successful, false otherwise
    virtual bool Initialize() = 0;

    /// \brief Run the game loop until the game ends
    virtual void RunLoop() = 0;

    /// \brief Shut down the game
    virtual void Shutdown() = 0;

protected:
    /// \brief Process input
    /// This method should handle any input processing required by the game.
    virtual void ProcessInput() = 0;

    /// \brief Update the game
    /// This method should handle the game update logic.
    virtual void UpdateGame() = 0;

    /// \brief Generate output
    /// This method should handle generating the output (e.g., rendering) for the game.
    virtual void GenerateOutput() = 0;
};
