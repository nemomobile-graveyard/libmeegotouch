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

#include <MSlider>
#include "demoextension.h"

Q_EXPORT_PLUGIN2(demoextension, DemoApplicationExtension)

DemoApplicationExtension::DemoApplicationExtension() :
    app(NULL),
    slider(NULL)
{
}

DemoApplicationExtension::~DemoApplicationExtension()
{
    delete slider;
}

void DemoApplicationExtension::setDemoApplicationInterface(DemoApplicationInterface &application)
{
    app = &application;
}

void DemoApplicationExtension::setDemoApplicationControlledValue(int value)
{
    slider->setValue(value);
}

bool DemoApplicationExtension::initialize(const QString &)
{
    slider = new MSlider;
    slider->setObjectName("demo_slider");
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(informSliderValueToApplication(int)));

    return true;
}

QGraphicsWidget *DemoApplicationExtension::widget()
{
    return slider;
}

void DemoApplicationExtension::informSliderValueToApplication(int value)
{
    if (app != NULL) {
        app->setDemoExtensionControlledValue(value);
    }
}
