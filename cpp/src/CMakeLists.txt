include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library (
    BeatTrackingEvaluationToolbox STATIC
    BeatTrackingEvaluationToolbox.cpp
    BeatTrackingEvaluationToolbox.h
)

source_group (Source src)