/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include <DuiLayout>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiButton>
#include <QDebug>
#include <DuiGridLayoutPolicy>

namespace
{
    //! the maximum and minimum width for labels
    const int MaximumLabelWidth = 175;
    const int MinimumTextEntryWidth = 275;
    QList<DuiTextEdit *> Entries;
}

ImToolbarPage::ImToolbarPage()
    : DuiApplicationPage()
{
    setTitle("Custom Inputmethod Toolbar Text Entries");
}


ImToolbarPage::~ImToolbarPage()
{
}

void ImToolbarPage::createContent()
{
    DuiApplicationPage::createContent();
    DuiWidget *panel = centralWidget();
    DuiGridLayoutPolicy *layoutPolicy = ImToolbarPage::createAndSetupGridPolicy(panel);

    DuiTextEdit *multi1 =  new DuiTextEdit(DuiTextEditModel::MultiLine, "", centralWidget());
    multi1->attachToolbar("toolbar1");

    Entries << multi1;
    Entries.at(0)->setPrompt("Example toolbar 1");

    DuiTextEdit *single1 = new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());
    single1->attachToolbar("toolbar2");

    Entries << single1;
    Entries.at(1)->setPrompt("Example toolbar 2");

    DuiTextEdit *multi2 =  new DuiTextEdit(DuiTextEditModel::MultiLine, "", centralWidget());

    Entries << multi2;
    Entries.at(2)->setPrompt("Normal textentry without custom toolbar");

    for (int i = 0; i < Entries.count(); i++)
        layoutPolicy->addItem(Entries.at(i), i, 1);

    DuiLabel *label;
    for (int i = 0; i < Entries.count(); i++) {
        label = new DuiLabel(centralWidget());
        label->setText("Free text");
        label->setWordWrap(true);
        label->setMinimumWidth(MaximumLabelWidth);
        label->setMaximumWidth(MaximumLabelWidth);
        layoutPolicy->addItem(label, i, 0);
    }

    layoutPolicy->setColumnMinimumWidth(1, MinimumTextEntryWidth);
    DuiLabel *labelHeader1 = new DuiLabel(centralWidget());
    labelHeader1->setMinimumHeight(250);
    layoutPolicy->addItem(labelHeader1, Entries.count(), 0);
}

DuiGridLayoutPolicy *ImToolbarPage::createAndSetupGridPolicy(DuiWidget *panel)
{
    DuiLayout *layout = new DuiLayout(panel);
    panel->setLayout(layout);
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
    policy->setContentsMargins(20, 0, 20, 0);
    return policy;
}

