#pragma once

#include <cstddef>

namespace stats
{

/**
 * Takes one value at a time, providing accumulated descriptive statistics.
 *
 * StatisticsAccumulator accepts 32-bit floating point values with add().
 * It provides measures of count, minimum, maximum, mean, absolute mean,
 * quadratic mean (rms), standard deviation, skewness, and kurtosis.
 *
 * Use the accumulator with code like the following.

 \code
 #include <stats/StatisticsAccumulator.h>


 // create a statistics accumulator

 stats::StatisticsAccumulator statistics;


 // add some values

 statistics.add( -2.0 );
 statistics.add(  0.0 );
 statistics.add( +2.0 );
 statistics.add( +4.0 );

 // access the measures

 size_t n = statistics.count(); // sets n to 4
 float  u = statistics.mean();  // sets u to 1.0;
 \endcode

 * Be sure to watch for undefined measures.

 \code
 if( stats::undefined( statistics.mean() ) ){
     // no values -- ignore the undefined mean
 }
 if( stats::undefined( statistics.skewness() ) ){
     // all values are the same -- ignore the undefined skewness
 }
 \endcode

 * The accumulator does not store the values. It allows statistics of
 * larga data.
 *
 * The accumulation algorithm is stable. Rounding errors accumulate slowly.
 * Stress tests find 32-bit floating point deviations for large data:
 *
 * - standard deviation: approximately 134 billion values
 * - skewness: approximately 29 billion values
 * - kurtosis: approximately 2.6 billion values
 *
 * \sa
 * <a href="http://www.johndcook.com/blog/skewness_kurtosis/">
 * Computing skewness and kurtosis in one pass.
 * </a>
 * This class includes John D. Cook's skewness/kurtosis extension of the
 * method of Knuth and Welford for computing standard deviation in one pass
 * through the data.
 */

class StatisticsAccumulator
{
  private:
    std::size_t count_;
    float minimum_, maximum_;
    double moment1_, abs_moment1_, moment2_, moment3_, moment4_;

  public:
    StatisticsAccumulator();

    /**
     * Updates the accumulated statistics with the value.
     */
    void add(const float& value);

    /**
     * Returns the total number of values provided with add().
     */
    std::size_t count() const;

    /**
     * Returns the minimum of the values provided with add().
     */
    float minimum() const;

    /**
     * Returns the maximum of the values provided with add().
     */
    float maximum() const;

    /**
     * Returns the arithmetic mean of the values provided with add().
     */
    float mean() const;

    /**
     * Returns the mean of the absolute values provided with add().
     */
    float absolute_mean() const;

    /**
     * Returns the quadratic mean (rms) of the values provided with add().
     */
    float quadratic_mean() const;

    /**
     * Returns the standard deviation of the values provided with add().
     */
    float standard_deviation() const;

    /**
     * Returns the skewness of the values provided with add().
     *
     * The normal distribution's skewness is zero.
     */
    float skewness() const;

    /**
     * Returns the kurtosis of the values provided with add().
     *
     * The measure is technically "excess kurtosis", for which the normal
     * distribution is zero.
     */
    float kurtosis() const;

    /**
     * "Adds" accumulated statistics, aggregating the results.
     *
     * Use code like the following to combine accumulated statistics.

     \code
     #include <stats/StatisticsAccumulator.h>


     // make three accumulators

     StatisticsAccumulator accumulator1, accumulator2, accumulator3;


     // add values to each accumulator

     ...


     // and combine the results

     StatisticsAccumulator combined = accumulator1 + accumulator2 + accumulator3;
     \endcode

     */
    StatisticsAccumulator operator+(const StatisticsAccumulator& that) const;

    /**
     * "Adds" the specified accumulator to this one, aggregating the results.
     */
    StatisticsAccumulator& operator+=(const StatisticsAccumulator& rhs);
};

} // namespace stats
