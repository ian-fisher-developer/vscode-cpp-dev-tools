#pragma once

// Example of a multi-threaded statistics accumulation.
//
// parallel_statistics::run() processes values using several accumulators, each
// in its own thread, and then combines the results in to the return object.
// Use the parallel solution with code like this:
//
//   stats::StatisticsAccumulator statistics = parallel_statistics::run( values, number_of_values );
//   float u = statistics.mean(); // sets `u` to the mean of the values
//
// This example is complete. Feel free to copy it to an application and change
// the details for the custom requirements.

#include <thread>
#include <vector>

#include "stats/StatisticsAccumulator.hpp"

namespace parallel_statistics
{

inline size_t number_of_threads_hint()
{
    size_t number_of_threads = std::thread::hardware_concurrency();
    if (number_of_threads == 0)
    {
        number_of_threads = 2; // set to 2, if not detected
    }
    return number_of_threads;
}

inline size_t optimal_number_of_threads(size_t number_of_values)
{
    size_t hint = number_of_threads_hint();
    size_t number_of_threads =
        number_of_values < hint ? number_of_values : hint; // watch for small number of values
    return number_of_threads;
}

inline void add_to_accumulator(stats::StatisticsAccumulator& accumulator, const float* const values,
                              const size_t number_of_values)
{
    for (size_t i = 0; i < number_of_values; ++i)
    {
        accumulator.add(values[i]);
    }
}

inline stats::StatisticsAccumulator run(const float* const values, size_t number_of_values)
{
    if (number_of_values == 0)
    {
        return stats::StatisticsAccumulator();
    }

    // initialize several accumulators
    size_t number_of_threads = optimal_number_of_threads(number_of_values);
    std::vector<stats::StatisticsAccumulator> accumulators(number_of_threads);

    // make a container for the spawned threads
    // (-1, because we already have this main thread)
    std::vector<std::thread> spawned_threads(number_of_threads - 1);

    // split the input into blocks and process each block in a spawned thread
    // (do not worry about the remainder for now)
    size_t block_size        = number_of_values / number_of_threads;
    const float* block_start = values;
    for (size_t i = 0; i < spawned_threads.size(); ++i)
    {
        spawned_threads[i] =
            std::thread(&add_to_accumulator, std::ref(accumulators[i]), block_start, block_size);
        block_start += block_size;
    }

    // process the remainder in this main thread
    size_t last_block_size = number_of_values - block_size * (number_of_threads - 1);
    add_to_accumulator(accumulators.back(), block_start, last_block_size);

    // wait for the spawned threads to complete
    for (auto& spawned_thread : spawned_threads)
    {
        spawned_thread.join();
    }

    // combine the results
    stats::StatisticsAccumulator combined;
    for (const auto& accumulator : accumulators)
    {
        combined += accumulator;
    }

    return combined;
}

} // namespace parallel_statistics
