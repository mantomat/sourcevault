add_library(sourcevaultcore STATIC
        shared/Cloneable.h
        shared/MediumVisitor.h
        shared/MediumTypeVisitor.h
        shared/MediumTypeVisitor.cpp
        shared/OverloadedVariantVisitor.h
        shared/QUuidHasher.h
        shared/MutMediumVisitor.h
        
        model/Medium.cpp
        model/Medium.h
        model/Book.cpp
        model/Book.h
        model/Video.cpp
        model/Video.h
        model/Article.cpp
        model/Article.h
        model/Library.cpp
        model/Library.h
        model/ValidatedField.h
        model/ValidatedSet.h
        model/MediumUserData.cpp
        model/MediumUserData.h

        queries/FilteringQuery.cpp
        queries/FilteringQuery.h
        queries/SearchQuery.cpp
        queries/SearchQuery.h
        queries/SortingQuery.cpp
        queries/SortingQuery.h

        queries/search/SearchEngine.cpp
        queries/search/SearchEngine.h
        queries/search/SearchScoreVisitor.cpp
        queries/search/SearchScoreVisitor.h
        queries/search/FieldScoreCalculator.cpp
        queries/search/FieldScoreCalculator.h

        queries/filters/Filter.h
        queries/filters/FavoriteFilter.h
        queries/filters/FavoriteFilter.cpp
        queries/filters/MediumTypeFilter.h
        queries/filters/MediumTypeFilter.cpp
        queries/filters/MinimumPriorityFilter.h
        queries/filters/MinimumPriorityFilter.cpp
        queries/filters/TopicsFilter.h
        queries/filters/TopicsFilter.cpp

        queries/sortings/Sort.cpp
        queries/sortings/Sort.h
        queries/sortings/TitleSort.cpp
        queries/sortings/TitleSort.h
        queries/sortings/DateAddedSort.cpp
        queries/sortings/DateAddedSort.h
        queries/sortings/PrioritySort.cpp
        queries/sortings/PrioritySort.h

        persistence/MediaSerializationConfigs.h

        persistence/json/JsonPersistenceManager.cpp
        persistence/json/JsonPersistenceManager.h
        persistence/json/deserialization/JsonMediaDeserializer.cpp
        persistence/json/deserialization/JsonMediaDeserializer.h
        persistence/json/deserialization/JsonConverter.h
        persistence/json/deserialization/JsonDeserializationError.h
        persistence/json/deserialization/JsonParsingLib.h
        persistence/json/deserialization/JsonArticleParser.cpp
        persistence/json/deserialization/JsonArticleParser.h
        persistence/json/deserialization/JsonBookParser.cpp
        persistence/json/deserialization/JsonBookParser.h
        persistence/json/deserialization/JsonMediumParser.h
        persistence/json/deserialization/JsonVideoParser.cpp
        persistence/json/deserialization/JsonVideoParser.h
        persistence/json/serialization/JsonMediumSerializerVisitor.cpp
        persistence/json/serialization/JsonMediumSerializerVisitor.h
        persistence/json/serialization/JsonMediaSerializer.cpp
        persistence/json/serialization/JsonMediaSerializer.h

        persistence/thumbnails/MediumThumbnailSetter.cpp
        persistence/thumbnails/MediumThumbnailSetter.h
        persistence/thumbnails/MediumThumbnailVisitor.cpp
        persistence/thumbnails/MediumThumbnailVisitor.h
        persistence/thumbnails/ThumbnailsExporter.cpp
        persistence/thumbnails/ThumbnailsExporter.h
        persistence/thumbnails/ThumbnailsImporter.cpp
        persistence/thumbnails/ThumbnailsImporter.h
)

target_include_directories(sourcevaultcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultcore PUBLIC Qt6::Core Qt6::Network)
