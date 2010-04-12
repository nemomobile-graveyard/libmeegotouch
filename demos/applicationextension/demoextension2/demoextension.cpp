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

Q_EXPORT_PLUGIN2(demoextension2, DemoApplicationExtension)

DemoApplicationExtension::DemoApplicationExtension() :
    app(NULL)
{
}

void DemoApplicationExtension::setDemoApplicationInterface(DemoApplicationInterface &application)
{
    app = &application;
}

void DemoApplicationExtension::setDemoApplicationControlledValue(int value)
{
    if (app != NULL) {
        QStringList words;
        words << "low" << "medium" << "high";
        app->setDemoExtensionControlledValue(words[qBound(0, value / 33, 2) ]);
    }
}

bool DemoApplicationExtension::initialize(const QString &)
{
    return true;
}
