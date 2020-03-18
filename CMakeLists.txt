    cmake_minimum_required(VERSION 3.5)
project (array-bench)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}  -O3")

add_executable(main ${CMAKE_SOURCE_DIR}/array.cpp)

enable_testing()
find_package(gflags CONFIG REQUIRED)
target_link_libraries(main PRIVATE gflags_static)




find_package(GTest CONFIG REQUIRED)
target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(AllTestsInMain main)
find_package(benchmark CONFIG REQUIRED)
    target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)


add_executable(roofline ${CMAKE_SOURCE_DIR}/roofline.cpp)

enable_testing()
find_package(gflags CONFIG REQUIRED)
target_link_libraries(roofline PRIVATE gflags_static)


find_package(GTest CONFIG REQUIRED)
target_link_libraries(roofline PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(AllTestsInMain roofline)
find_package(benchmark CONFIG REQUIRED)
target_link_libraries(roofline PRIVATE benchmark::benchmark benchmark::benchmark_main)
