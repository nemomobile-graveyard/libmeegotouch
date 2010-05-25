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

#ifndef DEMOEXTENSION_H_
#define DEMOEXTENSION_H_

#include <QObject>
#include "demoapplicationextensioninterface.h"

class MSlider;

/*!
  This class is a demo application extension that has an UI
 */
class DemoApplicationExtension : public QObject, public DemoApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(DemoApplicationExtensionInterface MApplicationExtensionInterface)

public:
    DemoApplicationExtension();
    virtual ~DemoApplicationExtension();

    //! methods derived from DemoApplicationExtensionInterface
    void setDemoApplicationInterface(DemoApplicationInterface &application);
    virtual void setDemoApplicationControlledValue(int value);

    //! methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

private slots:
    void informSliderValueToApplication(int value);

private:
    DemoApplicationInterface *app;
    MSlider *slider;
};


#endif
