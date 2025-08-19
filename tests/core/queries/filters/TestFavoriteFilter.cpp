#include "TestFavoriteFilter.h"

#include "model/Video.h"
#include "queries/filters/FavoriteFilter.h"

#include <QTest>

using Core::Model::Video;
using Core::Queries::Filters::FavoriteFilter;

void TestFavoriteFilter::testClone() {
    const auto filter{std::make_unique<FavoriteFilter>()};

    const auto clone{filter->clone()};

    QVERIFY(filter != clone);
    QVERIFY(dynamic_cast<const FavoriteFilter *>(clone.get()) != nullptr);
}

void TestFavoriteFilter::testMatches_data() {
    QTest::addColumn<Video>("medium");
    QTest::addColumn<bool>("shouldMatch");

    QTest::addRow("Non-favorite media don't match") << Video::create("video").value() << false;

    auto favoriteVideo{Video::create("fav video").value()};
    favoriteVideo.userData().favorite() = true;
    QTest::addRow("Favorite media match") << favoriteVideo << true;
}
void TestFavoriteFilter::testMatches() {
    QFETCH(Video, medium);
    QFETCH(bool, shouldMatch);
    FavoriteFilter filter;

    const bool didMatch{filter.matches(&medium)};

    QCOMPARE(didMatch, shouldMatch);
}