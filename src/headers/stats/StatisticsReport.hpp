#pragma once

#include <string>

namespace stats
{

class StatisticsAccumulator;

/**
 * Returns a text description of the statistics, in a form suitable for
 * brief annotation.
 */
std::string description(const stats::StatisticsAccumulator&);

} // namespace stats
