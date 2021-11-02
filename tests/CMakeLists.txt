include_directories (doctest)
include_directories (${BeatTrackingEvaluationToolbox_SOURCE_DIR}/src)

add_executable (Tests 
    main.cpp 
    TestSignals.cpp
    Test_FMeasure.cpp
    Test_CemgilAccuracy.cpp
    Test_PScore.cpp
    Test_Continuity.cpp
    )

target_link_libraries (Tests BeatTrackingEvaluationToolbox)
target_compile_features (Tests PRIVATE cxx_std_17)
add_test (NAME Tests COMMAND Tests)