    cmake_minimum_required(VERSION 3.5)
project (array-bench)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Ofast -std=c++11")

set(BENCHMARKS array  roofline cacheline cache)

foreach( exe ${BENCHMARKS} )
    
    add_executable(${exe} ${CMAKE_SOURCE_DIR}/${exe}.cpp)

    enable_testing()
    find_package(gflags CONFIG REQUIRED)
    target_link_libraries(${exe} PRIVATE gflags_static)


    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(${exe} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    add_test(AllTestsInMain roofline)
    find_package(benchmark CONFIG REQUIRED)
    target_link_libraries(${exe} PRIVATE benchmark::benchmark benchmark::benchmark_main)

endforeach()
