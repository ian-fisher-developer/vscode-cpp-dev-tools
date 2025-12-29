#include <gtest/gtest.h>
#include <limits>

#include "stats/StatisticsAccumulator.hpp"
#include "stats/StatisticsUtilities.hpp"
#include "test_data/DocumentedTestSet.hpp"

namespace
{ // unnamed namespace

void test_equivalence(const stats::StatisticsAccumulator& expected,
                      const stats::StatisticsAccumulator& actual)
{
    EXPECT_EQ(expected.count(), actual.count());
    EXPECT_EQ(expected.minimum(), actual.minimum());
    EXPECT_EQ(expected.maximum(), actual.maximum());
    EXPECT_EQ(expected.mean(), actual.mean());
    EXPECT_EQ(expected.absolute_mean(), actual.absolute_mean());
    EXPECT_EQ(expected.quadratic_mean(), actual.quadratic_mean());
    EXPECT_EQ(expected.standard_deviation(), actual.standard_deviation());
    EXPECT_EQ(expected.skewness(), actual.skewness());
    EXPECT_EQ(expected.kurtosis(), actual.kurtosis());
}

} // unnamed namespace

TEST(StatisticsAccumulator, BehavesWellWithNoValues)
{
    stats::StatisticsAccumulator statistics;

    EXPECT_EQ(0U, statistics.count());
    EXPECT_TRUE(stats::undefined(statistics.minimum()));
    EXPECT_TRUE(stats::undefined(statistics.maximum()));
    EXPECT_TRUE(stats::undefined(statistics.mean()));
    EXPECT_TRUE(stats::undefined(statistics.absolute_mean()));
    EXPECT_TRUE(stats::undefined(statistics.quadratic_mean()));
    EXPECT_TRUE(stats::undefined(statistics.standard_deviation()));
    EXPECT_TRUE(stats::undefined(statistics.skewness()));
    EXPECT_TRUE(stats::undefined(statistics.kurtosis()));
}

TEST(StatisticsAccumulator, BehavesWellWithOneValue)
{
    stats::StatisticsAccumulator statistics;

    float value(123.4F);
    statistics.add(value);

    EXPECT_EQ(1U, statistics.count());
    EXPECT_EQ(value, statistics.minimum());
    EXPECT_EQ(value, statistics.maximum());
    EXPECT_EQ(value, statistics.mean());
    EXPECT_EQ(value, statistics.absolute_mean());
    EXPECT_EQ(value, statistics.quadratic_mean());
    EXPECT_EQ(0.F, statistics.standard_deviation());
    EXPECT_TRUE(stats::undefined(statistics.skewness()));
    EXPECT_TRUE(stats::undefined(statistics.kurtosis()));
}

TEST(StatisticsAccumulator, BehavesWellWithConstantValues)
{
    stats::StatisticsAccumulator statistics;

    float value(234.5F);
    std::vector<float> test_set(50, value);
    for (const float& value : test_set)
    {
        statistics.add(value);
    }

    EXPECT_EQ(test_set.size(), statistics.count());
    EXPECT_EQ(value, statistics.minimum());
    EXPECT_EQ(value, statistics.maximum());
    EXPECT_EQ(value, statistics.mean());
    EXPECT_EQ(value, statistics.absolute_mean());
    EXPECT_EQ(value, statistics.quadratic_mean());
    EXPECT_EQ(0.F, statistics.standard_deviation());
    EXPECT_TRUE(stats::undefined(statistics.skewness()));
    EXPECT_TRUE(stats::undefined(statistics.kurtosis()));
}

TEST(StatisticsAccumulator, WorksWithNegativeValues)
{
    stats::StatisticsAccumulator statistics;

    statistics.add(-6.F);
    statistics.add(-2.F);
    statistics.add(-1.F);

    EXPECT_EQ(3, statistics.count());
    EXPECT_EQ(-6.F, statistics.minimum());
    EXPECT_EQ(-1.F, statistics.maximum());
    EXPECT_EQ(-3.F, statistics.mean());
    EXPECT_EQ(+3.F, statistics.absolute_mean());

    double mean_square      = (-6.0 * -6.0 + -2.0 * -2.0 + -1.0 * -1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, statistics.quadratic_mean());

    double variance = (pow(-6.0 - -3.0, 2) + pow(-2.0 - -3.0, 2) + pow(-1.0 - -3.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, statistics.standard_deviation());
}

TEST(StatisticsAccumulator, WorksWithPositiveValues)
{
    stats::StatisticsAccumulator statistics;

    statistics.add(6.F);
    statistics.add(2.F);
    statistics.add(1.F);

    EXPECT_EQ(3, statistics.count());
    EXPECT_EQ(1.F, statistics.minimum());
    EXPECT_EQ(6.F, statistics.maximum());
    EXPECT_EQ(3.F, statistics.mean());
    EXPECT_EQ(3.F, statistics.absolute_mean());

    double mean_square      = (6.0 * 6.0 + 2.0 * 2.0 + 1.0 * 1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, statistics.quadratic_mean());

    double variance           = (pow(6.0 - 3.0, 2) + pow(2.0 - 3.0, 2) + pow(1.0 - 3.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, statistics.standard_deviation());
}

TEST(StatisticsAccumulator, WorksWithZeroCenteredValues)
{
    stats::StatisticsAccumulator statistics;

    statistics.add(-1);
    statistics.add(0);
    statistics.add(+1);

    EXPECT_EQ(3, statistics.count());
    EXPECT_EQ(-1.F, statistics.minimum());
    EXPECT_EQ(+1.F, statistics.maximum());
    EXPECT_EQ(0.F, statistics.mean());
    EXPECT_EQ(2.F / 3, statistics.absolute_mean());

    double mean_square      = (-1.0 * -1.0 + 0.0 * 0.0 + 1.0 * 1.0) / 3;
    double root_mean_square = sqrt(mean_square);
    EXPECT_FLOAT_EQ(root_mean_square, statistics.quadratic_mean());

    double variance           = (pow(-1.0 - 0.0, 2) + pow(0.0 - 0.0, 2) + pow(1.0 - 0.0, 2)) / 3;
    double standard_deviation = sqrt(variance);
    EXPECT_FLOAT_EQ(standard_deviation, statistics.standard_deviation());
}

TEST(StatisticsAccumulator, WorksAtMinimumBoundary)
{
    stats::StatisticsAccumulator statistics;

    statistics.add(-std::numeric_limits<float>::max());

    EXPECT_EQ(-std::numeric_limits<float>::max(), statistics.minimum());
    EXPECT_EQ(-std::numeric_limits<float>::max(), statistics.maximum());
}

TEST(StatisticsAccumulator, WorksAtMaximumBoundary)
{
    stats::StatisticsAccumulator statistics;

    statistics.add(std::numeric_limits<float>::max());

    EXPECT_EQ(std::numeric_limits<float>::max(), statistics.minimum());
    EXPECT_EQ(std::numeric_limits<float>::max(), statistics.maximum());
}

TEST(StatisticsAccumulator, AgreesWithDocumentedExample)
{
    stats::StatisticsAccumulator statistics;

    for (const float& value : documented_test_set::values())
    {
        statistics.add(value);
    }

    EXPECT_EQ(documented_test_set::count(), statistics.count());
    EXPECT_EQ(documented_test_set::minimum(), statistics.minimum());
    EXPECT_EQ(documented_test_set::maximum(), statistics.maximum());
    EXPECT_EQ(documented_test_set::mean(), statistics.mean());
    EXPECT_EQ(documented_test_set::absolute_mean(), statistics.absolute_mean());
    EXPECT_EQ(documented_test_set::quadratic_mean(), statistics.quadratic_mean());
    EXPECT_EQ(documented_test_set::standard_deviation(), statistics.standard_deviation());
    EXPECT_EQ(documented_test_set::skewness(), statistics.skewness());
    EXPECT_EQ(documented_test_set::kurtosis(), statistics.kurtosis());
}

TEST(StatisticsAccumulator, HandlesSelfAssignment)
{
    stats::StatisticsAccumulator statistics;

    const stats::StatisticsAccumulator& statistics2(statistics);

    statistics = statistics2;

    EXPECT_EQ(&statistics, &statistics2);
}

TEST(StatisticsAccumulator, CombinesResultsFromEmptyAccumulators)
{
    stats::StatisticsAccumulator fullset;
    stats::StatisticsAccumulator subset1, subset2, subset3;

    stats::StatisticsAccumulator combined = subset1 + subset2 + subset3;

    test_equivalence(fullset, combined);
}

TEST(StatisticsAccumulator, CombinesResultsFromSomeEmptyAccumulators)
{
    stats::StatisticsAccumulator fullset;

    stats::StatisticsAccumulator subset1;

    stats::StatisticsAccumulator subset2;
    subset2.add(1.F);
    fullset.add(1.F);
    subset2.add(1.5F);
    fullset.add(1.5F);
    subset2.add(2.F);
    fullset.add(2.F);

    stats::StatisticsAccumulator subset3;

    stats::StatisticsAccumulator subset4;
    subset2.add(3.F);
    fullset.add(3.F);
    subset2.add(4.F);
    fullset.add(4.F);

    stats::StatisticsAccumulator combined = subset1 + subset2 + subset3 + subset4;

    test_equivalence(fullset, combined);
}

TEST(StatisticsAccumulator, CombinesResultsFromSingleValueAccumulators)
{
    stats::StatisticsAccumulator fullset;

    stats::StatisticsAccumulator statistics1;
    statistics1.add(1.F);
    fullset.add(1.F);

    stats::StatisticsAccumulator statistics2;
    statistics2.add(2.F);
    fullset.add(2.F);

    stats::StatisticsAccumulator combined = statistics1 + statistics2;

    test_equivalence(fullset, combined);
}

TEST(StatisticsAccumulator, CombinesResultsFromMultipleAccumulators)
{
    stats::StatisticsAccumulator fullset;
    stats::StatisticsAccumulator subset1;
    stats::StatisticsAccumulator subset2;

    int count = 0;
    for (const float& value : documented_test_set::values())
    {
        fullset.add(value);
        if ((++count % 3) == 0)
        {
            subset1.add(value);
        }
        else
        {
            subset2.add(value);
        }
    }

    stats::StatisticsAccumulator combined1 = subset1 + subset2;
    test_equivalence(fullset, combined1);

    stats::StatisticsAccumulator combined2 = subset2 + subset1;
    test_equivalence(fullset, combined2);
}

TEST(StatisticsAccumulator, IncrementsResultsFromAnotherAccumulator)
{
    stats::StatisticsAccumulator fullset;
    stats::StatisticsAccumulator subset1;
    stats::StatisticsAccumulator subset2;

    int count = 0;
    for (const float& value : documented_test_set::values())
    {
        fullset.add(value);
        if ((++count % 3) == 0)
        {
            subset1.add(value);
        }
        else
        {
            subset2.add(value);
        }
    }

    stats::StatisticsAccumulator combined;
    combined += subset1;
    combined += subset2;

    test_equivalence(fullset, combined);
}
