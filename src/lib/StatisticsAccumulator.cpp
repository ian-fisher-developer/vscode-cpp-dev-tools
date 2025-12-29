#include <algorithm>
#include <cmath>
#include <limits>

#include "stats/StatisticsAccumulator.hpp"
#include "stats/StatisticsUtilities.hpp"

namespace stats
{

StatisticsAccumulator::StatisticsAccumulator()
    : count_(0)
    , minimum_(std::numeric_limits<float>::max())
    , maximum_(-std::numeric_limits<float>::max())
    , moment1_(0.0)
    , abs_moment1_(0.0)
    , moment2_(0.0)
    , moment3_(0.0)
    , moment4_(0.0)
{
}

void StatisticsAccumulator::add(const float& value)
{
    ++count_;

    minimum_ = std::min(value, minimum_);
    maximum_ = std::max(value, maximum_);

    const double dval     = static_cast<double>(value);
    const double nvals    = static_cast<double>(count_);
    const double delta    = dval - moment1_;
    const double delta_n  = delta / nvals;
    const double delta_n2 = delta_n * delta_n;
    const double term1    = delta * delta_n * (nvals - 1);
    moment1_ += delta_n;
    abs_moment1_ += (fabs(dval) - abs_moment1_) / nvals;
    moment4_ += term1 * delta_n2 * (nvals * nvals - 3 * nvals + 3) + 6.0 * delta_n2 * moment2_ -
                4.0 * delta_n * moment3_;
    moment3_ += term1 * delta_n * (nvals - 2) - 3.0 * delta_n * moment2_;
    moment2_ += term1;
}

size_t StatisticsAccumulator::count() const
{
    return count_;
}

float StatisticsAccumulator::minimum() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    return minimum_;
}

float StatisticsAccumulator::maximum() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    return maximum_;
}

float StatisticsAccumulator::mean() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    return static_cast<float>(moment1_);
}

float StatisticsAccumulator::absolute_mean() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    return static_cast<float>(abs_moment1_);
}

float StatisticsAccumulator::quadratic_mean() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    const double mean2 = moment1_ * moment1_;
    const double nvals = static_cast<double>(count_);
    const double sd2   = moment2_ / nvals;
    const double rms2  = mean2 + sd2;
    const double rms   = sqrt(rms2);
    return static_cast<float>(rms);
}

float StatisticsAccumulator::standard_deviation() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    const double nvals   = static_cast<double>(count_);
    const double std_dev = sqrt(moment2_ / nvals);
    return static_cast<float>(std_dev);
}

float StatisticsAccumulator::skewness() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    if (moment2_ == 0.0)
    {
        return stats::undefined();
    }

    const double nvals = static_cast<double>(count_);
    const double skew  = (sqrt(nvals) * moment3_) / pow(moment2_, 1.5);
    return static_cast<float>(skew);
}

float StatisticsAccumulator::kurtosis() const
{
    if (count_ == 0)
    {
        return stats::undefined();
    }

    if (moment2_ == 0.0)
    {
        return stats::undefined();
    }

    const double nvals = static_cast<double>(count_);
    const double kurt  = (nvals * moment4_) / (moment2_ * moment2_) - 3.0;
    return static_cast<float>(kurt);
}

StatisticsAccumulator StatisticsAccumulator::operator+(const StatisticsAccumulator& that) const
{
    if (this->count_ == 0)
    {
        return that;
    }

    if (that.count_ == 0)
    {
        return *this;
    }

    const double a_n = static_cast<double>(this->count_);
    const double b_n = static_cast<double>(that.count_);
    const double c_n = static_cast<double>(this->count_ + that.count_);

    const double& a_m1(this->moment1_);
    const double& a_abs_m1(this->abs_moment1_);
    const double& a_m2(this->moment2_);
    const double& a_m3(this->moment3_);
    const double& a_m4(this->moment4_);

    const double& b_m1(that.moment1_);
    const double& b_abs_m1(that.abs_moment1_);
    const double& b_m2(that.moment2_);
    const double& b_m3(that.moment3_);
    const double& b_m4(that.moment4_);

    const double delta  = b_m1 - a_m1;
    const double delta2 = delta * delta;
    const double delta3 = delta * delta2;
    const double delta4 = delta2 * delta2;

    StatisticsAccumulator combined;

    combined.count_   = this->count_ + that.count_;
    combined.minimum_ = std::min(this->minimum_, that.minimum_);
    combined.maximum_ = std::max(this->maximum_, that.maximum_);

    combined.moment1_ = (a_n * a_m1 + b_n * b_m1) / c_n;

    combined.abs_moment1_ = (a_n * a_abs_m1 + b_n * b_abs_m1) / c_n;

    combined.moment2_ = a_m2 + b_m2 + delta2 * a_n * b_n / c_n;

    combined.moment3_ = a_m3 + b_m3 + delta3 * a_n * b_n * (a_n - b_n) / (c_n * c_n);
    combined.moment3_ += 3.0 * delta * (a_n * b_m2 - b_n * a_m2) / c_n;

    combined.moment4_ =
        a_m4 + b_m4 + delta4 * a_n * b_n * (a_n * a_n - a_n * b_n + b_n * b_n) / (c_n * c_n * c_n);
    combined.moment4_ += 6.0 * delta2 * (a_n * a_n * b_m2 + b_n * b_n * a_m2) / (c_n * c_n) +
                         4.0 * delta * (a_n * b_m3 - b_n * a_m3) / c_n;

    return combined;
}

StatisticsAccumulator& StatisticsAccumulator::operator+=(const StatisticsAccumulator& rhs)
{
    StatisticsAccumulator combined = *this + rhs;
    *this                          = combined;
    return *this;
}

} // namespace stats
