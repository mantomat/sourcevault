#ifndef BOOK_H
#define BOOK_H

#include "Medium.h"
#include "OptionalField.h"

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

    OptionalField<int>& yearPublished();
    const OptionalField<int>& yearPublished() const;

    OptionalField<unsigned int>& pageNumber();
    const OptionalField<unsigned int>& pageNumber() const;

    ValidatedField<QString>& description();
    const ValidatedField<QString>& description() const;
    static bool descriptionValidator(const QString& descriptionToValidate);

    ValidatedField<QUrl>& thumbnailUrl();
    const ValidatedField<QUrl>& thumbnailUrl() const;
    static bool thumbnailUrlValidator(const QUrl& url);

  private:
    ValidatedField<QString> isbn_{isbnValidator};
    ValidatedField<QString> edition_{editionValidator};
    ValidatedField<QString> publisher_{publisherValidator};
    OptionalField<int> yearPublished_;
    OptionalField<unsigned int> pageNumber_;
    ValidatedField<QString> description_{descriptionValidator};
    ValidatedField<QUrl> thumbnailUrl_{thumbnailUrlValidator};
};

}

#endif
