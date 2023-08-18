cmake_minimum_required (VERSION 3.12)
project (BeatTrackingEvaluationToolbox VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 11)

#option (BUILD_TESTS "Build tests" OFF)

add_subdirectory (src)
enable_testing()
add_subdirectory (tests)

# if (BUILD_TESTS)
#     enable_testing()
#     add_subdirectory (tests)
# endif (BUILD_TESTS)

set (CMAKE_SUPPRESS_REGENERATION true)

