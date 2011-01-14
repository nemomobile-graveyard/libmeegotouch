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

#ifndef UT_MAPPLICATIONEXTENSIONLOADER_H
#define UT_MAPPLICATIONEXTENSIONLOADER_H

#include <QObject>
#include <mapplicationextensioninterface.h>

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

Q_DECLARE_INTERFACE(BadApplicationExtensionInterface, "com.meego.core.BadApplicationExtensionInterface/1.0")

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
class GoodExtension : public QObject, public MApplicationExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

    bool success;
    QGraphicsWidget *widget_;
};

class MApplicationExtensionMetaData;

class Ut_MApplicationExtensionLoader : public QObject
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
    void testExtensionLoadingUsesCorrectLoaderHints();

private:
    MApplicationExtensionMetaData *metadata;
};

#endif
