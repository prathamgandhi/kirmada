file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.cc")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${SRC_DIR})

add_executable(kirmada ${SOURCE_FILES})