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

#include "msettingslanguagetextfactory.h"
#include "msettingslanguagetextcontroller.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguagetext.h"
#include "msettingslanguagewidget.h"

#include <MStylableWidget>
#include <MLabel>
#include <MTextEdit>
#include <MDataStore>
#include <QGraphicsLinearLayout>

MWidgetController *MSettingsLanguageTextFactory::createWidget(const MSettingsLanguageText &settingsText, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    MStylableWidget *parentWidget = new MStylableWidget;
    parentWidget->setStyleName("MSettingsLanguageItem");

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageTextController *textController = new MSettingsLanguageTextController(parentWidget);

    // Create a vertical layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsText.title());
    label->setStyleName("CommonSingleTitleInverted");
    layout->addItem(label);

    // Create a text edit widget and put it into the layout
    MTextEdit *textEdit = new MTextEdit;
    textEdit->connect(textEdit, SIGNAL(lostFocus(Qt::FocusReason)), textController, SLOT(textEditLostFocus(Qt::FocusReason)));
    textEdit->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    textEdit->setProperty("key", settingsText.key());
    layout->addItem(textEdit);

    // Get the previously entered value if it exists
    if (dataStore != NULL) {
        textEdit->setText(dataStore->value(settingsText.key()).toString());
    }

    return parentWidget;
}
