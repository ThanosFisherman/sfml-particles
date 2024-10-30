//
// Created by thanos on 28/10/2024.
//
#ifndef FRAMERATE_HPP
#define FRAMERATE_HPP
#include <SFML/System/Clock.hpp>

class Framerate
{
public:
    /// @brief Constructor with initialization.
    ///
    Framerate() : mFrame(0), mFps(0) {}

private:
    unsigned int mFrame;
    unsigned int mFps;
    sf::Clock mClock{};

public:
    void update()
    {
        if(mClock.getElapsedTime().asSeconds() >= 1.f)
        {
            mFps = mFrame;
            mFrame = 0;
            mClock.restart();
        }

        ++mFrame;
    }

    /// @brief Get the current FPS count.
    /// @return FPS count.
    [[nodiscard]] unsigned int getFPS() const { return mFps; }
};

#endif // FRAMERATE_HPP
