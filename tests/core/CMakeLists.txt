add_executable(testscore CoreTestsMain.cpp
        testutils/mocks/MockConcreteMedium.h
        testutils/fixtures/JsonSerializationFixtures.h

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

        queries/TestFilteringQuery.cpp
        queries/TestFilteringQuery.h
        queries/TestSearchQuery.cpp
        queries/TestSearchQuery.h
        queries/TestSortingQuery.cpp
        queries/TestSortingQuery.h

        queries/search/TestFieldScoreCalculator.cpp
        queries/search/TestFieldScoreCalculator.h
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

        persistence/json/TestJsonPersistenceManager.cpp
        persistence/json/TestJsonPersistenceManager.h
        persistence/json/deserialization/TestJsonParsingLib.cpp
        persistence/json/deserialization/TestJsonParsingLib.h
        persistence/json/deserialization/TestJsonArticleParser.cpp
        persistence/json/deserialization/TestJsonArticleParser.h
        persistence/json/deserialization/TestJsonBookParser.cpp
        persistence/json/deserialization/TestJsonBookParser.h
        persistence/json/deserialization/TestJsonVideoParser.cpp
        persistence/json/deserialization/TestJsonVideoParser.h
        persistence/json/deserialization/TestJsonMediaDeserializer.cpp
        persistence/json/deserialization/TestJsonMediaDeserializer.h
        persistence/json/serialization/TestJsonMediumSerializerVisitor.cpp
        persistence/json/serialization/TestJsonMediumSerializerVisitor.h
        persistence/json/serialization/TestJsonMediaSerializer.cpp
        persistence/json/serialization/TestJsonMediaSerializer.h

        persistence/thumbnails/TestMediumThumbnailSetter.cpp
        persistence/thumbnails/TestMediumThumbnailSetter.h
        persistence/thumbnails/TestMediumThumbnailVisitor.cpp
        persistence/thumbnails/TestMediumThumbnailVisitor.h
        persistence/thumbnails/TestThumbnailsExporter.cpp
        persistence/thumbnails/TestThumbnailsExporter.h
        persistence/thumbnails/TestThumbnailsImporter.cpp
        persistence/thumbnails/TestThumbnailsImporter.h

)
target_link_libraries(testscore sourcevaultcore Qt6::Test)
target_compile_options(testscore PRIVATE ${COMMON_COMPILE_OPTIONS})
target_include_directories(testscore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME TestsCore COMMAND testscore)