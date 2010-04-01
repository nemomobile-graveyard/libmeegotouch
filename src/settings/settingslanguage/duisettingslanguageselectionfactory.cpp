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

#include "duisettingslanguageselectionfactory.h"
#include "duisettingslanguageselectioncontroller.h"
#include "duisettingslanguageselection.h"
#include "duisettingslanguageoption.h"

#include <DuiWidgetView>
#include <QGraphicsLinearLayout>
#include <DuiButtonGroup>
#include <DuiButton>
#include <DuiDataStore>

DuiWidgetController *DuiSettingsLanguageSelectionFactory::createWidget(const DuiSettingsLanguageSelection &settingsSelection,
        DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    Q_UNUSED(rootWidget)

    DuiWidgetController *parentWidget = new DuiWidgetController;
    parentWidget->setView(new DuiWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    DuiSettingsLanguageSelectionController *selectionController = new DuiSettingsLanguageSelectionController(parentWidget);

    // Create a horizontal layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a button group
    DuiButtonGroup *buttonGroup = new DuiButtonGroup(parentWidget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(DuiButton *)), selectionController, SLOT(buttonClicked(DuiButton *)));

    // Get the previously selected value or use 0 if it doesn't exist
    int selectedValue = dataStore != NULL ? dataStore->value(settingsSelection.key()).toInt() : 0;
    foreach(const DuiSettingsLanguageOption * value, settingsSelection.options()) {
        // Create a radio button for the enum value
        DuiButton *button = new DuiButton(parentWidget);
        button->setText(value->title());
        button->setCheckable(true);
        button->setObjectName("SettingsLanguageOptionButton");
        button->setChecked(value->value() == selectedValue);
        button->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
        button->setProperty("key", settingsSelection.key());
        button->setProperty("value", value->value());
        layout->addItem(button);

        // Add the button to a button group
        buttonGroup->addButton(button);
    }

    return parentWidget;
}
