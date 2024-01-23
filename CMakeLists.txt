cmake_minimum_required(VERSION 3.28.1)

#set(CMAKE_CXX_STANDARD 20)

project(CG_Arena LANGUAGES CXX)

add_subdirectory(CG_Game/src)
#add_subdirectory(CG_Cockpit/src)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/application.cpp 
)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME}
    PUBLIC "CG_Game/src"
    #PUBLIC "CG_Cockpit/src"
)

target_link_directories(${PROJECT_NAME}
    PRIVATE CG_Game/src
    #PRIVATE CG_Cockpit
)

target_link_libraries(${PROJECT_NAME}
    CG_Game
    #CG_Cockpit
)
