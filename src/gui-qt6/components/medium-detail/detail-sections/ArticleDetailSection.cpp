#include "ArticleDetailSection.h"

#include "components/medium-detail/field-editors/DateFieldEditor.h"
#include "components/medium-detail/field-editors/NumberFieldEditor.h"
#include "components/medium-detail/field-editors/StringFieldEditor.h"

#include <QFormLayout>

namespace Gui::Components {

ArticleDetailSection::ArticleDetailSection(const Dependencies &deps, QWidget *parent)
    : QWidget(parent)
    , articleUrlEditor{new UrlFieldEditor{deps.articleUrlValidator, false, this}}
    , sourceNameEditor{new StringFieldEditor{deps.sourceNameValidator, true, this}}
    , readTimeMinutesFieldEditor{new NumberFieldEditor{deps.readTimeMinutesValidator, this}}
    , publicationDateFieldEditor{new DateFieldEditor{deps.publicationDateValidator, this}} {

    auto *layout{new QFormLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow("Article URL:", articleUrlEditor);
    layout->addRow("Source name:", sourceNameEditor);
    layout->addRow("Read time (minutes):", readTimeMinutesFieldEditor);
    layout->addRow("Publication date:", publicationDateFieldEditor);

    connect(articleUrlEditor, &UrlFieldEditor::stateChanged, this,
            &ArticleDetailSection::stateChanged);
    connect(sourceNameEditor, &StringFieldEditor::stateChanged, this,
            &ArticleDetailSection::stateChanged);
    connect(readTimeMinutesFieldEditor, &NumberFieldEditor::stateChanged, this,
            &ArticleDetailSection::stateChanged);
    connect(publicationDateFieldEditor, &DateFieldEditor::stateChanged, this,
            &ArticleDetailSection::stateChanged);
}

auto ArticleDetailSection::isEverythingValid() const -> bool {
    return articleUrlEditor->isValid() && sourceNameEditor->isValid() &&
           readTimeMinutesFieldEditor->isValid() && publicationDateFieldEditor->isValid();
}

auto ArticleDetailSection::getState() const -> ArticleDetailViewModel {
    return ArticleDetailViewModel{.articleUrl = articleUrlEditor->url(),
                                  .sourceName = sourceNameEditor->text(),
                                  .readTimeMinutes = static_cast<std::optional<unsigned int>>(
                                      readTimeMinutesFieldEditor->number()),
                                  .publicationDate = publicationDateFieldEditor->date()};
}

void ArticleDetailSection::setState(const ArticleDetailViewModel &initData) {
    articleUrlEditor->setUrl(initData.articleUrl);
    sourceNameEditor->setText(initData.sourceName);
    readTimeMinutesFieldEditor->setNumber(
        static_cast<std::optional<int>>(initData.readTimeMinutes));
    publicationDateFieldEditor->setDate(initData.publicationDate);
}

void ArticleDetailSection::setEditMode(bool isEditing) {
    articleUrlEditor->setEditMode(isEditing);
    sourceNameEditor->setEditMode(isEditing);
    readTimeMinutesFieldEditor->setEditMode(isEditing);
    publicationDateFieldEditor->setEditMode(isEditing);
}

}