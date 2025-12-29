#include "stats/StatisticsReport.hpp"

#include <sstream>

#include "StatisticsReportsHelpers.hpp"
#include "stats/StatisticsAccumulator.hpp"
#include "stats/StatisticsUtilities.hpp"

namespace stats
{

std::string description(const stats::StatisticsAccumulator &statistics)
{
    using namespace stats::detail;

    std::ostringstream oss;

    oss << count_description(statistics.count());

    if (statistics.count() == 0)
    {
        return oss.str();
    }

    if (statistics.count() == 1)
    {
        oss << std::endl << label_and_value(kValueLabel, statistics.minimum());
    }
    else
    {
        oss << std::endl << label_and_value(kMinimumLabel, statistics.minimum());
        oss << std::endl << label_and_value(kMaximumLabel, statistics.maximum());
        oss << std::endl << label_and_value(kMeanLabel, statistics.mean());
        oss << std::endl << label_and_value(kAbsMeanLabel, statistics.absolute_mean());
        oss << std::endl << label_and_value(kRmsLabel, statistics.quadratic_mean());
        oss << std::endl << label_and_value(kStdDevnLabel, statistics.standard_deviation());
        if (!stats::undefined(statistics.skewness()))
        {
            oss << std::endl << label_and_value(kSkewnessLabel, statistics.skewness());
        }
        if (!stats::undefined(statistics.kurtosis()))
        {
            oss << std::endl << label_and_value(kKurtosisLabel, statistics.kurtosis());
        }
    }

    return oss.str();
}

} // namespace stats
