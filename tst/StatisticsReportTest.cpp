#include "stats/StatisticsReport.hpp"

#include <gtest/gtest.h>

#include "stats/StatisticsAccumulator.hpp"
#include "test_data/DocumentedTestSet.hpp"

TEST(StatisticsReport, BehavesWellWithNoValues)
{
    stats::StatisticsAccumulator statistics;

    EXPECT_EQ("No Values", stats::description(statistics));
}

TEST(StatisticsReport, BehavesWellWithOneValue)
{
    stats::StatisticsAccumulator statistics;

    float value(123.4F);
    statistics.add(value);

    EXPECT_EQ("1 Value\n Value = 123.4", stats::description(statistics));
}

TEST(StatisticsReport, BehavesWellWithConstantValues)
{
    stats::StatisticsAccumulator statistics;

    for (size_t i = 0; i < 10; ++i)
    {
        statistics.add(2);
    }

    EXPECT_EQ("10 Values\n Minimum  = 2\n Maximum  = 2\n Mean     = 2\n Abs.Mean = 2\n Rms      = "
              "2\n Std.Devn = 0",
              stats::description(statistics));
}

TEST(StatisticsReport, AgreesWithDocumentedExample)
{
    stats::StatisticsAccumulator statistics;

    for (size_t i = 0; i < documented_test_set::count(); ++i)
    {
        statistics.add(documented_test_set::values()[i]);
    }

    EXPECT_EQ(documented_test_set::statistics_description(), stats::description(statistics));
}
