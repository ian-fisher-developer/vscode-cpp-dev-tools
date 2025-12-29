add_library(${PROJECT_NAME} STATIC)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

target_sources(
    ${PROJECT_NAME}
    PRIVATE headers/stats/StatisticsAccumulator.hpp
            headers/stats/StatisticsReport.hpp
            headers/stats/StatisticsUtilities.hpp
            lib/StatisticsAccumulator.cpp
            lib/StatisticsReport.cpp
            lib/StatisticsReportsHelpers.cpp
            lib/StatisticsReportsHelpers.hpp
            lib/StatisticsUtilities.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC headers)

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
