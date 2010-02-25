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

#ifndef UT_DUIAPPLICATIONEXTENSIONLOADER_H
#define UT_DUIAPPLICATIONEXTENSIONLOADER_H

#include <QObject>
#include <duiapplicationextensioninterface.h>

/*!
 * A QObject whose destruction can be tracked
 */
class SomeQObject : public QObject
{
    Q_OBJECT

public:
    virtual ~SomeQObject();
};

/*!
 * A bad application extension interface that does not derive from the base class
 */
class BadApplicationExtensionInterface
{
};

Q_DECLARE_INTERFACE(BadApplicationExtensionInterface, "com.nokia.dui.core.BadApplicationExtensionInterface/1.0")

/*!
 * A bad application extension that does not derive from the correct interface
 */
class BadExtension : public QObject, public BadApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(BadApplicationExtensionInterface)
};

/*!
 * A good application extension that does derive from the correct interface
 */
class GoodExtension : public QObject, public DuiApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiApplicationExtensionInterface)

public:
    virtual bool initialize();
    virtual DuiWidget *widget();

    bool success;
    DuiWidget *widget_;
};

class DuiApplicationExtensionMetaData;

class Ut_DuiApplicationExtensionLoader : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testExtensionLoading();
    void testExtensionLoadingFailNullExtensionObject();
    void testExtensionLoadingFailWrongTypeExtensionObject();
    void testExtensionLoadingFailWrongInterfaceExtensionObject();

private:
    DuiApplicationExtensionMetaData *metadata;
};

#endif
