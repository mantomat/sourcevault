#ifndef LIBRARYSIGNALS_H
#define LIBRARYSIGNALS_H

#include <QObject>

/**
 * This class is designed to decouple the signal emitter (QObject) from Library.
 *
 * A QObject's purpose is to be a unique "identity" within Qt's object tree. It has a parent,
 * children, and a unique identity that cannot be copied or moved.
 *
 * On the contrary, Library is a "value" class. It represents the data that lies beneath it, it
 * doesn't have an identity. But we needed to design some kind of observer pattern for it, and
 * overlooking the powerful Qt's signaling system would have been a bad choice.
 */
class LibrarySignals final : public QObject {
    Q_OBJECT
  signals:
    void mediaChanged();
};

#endif
