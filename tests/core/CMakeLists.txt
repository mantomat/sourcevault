add_executable(testscore CoreTestsMain.cpp
        model/TestMedium.cpp
        model/TestMedium.h
        model/TestValidatedField.cpp
        model/TestValidatedField.h
        model/TestValidatedSet.cpp
        model/TestValidatedSet.h
        model/TestMediumUserData.cpp
        model/TestMediumUserData.h
        model/TestOptionalField.cpp
        model/TestOptionalField.h
        model/TestVideo.cpp
        model/TestVideo.h
        model/TestBook.cpp
        model/TestBook.h
        model/TestArticle.cpp
        model/TestArticle.h
        model/TestLibrary.cpp
        model/TestLibrary.h
)
target_link_libraries(testscore sourcevaultcore Qt6::Test)
target_compile_options(testscore PRIVATE ${COMMON_COMPILE_OPTIONS})
target_compile_options(sourcevaultcore PRIVATE ${COMMON_COMPILE_OPTIONS})
add_test(NAME TestsCore COMMAND testscore)