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

#ifndef MUSERGUIDEIF_H
#define MUSERGUIDEIF_H

/*
 * automatically generated with the command line :
 * m-servicefwgen -p MUserGuideIf
 */

#include <QObject>

#include <mservicefwbaseif.h>
#include <mexport.h>

class M_EXPORT MUserGuideIf : public MServiceFwBaseIf
{
Q_OBJECT

public:
    bool mainPage();
    bool pageByPath( const QString &path );
    bool pageByPathChained( const QString &path );
    bool pageByPathChained( const QString &path, const QString &_taskTitle );

public:
    /*!
     * @brief Constructs a base interface
     * @param preferredService the preferred service provider. Leave
     * empty if no preferred provider. In most cases, this should be left
     * empty.
     * @param parent Parent object
     */

    MUserGuideIf( const QString& preferredService = "", QObject* parent = 0 );

    /*!
     * @brief Set the service name
     * @param service Name of the desired service
     */

    void setService(const QString & service);
Q_SIGNALS:

};
#endif

