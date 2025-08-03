#ifndef ARTICLE_H
#define ARTICLE_H

#include "Medium.h"

#include <QDate>
#include <QUrl>

namespace Core::Model {

class Article : public Medium {

  public:
    ~Article() override = default;

    ValidatedField<QUrl>& articleUrl();
    [[nodiscard]] const ValidatedField<QUrl>& articleUrl() const;
    static bool articleUrlValidator(const QUrl& urlToValidate);

    ValidatedField<QString>& sourceName();
    [[nodiscard]] const ValidatedField<QString>& sourceName() const;
    static bool sourceNameValidator(const QString& sourceNameToValidate);

    ValidatedField<unsigned int>& readTimeMinutes();
    [[nodiscard]] const ValidatedField<unsigned int>& readTimeMinutes() const;
    static bool readTimeMinutesValidator(unsigned int readTimeMinutesToValidate);

    ValidatedField<QDate>& publicationDate();
    [[nodiscard]] const ValidatedField<QDate>& publicationDate() const;
    static bool publicationDateValidator(const QDate& dateToValidate);

  private:
    ValidatedField<QUrl> articleUrl_{articleUrlValidator};
    ValidatedField<QString> sourceName_{sourceNameValidator};
    ValidatedField<unsigned int> readTimeMinutes_{readTimeMinutesValidator};
    ValidatedField<QDate> publicationDate_{publicationDateValidator};
};

}

#endif
