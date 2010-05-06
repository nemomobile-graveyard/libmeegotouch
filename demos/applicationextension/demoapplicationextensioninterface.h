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

#ifndef DEMOAPPLICATIONEXTENSIONINTERFACE_H_
#define DEMOAPPLICATIONEXTENSIONINTERFACE_H_

#include <QObject>
#include <mapplicationextensioninterface.h>

class DemoApplicationInterface
{
public:
    /*!
     * Sets a demo extension controlled value.
     */
    virtual void setDemoExtensionControlledValue(int value) = 0;
    virtual void setDemoExtensionControlledValue(const QString &value) = 0;
};

/*!
 * A demo application extension interface
 */
class DemoApplicationExtensionInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual ~DemoApplicationExtensionInterface() {}

    /*!
     * Tells the extension about an implementation of the demo application interface.
     */
    virtual void setDemoApplicationInterface(DemoApplicationInterface &application) = 0;

    /*!
     * Sets a demo application controlled value.
     */
    virtual void setDemoApplicationControlledValue(int value) = 0;
};

Q_DECLARE_INTERFACE(DemoApplicationExtensionInterface, "com.meego.core.DemoApplicationExtensionInterface/1.0")

#endif /* DEMOAPPLICATIONEXTENSIONINTERFACE_H_ */
