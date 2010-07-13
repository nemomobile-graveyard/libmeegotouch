#include "textentrynavigationpage.h"

#include <QList>
#include <QPointer>
#include <MLabel>
#include <MLayout>
#include <MGridLayoutPolicy>

namespace {
    MTextEdit *createTextEdit(QObject *owner,
                              const QString &objName,
                              M::TextContentType contentType,
                              bool multi = false,
                              bool rich = false)
    {
        MTextEditModel::LineMode lm = (multi ? MTextEditModel::MultiLine
                                             : MTextEditModel::SingleLine);

        MTextEdit *e = 0;

        e = (rich ? new MRichTextEdit(lm)
                  : new MTextEdit(lm));

        e->setParent(owner);
        e->setContentType(contentType);

        e->setObjectName(objName);

        return e;
    }
}

TextEntryNavigationPage::TextEntryNavigationPage()
    : TemplatePage(TemplatePage::UserInput)
{}

TextEntryNavigationPage::~TextEntryNavigationPage()
{}

QString TextEntryNavigationPage::timedemoTitle()
{
    return "TextEntryNavigation";
}

void TextEntryNavigationPage::createContent()
{
    MApplicationPage::createContent();
    QGraphicsWidget *panel = centralWidget();
    MLayout *layout = new MLayout(panel);
    panel->setLayout(layout);

    MGridLayoutPolicy *layoutPolicy = new MGridLayoutPolicy(layout);
    layoutPolicy->setContentsMargins(20, 0, 20, 0);

    QList<MTextEdit *> edits;
    // 1st row:
    edits << createTextEdit(panel, "Numbers", M::NumberContentType)
          << 0
          << createTextEdit(panel, "Multi-line rich text", M::FreeTextContentType, true, true)
          << 0
    // 2nd row:
          << createTextEdit(panel, "Default", M::FreeTextContentType)
          << createTextEdit(panel, "Multi-line text", M::FreeTextContentType, true)
          << 0
          << createTextEdit(panel, "Default", M::FreeTextContentType)
    // 3rd row:
          << createTextEdit(panel, "Default", M::FreeTextContentType)
          << createTextEdit(panel, "Phone numbers", M::PhoneNumberContentType)
          << 0
          << createTextEdit(panel, "Multi-line rich text", M::FreeTextContentType, true, true);

    int i = 0;
    int j = 0;
    foreach (MTextEdit *edit, edits) {
        if (edit) {
            layoutPolicy->addItem(new MLabel(edit->objectName(), edit), i, j % 4);
            layoutPolicy->addItem(edit, i + 1, j % 4);
        }

        if (++j % 4 == 0) {
            i += 2;
        }
    }
}
