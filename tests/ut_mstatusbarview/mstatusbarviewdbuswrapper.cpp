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

#include "mstatusbarviewdbuswrapper.h"

MStatusBarViewDBusWrapper *MStatusBarViewDBusWrapper::self = 0;
bool MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = false;

MStatusBarViewDBusWrapper::MStatusBarViewDBusWrapper(QObject *parent)
    : QObject(parent)
{
    self = this;
    m_havePendingSharedPixmapHandleReply = false;
    m_openStatusIndicatorMenuCalled = false;
}

MStatusBarViewDBusWrapper::~MStatusBarViewDBusWrapper()
{
    self = 0;
}

bool MStatusBarViewDBusWrapper::isPixmapProviderServiceRegistered()
{
    return m_isPixmapProviderServiceRegistered;
}

void MStatusBarViewDBusWrapper::querySharedPixmapHandleFromProvider()
{
    if (m_havePendingSharedPixmapHandleReply == true) {
        qFatal("Called querySharedPixmapHandleFromProvider() again before reply");
    }

    m_havePendingSharedPixmapHandleReply = true;
}

bool MStatusBarViewDBusWrapper::havePendingSharedPixmapHandleReply()
{
    return m_havePendingSharedPixmapHandleReply;
}

void MStatusBarViewDBusWrapper::openStatusIndicatorMenu()
{
    m_openStatusIndicatorMenuCalled = true;
}

void MStatusBarViewDBusWrapper::emitPixmapProviderServiceRegistered()
{
    emit pixmapProviderServiceRegistered();
}

void MStatusBarViewDBusWrapper::emitPixmapProviderServiceUnregistered()
{
    emit pixmapProviderServiceUnregistered();
}

void MStatusBarViewDBusWrapper::emitSharedPixmapHandleFromProviderReceived(quint32 handle, bool ok)
{
    emit sharedPixmapHandleFromProviderReceived(handle, ok);
}
