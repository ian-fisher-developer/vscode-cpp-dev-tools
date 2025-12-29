#include <gtest/gtest.h>

#include "stats/StatisticsAccumulator.hpp"
#include "stats/StatisticsReport.hpp"
#include "test_data/DocumentedTestSet.hpp"
#include "test_data/StressData.hpp"

// Note that each of these tests takes many minutes to run. They are gathered
// in their own test program to avoid slowing the regular unit test program.

TEST(Statistics, Stress_CountsPast32BitLimits)
{
    stats::StatisticsAccumulator statistics;
    uint64_t large_number_counter(0);
    while (large_number_counter < kFiveBillion)
    {
        statistics.add(1.5F);
        ++large_number_counter;
    }
    EXPECT_EQ(large_number_counter, statistics.count());
    EXPECT_EQ("5000000000 Values\n Minimum  = 1.5\n Maximum  = 1.5\n Mean     = 1.5\n Abs.Mean = "
              "1.5\n Rms      = 1.5\n Std.Devn = 0",
              stats::description(statistics));
}

TEST(Statistics, Stress_CountsPast32BitLimitsWithTwoAccumulators)
{
    const float value(1.5F);
    uint64_t large_number_counter(0);

    stats::StatisticsAccumulator statistics1;
    while (large_number_counter < kTwoBillion)
    {
        statistics1.add(value);
        ++large_number_counter;
    }

    stats::StatisticsAccumulator statistics2;
    while (large_number_counter < kFiveBillion)
    {
        statistics2.add(value);
        ++large_number_counter;
    }

    EXPECT_EQ(kTwoBillion, statistics1.count());
    EXPECT_EQ(kThreeBillion, statistics2.count());

    stats::StatisticsAccumulator combined = statistics1 + statistics2;
    EXPECT_EQ(kFiveBillion, combined.count());
    EXPECT_EQ("5000000000 Values\n Minimum  = 1.5\n Maximum  = 1.5\n Mean     = 1.5\n Abs.Mean = "
              "1.5\n Rms      = 1.5\n Std.Devn = 0",
              stats::description(combined));
}
