cmake_minimum_required(VERSION 4.1)
project(vk_explore CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-std=c++173 -O2 -g")

set(SOURCE_FILES
    main.cpp)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
