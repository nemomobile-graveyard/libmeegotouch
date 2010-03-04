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

#ifndef TEXTPROCESSORINTERFACE_H
#define TEXTPROCESSORINTERFACE_H

/*
 * automatically generated with the command line :
 * dui-servicefwgen -p com.nokia.TextProcessorInterface
 */

#include <QObject>

#include <duiservicefwbaseif.h>
#include <textprocessorinterfaceproxy.h>

class TextProcessorInterface : public DuiServiceFwBaseIf
{
Q_OBJECT

public:
    QString reverse( const QString &message );

public:
    /*!
     * @brief Constructs a base interface
     * @param preferredService, define the preferred service provider. Leave
       empty if no preferred provider. In most cases, this should be left
       empty.
     * @param parent Parent object
     */

    TextProcessorInterface( const QString& preferredService = "", QObject* parent = 0 );

    /*!
     * @brief Set the service name
     * @param service Name of the desired service
     */

    void setService(const QString & service);
Q_SIGNALS:


};
#endif
