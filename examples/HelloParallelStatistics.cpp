#include <array>
#include <iostream>

#include "ParallelStatistics.hpp"
#include "stats/StatisticsReport.hpp"

int main(int /*unused*/, char** /*unused*/)
{
    std::array<float, 3> values = {1.0, 2.0, 3.0};

    stats::StatisticsAccumulator statistics =
        parallel_statistics::run(values.data(), values.size());

    std::cout << stats::description(statistics) << std::endl;
}
