#include "UrlFieldEditor.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>

namespace Gui::Components {

UrlFieldEditor::UrlFieldEditor(const std::function<bool(const QUrl &)> &newValidator,
                               bool enableFileSystem, QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QLineEdit{this}}
    , enableFileSystemSelect(enableFileSystem)
    , validator{newValidator} {

    QWidget *urlEditWidget{new QWidget{this}};
    QHBoxLayout *editLayout{new QHBoxLayout{urlEditWidget}};
    editLayout->setContentsMargins(0, 0, 0, 0);
    editLayout->addWidget(editInput);

    if (enableFileSystemSelect) {
        QPushButton *selectFromFileManagerButton{new QPushButton{this}};
        selectFromFileManagerButton->setIcon(QIcon{":/assets/icons/filesystem.png"});
        editLayout->addWidget(selectFromFileManagerButton);
        connect(selectFromFileManagerButton, &QPushButton::pressed, this,
                &UrlFieldEditor::requestThumbnailUrlSelectionModal);
    }

    setEditModeWidget(urlEditWidget);
    setEditMode(false);

    connect(editInput, &QLineEdit::textChanged, this, &UrlFieldEditor::validateInput);
    connect(editInput, &QLineEdit::textChanged, this, &UrlFieldEditor::stateChanged);
    connect(this, &UrlFieldEditor::enabledChanged, this, &UrlFieldEditor::validateInput);
    connect(this, &UrlFieldEditor::enabledChanged, this, &UrlFieldEditor::stateChanged);
}

void UrlFieldEditor::setUrl(const std::optional<QUrl> &url) {
    if (url.has_value()) {
        setViewModeText(url->toString());
        editInput->setText(url->toString());
        setEnabled(true);
    } else {
        setViewModeText(notSet);
        editInput->setText("");
        setEnabled(false);
    }
    validateInput();
}

auto UrlFieldEditor::url() const -> std::optional<QUrl> {
    if (!isEnabled()) {
        return std::nullopt;
    }
    if (!isValid()) {
        return std::nullopt;
    }
    return stringToUrl(editInput->text());
}

auto UrlFieldEditor::isValid() const -> bool {
    return !isEnabled() || validator(stringToUrl(editInput->text()));
}

auto UrlFieldEditor::stringToUrl(const QString &str) -> QUrl {
    if (str.contains("://")) {
        return QUrl{str};
    }
    return QUrl::fromLocalFile(str);
}

void UrlFieldEditor::onThumbnailUrlPathSelected(const QString &path) {
    setUrl(stringToUrl(path));
}

void UrlFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}