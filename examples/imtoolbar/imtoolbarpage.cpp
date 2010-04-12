/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "imtoolbarpage.h"
#include <MLayout>
#include <MLocale>
#include <MLabel>
#include <MTextEdit>
#include <MButton>
#include <QDebug>
#include <MGridLayoutPolicy>

namespace
{
    //! the maximum and minimum width for labels
    const int MaximumLabelWidth = 175;
    const int MinimumTextEntryWidth = 275;
    QList<MTextEdit *> Entries;
}

ImToolbarPage::ImToolbarPage()
    : MApplicationPage()
{
    setTitle("Custom Inputmethod Toolbar Text Entries");
}


ImToolbarPage::~ImToolbarPage()
{
}

void ImToolbarPage::createContent()
{
    MApplicationPage::createContent();
    QGraphicsWidget *panel = centralWidget();
    MGridLayoutPolicy *layoutPolicy = ImToolbarPage::createAndSetupGridPolicy(panel);

    MTextEdit *multi1 =  new MTextEdit(MTextEditModel::MultiLine, "", centralWidget());
    // can specify only the file name, then use the default path /usr/share/m/imtoolbars/
    multi1->attachToolbar("toolbar1.xml");

    Entries << multi1;
    Entries.at(0)->setPrompt("Example toolbar 1");

    MTextEdit *single1 = new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    // can also specify the absolute name.
    single1->attachToolbar("/usr/share/m/imtoolbars/toolbar2.xml");

    Entries << single1;
    Entries.at(1)->setPrompt("Example toolbar 2");

    MTextEdit *multi2 =  new MTextEdit(MTextEditModel::MultiLine, "", centralWidget());

    Entries << multi2;
    Entries.at(2)->setPrompt("Normal textentry without custom toolbar");

    for (int i = 0; i < Entries.count(); i++)
        layoutPolicy->addItem(Entries.at(i), i, 1);

    MLabel *label;
    for (int i = 0; i < Entries.count(); i++) {
        label = new MLabel(centralWidget());
        label->setText("Free text");
        label->setWordWrap(true);
        label->setMinimumWidth(MaximumLabelWidth);
        label->setMaximumWidth(MaximumLabelWidth);
        layoutPolicy->addItem(label, i, 0);
    }

    layoutPolicy->setColumnMinimumWidth(1, MinimumTextEntryWidth);
    MLabel *labelHeader1 = new MLabel(centralWidget());
    labelHeader1->setMinimumHeight(250);
    layoutPolicy->addItem(labelHeader1, Entries.count(), 0);
}

MGridLayoutPolicy *ImToolbarPage::createAndSetupGridPolicy(QGraphicsWidget *panel)
{
    MLayout *layout = new MLayout(panel);
    panel->setLayout(layout);
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->setContentsMargins(20, 0, 20, 0);
    return policy;
}

