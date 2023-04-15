/// \file Random.h
/// \brief Random number generator utility class header file

#pragma once

#include <random>
#include "Math.hpp"

/// \class Random
/// \brief Random number generator utility class
///
/// A utility class for generating random numbers with various requirements.
class Random
{
public:
    /// \brief Initialize the random number generator
    static void Init();

    /// \brief Seed the random number generator with a specific value
    /// \param seed The seed value
    /// \note It is recommended to generate the seed automatically instead of setting it manually.
    static void Seed(unsigned int seed);

    /// \brief Get a random float between 0.0f and 1.0f
    /// \return Random float between 0.0f and 1.0f
    static float GetFloat();

    /// \brief Get a random float within the specified range
    /// \param min Minimum value of the range
    /// \param max Maximum value of the range
    /// \return Random float within the given range
    static float GetFloatRange(float min, float max);

    /// \brief Get a random integer within the specified range
    /// \param min Minimum value of the range
    /// \param max Maximum value of the range
    /// \return Random integer within the given range
    static int GetIntRange(int min, int max);

    /// \brief Get a random 2D vector within the specified minimum/maximum range
    /// \param min Minimum values of the range (x, y)
    /// \param max Maximum values of the range (x, y)
    /// \return Random Vector2 within the given range
    static Vector2 GetVector(const Vector2 &min, const Vector2 &max);

    /// \brief Get a random 3D vector within the specified minimum/maximum range
    /// \param min Minimum values of the range (x, y, z)
    /// \param max Maximum values of the range (x, y, z)
    /// \return Random Vector3 within the given range
    static Vector3 GetVector(const Vector3 &min, const Vector3 &max);

private:
    /// \brief The random number generator used by the utility class
    static std::mt19937 sGenerator;
};
