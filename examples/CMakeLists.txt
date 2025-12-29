add_executable(hello_statistics_example HelloStatistics.cpp)
target_link_libraries(hello_statistics_example PRIVATE ${PROJECT_NAME})
set_target_properties(
    hello_statistics_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_executable(parallel_statistics_example HelloParallelStatistics.cpp)
target_link_libraries(parallel_statistics_example PRIVATE ${PROJECT_NAME})
set_target_properties(
    parallel_statistics_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
