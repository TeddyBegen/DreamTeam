cmake_minimum_required(VERSION 3.15.0)
project(dream-team-agent VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)

set(TARGET_NAME agent)

set(TARGET_SOURCE_FILES

    "node.hpp"
    "node.cpp"

    "state.hpp"
    "state.cpp"

    "information.hpp"
    "information.cpp"

    "action.hpp"
    "action.cpp"

    "comms.hpp"
    "comms.cpp"

)

add_library(${TARGET_NAME} OBJECT ${TARGET_SOURCE_FILES})

add_executable(dream-team-agent main.cpp)

target_link_libraries(dream-team-agent ${TARGET_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
