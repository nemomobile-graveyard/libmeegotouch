/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBARVIEWDBUSWRAPPER_H
#define MSTATUSBARVIEWDBUSWRAPPER_H

#include <QObject>

class MStatusBarViewDBusWrapper : public QObject
{
    Q_OBJECT
public:
    MStatusBarViewDBusWrapper(QObject *parent = 0);
    virtual ~MStatusBarViewDBusWrapper();

    bool isPixmapProviderServiceRegistered();

    // Will cause sharedPixmapHandleFromProviderReceived() to be
    // eventually emited
    void querySharedPixmapHandleFromProvider();

    bool havePendingSharedPixmapHandleReply();

    void openStatusIndicatorMenu();

Q_SIGNALS:
    void pixmapProviderServiceRegistered();
    void pixmapProviderServiceUnregistered();

    // \param ok true if reply was successful and false otherwise
    void sharedPixmapHandleFromProviderReceived(quint32 handle, bool ok);

public:
    static MStatusBarViewDBusWrapper *self;
    static bool m_isPixmapProviderServiceRegistered;
    bool m_havePendingSharedPixmapHandleReply;
    bool m_openStatusIndicatorMenuCalled;

    void emitPixmapProviderServiceRegistered();
    void emitPixmapProviderServiceUnregistered();
    void emitSharedPixmapHandleFromProviderReceived(quint32 handle, bool ok);
};

#endif // MSTATUSBARVIEWDBUSWRAPPER_H
