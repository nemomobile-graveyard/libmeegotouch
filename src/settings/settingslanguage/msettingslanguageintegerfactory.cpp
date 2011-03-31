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

#include "msettingslanguageinteger.h"
#include "msettingslanguageintegerfactory.h"
#include "msettingslanguageintegercontroller.h"
#include "msettingslanguagewidget.h"

#include <MStylableWidget>
#include <MLabel>
#include <MSlider>
#include <MDataStore>
#include <QGraphicsLinearLayout>

MWidgetController *MSettingsLanguageIntegerFactory::createWidget(const MSettingsLanguageInteger &settingsInteger, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    MStylableWidget *parentWidget = new MStylableWidget;
    parentWidget->setStyleName("MSettingsLanguageItem");

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageIntegerController *intController = new MSettingsLanguageIntegerController(parentWidget);
    intController->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    intController->setProperty("key", settingsInteger.key());

    // Create a vertical layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsInteger.title());
    label->setStyleName("CommonSingleTitleInverted");
    layout->addItem(label);

    MSlider *slider = new MSlider;
    slider->setStyleName("CommonSliderInverted");
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
