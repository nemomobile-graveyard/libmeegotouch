/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include "textentrynavigationpage.h"

#include <QList>
#include <QPointer>
#include <MLabel>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MPannableViewport>

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
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);
    setStyleName(inv("CommonApplicationPage"));
    QGraphicsWidget *panel = centralWidget();
    MLayout *layout = new MLayout(panel);
    panel->setLayout(layout);

    MGridLayoutPolicy *layoutPolicy = new MGridLayoutPolicy(layout);
    layoutPolicy->setContentsMargins(0, 0, 0, 0);
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
            edit->setStyleName(inv("CommonSingleInputFieldLabeled"));
            MLabel *label =  new MLabel(edit->objectName(), panel);
            label->setObjectName("label");
            label->setStyleName(inv("CommonFieldLabel"));
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
