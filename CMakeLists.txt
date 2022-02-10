cmake_minimum_required(VERSION 3.12...3.22)

project(
  "Advent of Code"
  LANGUAGES "C" "CXX"
  VERSION "2021.0.0"
  DESCRIPTION "Advent of Code 2021"
  HOMEPAGE_URL "https://github.com/Phytolizer/aoc-2021"
)
set(PROJECT_LICENSE "MIT")
set(PROJECT_AUTHOR "Kyle Coffey")
set(PROJECT_AUTHOR_EMAIL "me@phytolizer.dev")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake Modules")
include(PushDirectory)

add_subdirectory("C")
add_subdirectory("C2")
