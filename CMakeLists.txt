project(Neuron_Test)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "-W -Wall -pedantic -std=c++11")
add_executable(Neurons
	Main.cpp
	Neuron.cpp
)
