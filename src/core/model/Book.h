#ifndef BOOK_H
#define BOOK_H

#include "Medium.h"

#include <QUrl>

namespace Core::Model {

class Book : public Medium {

  public:
    ~Book() override = default;

    ValidatedField<QString>& isbn();
    const ValidatedField<QString>& isbn() const;
    static bool isbnValidator(const QString& isbnToValidate);

    ValidatedField<QString>& edition();
    const ValidatedField<QString>& edition() const;
    static bool editionValidator(const QString& editionToValidate);

    ValidatedField<QString>& publisher();
    const ValidatedField<QString>& publisher() const;
    static bool publisherValidator(const QString& publisherToValidate);

    ValidatedField<int>& yearPublished();
    const ValidatedField<int>& yearPublished() const;
    static bool yearPublishedValidator(int yearPublishedToValidate);

    ValidatedField<unsigned int>& pageNumber();
    const ValidatedField<unsigned int>& pageNumber() const;
    static bool pageNumberValidator(unsigned int pageNumberToValidate);

    ValidatedField<QString>& description();
    const ValidatedField<QString>& description() const;
    static bool descriptionValidator(const QString& descriptionToValidate);

    ValidatedField<QUrl>& thumbnailUrl();
    const ValidatedField<QUrl>& thumbnailUrl() const;
    static bool thumbnailUrlValidator(const QUrl& urlToValidate);

  private:
    ValidatedField<QString> isbn_{isbnValidator};
    ValidatedField<QString> edition_{editionValidator};
    ValidatedField<QString> publisher_{publisherValidator};
    ValidatedField<int> yearPublished_{yearPublishedValidator};
    ValidatedField<unsigned int> pageNumber_{pageNumberValidator};
    ValidatedField<QString> description_{descriptionValidator};
    ValidatedField<QUrl> thumbnailUrl_{thumbnailUrlValidator};
};

}

#endif
