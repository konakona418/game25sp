cmake_minimum_required(VERSION 3.10.0)
project(game25sp VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendors/SFML)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendors/entt)

include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(game25sp main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics EnTT::EnTT)

