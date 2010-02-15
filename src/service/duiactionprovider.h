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

#ifndef DUIACTIONPROVIDER_H
#define DUIACTIONPROVIDER_H

#include <QObject>
#include <QUrl>

#include "duiexport.h"

/***** TODO: remove test action when not needed any more ******/
#include "duiserviceaction.h"

//! \internal
class TestAction : public DuiServiceAction
{
    Q_OBJECT

    QString text;

public:
    TestAction(const QString &text);
    virtual ~TestAction();

public Q_SLOTS:
    /*!
     * Executes the service of this action.
     */
    virtual void executeService();


};
//! \internal_end
/***** TODO: test action ends here ******/

/*!
 * A mock action provider for testing.
 */
class DUI_EXPORT DuiActionProvider : public QObject
{
    Q_OBJECT
public:
    /*!
     * Provides a default action for a given URI. The ownership
     * of the action is transformed to the caller (the caller must
     * destroy the action when it's no longer needed).
     * If no action can be provided, this method returns \c NULL.
     * \param uri A URI for which an action is provided.
     * \return The default action for the URI or \c NULL if no
     *         action can be provided.
     */
    static DuiAction *getDefaultAction(const QUrl &uri);
};

#endif // DUIACTIONPROVIDER_H
