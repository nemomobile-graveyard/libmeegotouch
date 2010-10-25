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

#ifndef MSERVICEACTION_H
#define MSERVICEACTION_H

#include "maction.h"

#include "mexport.h"

class MServiceActionPrivate;

/*!
 * Base class for actions that can be used to launch services.
 */
class M_CORE_EXPORT MServiceAction : public MAction
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    explicit MServiceAction(QObject *parent);

    /*!
     * Destructor.
     */
    virtual ~MServiceAction();

public slots:
    /*!
     * Executes the service of this action.
     */
    virtual void executeService();

protected:
    /*!
     * Constructor to be used from inheriting classes.
     */
    MServiceAction(MServiceActionPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(MServiceAction)
    Q_DISABLE_COPY(MServiceAction)

};


#endif // MSERVICEACTION_H
