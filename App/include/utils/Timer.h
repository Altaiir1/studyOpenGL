#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <utility>

namespace utils
{
    class Timer
    {
    private:
        std::chrono::high_resolution_clock::time_point start;
        std::string name;

    public:
        explicit Timer(std::string name) : start(std::chrono::high_resolution_clock::now()), name(std::move(name)) {}

        ~Timer()
        {
            const auto end = std::chrono::high_resolution_clock::now();
            const auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            const auto durationInSec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            const auto durationInMin = std::chrono::duration_cast<std::chrono::minutes>(end - start);
            std::cout << name << ": " << durationInMs.count() << " ms" << '\n';
            std::cout << name << ": " << durationInSec.count() << " s" << '\n';
            std::cout << name << ": " << durationInMin.count() << " m\n" << '\n';
        }

        // Prevent copying
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
    };
}