/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef ACCESSMANAGER_H
#define ACCESSMANAGER_H

/*
 * automatically generated with the command line :
 * m-servicefwgen -p com.nokia.widgetsgallery.AccessManager
 */

#include <QObject>

#include <mservicefwbaseif.h>

class AccessManager : public MServiceFwBaseIf
{
Q_OBJECT

public:
    bool login( bool fullscreen, bool autofocus, int orientation );
    bool login( bool fullscreen, bool autofocus, int orientation, const QString &_taskTitle );

public:
    /*!
     * @brief Constructs a base interface
     * @param preferredService the preferred service provider. Leave
     * empty if no preferred provider. In most cases, this should be left
     * empty.
     * @param parent Parent object
     */

    AccessManager( const QString& preferredService = "", QObject* parent = 0 );

    /*!
     * @brief Set the service name
     * @param service Name of the desired service
     */

    void setService(const QString & service);
Q_SIGNALS:

};
#endif

