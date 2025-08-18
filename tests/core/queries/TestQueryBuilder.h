
#ifndef TESTQUERYBUILDER_H
#define TESTQUERYBUILDER_H

#include "model/MediumUserData.h"
#include "queries/QueryBuilder.h"
#include "queries/filters/Filter.h"
#include "queries/sortings/Sort.h"

#include <QObject>

using Core::Model::MediumUserData;
using Core::Queries::QueryBuilder;
using Core::Queries::Filters::Filter;
using Core::Queries::Sortings::Sort;

class TestQueryBuilder : public QObject {
    Q_OBJECT

    static void queryTestHelper(std::vector<std::unique_ptr<const Medium>> libraryMedia,
                                const QueryBuilder &queryBuilder,
                                const std::set<QUuid> &expectedIdsAfterQuery);

  private slots:
    static void testCopyConstruction();

    static void testCopyAssignment();

    static void testAddFilter_data();
    static void testAddFilter();

    static void testSetSort_data();
    static void testSetSort();

    static void testReset_data();
    static void testReset();

    static void testQuery_data();
    static void testQuery();
};

#endif
