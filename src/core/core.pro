TEMPLATE = lib
TARGET = sourcevaultcore
CONFIG += c++20 staticlib

QT += core network

INCLUDEPATH += $$PWD

HEADERS += \
    shared/Cloneable.h \
    shared/MediumVisitor.h \
    shared/MediumTypeVisitor.h \
    shared/OverloadedVariantVisitor.h \
    shared/QUuidHasher.h \
    shared/MutMediumVisitor.h \
    model/Medium.h \
    model/Book.h \
    model/Video.h \
    model/Article.h \
    model/Library.h \
    model/ValidatedField.h \
    model/ValidatedSet.h \
    model/MediumUserData.h \
    queries/FilteringQuery.h \
    queries/SearchQuery.h \
    queries/SortingQuery.h \
    queries/search/SearchEngine.h \
    queries/search/SearchScoreVisitor.h \
    queries/search/FieldScoreCalculator.h \
    queries/filters/Filter.h \
    queries/filters/FavoriteFilter.h \
    queries/filters/MediumTypeFilter.h \
    queries/filters/MinimumPriorityFilter.h \
    queries/filters/TopicsFilter.h \
    queries/sortings/Sort.h \
    queries/sortings/TitleSort.h \
    queries/sortings/DateAddedSort.h \
    queries/sortings/PrioritySort.h \
    persistence/MediaSerializationConfigs.h \
    persistence/json/JsonPersistenceManager.h \
    persistence/json/deserialization/JsonMediaDeserializer.h \
    persistence/json/deserialization/JsonConverter.h \
    persistence/json/deserialization/JsonDeserializationError.h \
    persistence/json/deserialization/JsonParsingLib.h \
    persistence/json/deserialization/JsonArticleParser.h \
    persistence/json/deserialization/JsonBookParser.h \
    persistence/json/deserialization/JsonMediumParser.h \
    persistence/json/deserialization/JsonVideoParser.h \
    persistence/json/serialization/JsonMediumSerializerVisitor.h \
    persistence/json/serialization/JsonMediaSerializer.h \
    persistence/thumbnails/MediumThumbnailSetter.h \
    persistence/thumbnails/MediumThumbnailVisitor.h \
    persistence/thumbnails/ThumbnailsExporter.h \
    persistence/thumbnails/ThumbnailsImporter.h

SOURCES += \
    shared/MediumTypeVisitor.cpp \
    model/Medium.cpp \
    model/Book.cpp \
    model/Video.cpp \
    model/Article.cpp \
    model/Library.cpp \
    model/MediumUserData.cpp \
    queries/FilteringQuery.cpp \
    queries/SearchQuery.cpp \
    queries/SortingQuery.cpp \
    queries/search/SearchEngine.cpp \
    queries/search/SearchScoreVisitor.cpp \
    queries/search/FieldScoreCalculator.cpp \
    queries/filters/FavoriteFilter.cpp \
    queries/filters/MediumTypeFilter.cpp \
    queries/filters/MinimumPriorityFilter.cpp \
    queries/filters/TopicsFilter.cpp \
    queries/sortings/Sort.cpp \
    queries/sortings/TitleSort.cpp \
    queries/sortings/DateAddedSort.cpp \
    queries/sortings/PrioritySort.cpp \
    persistence/json/JsonPersistenceManager.cpp \
    persistence/json/deserialization/JsonMediaDeserializer.cpp \
    persistence/json/deserialization/JsonArticleParser.cpp \
    persistence/json/deserialization/JsonBookParser.cpp \
    persistence/json/deserialization/JsonVideoParser.cpp \
    persistence/json/serialization/JsonMediumSerializerVisitor.cpp \
    persistence/json/serialization/JsonMediaSerializer.cpp \
    persistence/thumbnails/MediumThumbnailSetter.cpp \
    persistence/thumbnails/MediumThumbnailVisitor.cpp \
    persistence/thumbnails/ThumbnailsExporter.cpp \
    persistence/thumbnails/ThumbnailsImporter.cpp