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

// -*- C++ -*-

#ifndef DUIRMISERVER_H
#define DUIRMISERVER_H

#include <QObject>

#include "duiexport.h"

class DuiRmiServerPrivate;
class DuiRmiServerPrivateSocket;

/*!
 * \class DuiRmiServer
 * \brief The DuiRmiServer provides a way for member functions of QObject-based
 * classes to be directly invoked from another process without relying on an
 * external transport such as d-bus.
 *
 * To make a QObject-based class remotely callable from another process,
 * ensure that the object has Q_OBJECT macro and the member functions you want
 * to export are public slots themselves.
 *
 * The types of the arguments of those member functions need to be supported
 * by QVariant as well. Most Qt types are supported. This include from plain
 * old data types to complex GUI types such as QRect, QColor, QImages, and more.
 * Container types such as QList are even supported.
 *
 * If needed, custom complex classes can also be sent across the wire
 * provided the operator QVariant(), operator<<() and
 * operator>>() are overloaded, and the class is declared in Qt's metaobject
 * system using qRegisterMetaType(). Qt uses this class internally for
 * mashalling/unmarshalling types (see QMetaType for details).
 */
class DUI_EXPORT DuiRmiServer: public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a DuiRmiServer
     *
     * \param key a unique key that identifies this server
     * \param parent QObject.
     */
    explicit DuiRmiServer(const QString &key, QObject *parent = 0);

    /*!
     * Disconnects all connections and destroys this object
     */
    virtual ~DuiRmiServer();

    /*!
     * Export a QObject for remote invocation. Currently only one QObject per
     * DuiRmiServer is supported.
     *
     * \param object QObject to be exported.
     */
    void exportObject(QObject *object);

private:
    Q_DISABLE_COPY(DuiRmiServer)
    Q_DECLARE_PRIVATE(DuiRmiServer)
    Q_PRIVATE_SLOT(d_func(), void _q_incoming())
    Q_PRIVATE_SLOT(d_func(), void _q_readData())

    DuiRmiServerPrivate *const d_ptr;
    friend class DuiRmiServerPrivateSocket;
};

#endif
