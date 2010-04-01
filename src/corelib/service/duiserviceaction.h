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

#ifndef DUISERVICEACTION_H
#define DUISERVICEACTION_H

#include "duiaction.h"

#include "duiexport.h"

class DuiServiceActionPrivate;

/*!
 * Base class for actions that can be used to launch services.
 */
class DUI_EXPORT DuiServiceAction : public DuiAction
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    explicit DuiServiceAction(QObject *parent);

    /*!
     * Destructor.
     */
    virtual ~DuiServiceAction();

public slots:
    /*!
     * Executes the service of this action.
     */
    virtual void executeService();

protected:
    /*!
     * Constructor to be used from inheriting classes.
     */
    DuiServiceAction(DuiServiceActionPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(DuiServiceAction)
    Q_DISABLE_COPY(DuiServiceAction)

};


#endif // DUISERVICEACTION_H
