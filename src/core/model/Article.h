#ifndef ARTICLE_H
#define ARTICLE_H

#include "Medium.h"
#include "OptionalField.h"

#include <QDate>
#include <QUrl>

namespace Core::Model {

class Article : public Medium {

  public:
    ~Article() override = default;

    ValidatedField<QUrl>& articleUrl();
    const ValidatedField<QUrl>& articleUrl() const;
    static bool articleUrlValidator(const QUrl& urlToValidate);

    ValidatedField<QString>& sourceName();
    const ValidatedField<QString>& sourceName() const;
    static bool sourceNameValidator(const QString& sourceNameToValidate);

    OptionalField<unsigned int>& readTimeMinutes();
    const OptionalField<unsigned int>& readTimeMinutes() const;

    ValidatedField<QDate>& publicationDate();
    const ValidatedField<QDate>& publicationDate() const;
    static bool publicationDateValidator(const QDate& dateToValidate);

  private:
    ValidatedField<QUrl> articleUrl_{articleUrlValidator};
    ValidatedField<QString> sourceName_{sourceNameValidator};
    OptionalField<unsigned int> readTimeMinutes_;
    ValidatedField<QDate> publicationDate_{publicationDateValidator};
};

}

#endif
