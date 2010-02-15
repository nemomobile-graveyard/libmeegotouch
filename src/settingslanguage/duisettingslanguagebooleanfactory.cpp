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

#include "duisettingslanguagebooleanfactory.h"
#include "duisettingslanguagebooleancontroller.h"
#include "duisettingslanguageboolean.h"

#include <DuiWidgetView>
#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <DuiDataStore>

DuiWidgetController *DuiSettingsLanguageBooleanFactory::createWidget(const DuiSettingsLanguageBoolean &settingsBool, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    Q_UNUSED(rootWidget)

    DuiWidgetController *parentWidget = new DuiWidgetController;
    parentWidget->setView(new DuiWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    DuiSettingsLanguageBooleanController *boolController = new DuiSettingsLanguageBooleanController(parentWidget);

    // Create a horizontal layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a button
    DuiButton *button = new DuiButton(parentWidget);
    button->setCheckable(true);
    button->setViewType(DuiButton::toggleType);
    button->connect(button, SIGNAL(toggled(bool)), boolController, SLOT(buttonToggled(bool)));

    // Get the previously selected value if it exists
    bool selectedValue = false;
    if (dataStore != NULL) {
        selectedValue = dataStore->value(settingsBool.key()).toBool();
    }
    button->setChecked(selectedValue);

    button->setText(settingsBool.title());
    button->setObjectName("SettingsLanguageBooleanValueButton");
    button->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    button->setProperty("key", settingsBool.key());
    layout->addItem(button);

    return parentWidget;
}
