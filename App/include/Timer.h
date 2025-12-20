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
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << name << ": " << duration.count() << " ms\n" << '\n';
        }

        // Prevent copying
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
    };
}