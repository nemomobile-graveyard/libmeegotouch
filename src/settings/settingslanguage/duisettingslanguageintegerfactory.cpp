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

#include "duisettingslanguageinteger.h"
#include "duisettingslanguageintegerfactory.h"
#include "duisettingslanguageintegercontroller.h"

#include <DuiWidgetView>
#include <DuiLabel>
#include <DuiSlider>
#include <DuiDataStore>
#include <QGraphicsLinearLayout>

DuiWidgetController *DuiSettingsLanguageIntegerFactory::createWidget(const DuiSettingsLanguageInteger &settingsInteger, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    DuiWidgetController *parentWidget = new DuiWidgetController;
    parentWidget->setView(new DuiWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    DuiSettingsLanguageIntegerController *intController = new DuiSettingsLanguageIntegerController(parentWidget);
    intController->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    intController->setProperty("key", settingsInteger.key());

    // Create a horizontal layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    DuiLabel *label = new DuiLabel(settingsInteger.title());
    label->setObjectName("SettingsLanguageLabel");
    layout->addItem(label);

    DuiSlider *slider = new DuiSlider;
    slider->setObjectName("SettingsLanguageSlider");
    int minVal;
    if (settingsInteger.minValue(minVal)) {
        slider->setMinimum(minVal);
    }
    int maxVal;
    if (settingsInteger.maxValue(maxVal)) {
        slider->setMaximum(maxVal);
    }

    slider->connect(slider, SIGNAL(valueChanged(int)), intController, SLOT(changeValue(int)));
    layout->addItem(slider);

    // Get the previously entered value if it exists
    if (dataStore != NULL) {
        slider->setValue(dataStore->value(settingsInteger.key()).toInt());
    }

    return parentWidget;
}
