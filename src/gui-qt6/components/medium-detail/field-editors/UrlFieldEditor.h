#ifndef URLFIELDEDITOR_H
#define URLFIELDEDITOR_H

#include "FieldEditor.h"

#include <QLabel>
#include <QLineEdit>
#include <QUrl>

namespace Gui::Components {

class UrlFieldEditor : public FieldEditor {
    Q_OBJECT

    QLineEdit *editInput;
    bool enableFileSystemSelect;

    std::function<bool(const QUrl &)> validator;

  public:
    ~UrlFieldEditor() override = default;

    UrlFieldEditor(const std::function<bool(const QUrl &)> &newValidator, bool enableFileSystem,
                   QWidget *parent);

    UrlFieldEditor(const UrlFieldEditor &) = delete;
    UrlFieldEditor(UrlFieldEditor &&) = delete;
    auto operator=(const UrlFieldEditor &) -> UrlFieldEditor & = delete;
    auto operator=(UrlFieldEditor &&) -> UrlFieldEditor & = delete;

    void setUrl(const std::optional<QUrl> &url);
    [[nodiscard]] auto url() const -> std::optional<QUrl>;
    [[nodiscard]] auto isValid() const -> bool override;

  private:
    static auto stringToUrl(const QString &str) -> QUrl;

  signals:
    void requestThumbnailUrlSelectionModal();

  public slots:
    void onThumbnailUrlPathSelected(const QString &path);

  private slots:
    void validateInput();
};

}

#endif