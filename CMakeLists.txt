project(Neuron_Test)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})

add_executable(Neurons
	Main.cpp
	Neuron.cpp
)
 
add_executable(Neuron_unittest Neuron.cpp Neuron_unittest.cpp)

target_link_libraries(Neuron_unittest gtest gtest_main)
add_test(Neuron_unittest Neuron_unittest)
