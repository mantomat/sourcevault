add_executable(testscore CoreTestsMain.cpp
        shared/TestMediumTypeVisitor.cpp
        shared/TestMediumTypeVisitor.h

        model/TestMedium.cpp
        model/TestMedium.h
        model/TestValidatedField.cpp
        model/TestValidatedField.h
        model/TestValidatedSet.cpp
        model/TestValidatedSet.h
        model/TestMediumUserData.cpp
        model/TestMediumUserData.h
        model/TestVideo.cpp
        model/TestVideo.h
        model/TestBook.cpp
        model/TestBook.h
        model/TestArticle.cpp
        model/TestArticle.h
        model/TestLibrary.cpp
        model/TestLibrary.h

        queries/TestQueryBuilder.cpp
        queries/TestQueryBuilder.h

        queries/search/TestSearchScoreVisitor.cpp
        queries/search/TestSearchScoreVisitor.h
        queries/search/TestSearchEngine.cpp
        queries/search/TestSearchEngine.h

        queries/filters/TestFavoriteFilter.cpp
        queries/filters/TestFavoriteFilter.h
        queries/filters/TestMediumTypeFilter.cpp
        queries/filters/TestMediumTypeFilter.h

        queries/sortings/TestSort.cpp
        queries/sortings/TestSort.h
        queries/sortings/TestDateAddedSort.cpp
        queries/sortings/TestDateAddedSort.h
        queries/sortings/TestTitleSort.cpp
        queries/sortings/TestTitleSort.h
)
target_link_libraries(testscore sourcevaultcore Qt6::Test)
target_compile_options(testscore PRIVATE ${COMMON_COMPILE_OPTIONS})
target_compile_options(sourcevaultcore PRIVATE ${COMMON_COMPILE_OPTIONS})
add_test(NAME TestsCore COMMAND testscore)