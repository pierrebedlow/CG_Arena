cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_CXX_STANDARD 20)
set(project_name CG_Arena)

project(${project_name} LANGUAGES CXX)

add_executable(${project_name} "/home/pedro/dev/CG_Arena/CG_Arena/src/main.cpp")

target_include_directories(${project_name} PUBLIC "/home/pedro/dev/CG_Arena/CG_Game" "/home/pedro/dev/CG_Arena/CG_Arena/src")
