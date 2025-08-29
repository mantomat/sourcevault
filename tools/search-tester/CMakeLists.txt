add_executable(search_tester
    main.cpp
    FieldScoreCalculatorRepl.h)
target_link_libraries(search_tester PRIVATE sourcevaultcore)
target_compile_options(search_tester PRIVATE ${COMMON_COMPILE_OPTIONS})