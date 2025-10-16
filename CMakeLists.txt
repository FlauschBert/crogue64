cmake_minimum_required(VERSION 4.1.2)
project(crogue64)
find_package(llvm-mos-sdk REQUIRED)
add_executable(crogue64 main.cpp)
