#ifndef TIMER_H
#define TIMER_H
#include <chrono>

template <typename F>
unsigned long long time_function(F f) {
    unsigned long long duration = 0;

    // start the timer
    auto start = std::chrono::high_resolution_clock::now();
    f();
    // stop the timer
    auto stop = std::chrono::high_resolution_clock::now();
    duration +=
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();

    // return the average time taken
    return duration;
}

#endif
