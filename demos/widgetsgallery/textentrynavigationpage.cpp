#include "textentrynavigationpage.h"

#include <QList>
#include <QPointer>
#include <MLabel>
#include <MLayout>
#include <MGridLayoutPolicy>

namespace {
    MTextEdit *createTextEdit(QGraphicsItem *parent,
                              const QString &objName,
                              M::TextContentType contentType,
                              bool multi = false,
                              bool rich = false)
    {
        MTextEditModel::LineMode lm = (multi ? MTextEditModel::MultiLine
                                             : MTextEditModel::SingleLine);

        MTextEdit *e = 0;

        e = (rich ? new MRichTextEdit(lm, "", parent)
                  : new MTextEdit(lm, "", parent));

        e->setContentType(contentType);

        e->setObjectName(objName);
        e->setPreferredWidth(50);

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
    layoutPolicy->setColumnStretchFactor(0,0);

    QList<MTextEdit *> edits;
    // 1st row:
    edits << createTextEdit(panel, "Numbers", M::NumberContentType)
          << createTextEdit(panel, "Multi-line rich text", M::FreeTextContentType, true, true)
    // 2nd row:
          << createTextEdit(panel, "Default", M::FreeTextContentType)
          << createTextEdit(panel, "Multi-line text", M::FreeTextContentType, true)
    // 3rd row:
          << createTextEdit(panel, "Phone number", M::PhoneNumberContentType)
          << createTextEdit(panel, "Multi-line rich text", M::FreeTextContentType, true, true);

    int i = 0;
    int j = 0;
    foreach (MTextEdit *edit, edits) {
        if (edit) {
            MLabel *label =  new MLabel(edit->objectName(), panel);
            label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            layoutPolicy->addItem(label, i, j);
            layoutPolicy->addItem(edit, i + 1, j);
        }

        if (++j == 2) {
            i += 2;
            j = 0;
        }
    }
}
