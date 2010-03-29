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

#ifndef DUIAPPLETINSTANTIATOR_H_
#define DUIAPPLETINSTANTIATOR_H_

#include <QString>
#include <QHash>
#include <QObject>

class DuiAppletInstantiatorPrivate;
class QDBusPendingCallWatcher;

//! \internal
/*!
 * Instantiates applets from a given package to the given mashup canvas.
 */
class DuiAppletInstantiator : public QObject
{
    Q_OBJECT

private:
    /*!
     * Constructs a new DuiAppletInstantiator.
     */
    DuiAppletInstantiator();

    /*!
     * Destroys the DuiAppletInstantiator.
     */
    virtual ~DuiAppletInstantiator();

public:
    /*!
     * Returns an instance of the applet instantiator.
     *
     * \return an instance of the applet instantiator
     */
    static DuiAppletInstantiator *instance();

    /*!
     * Instantiates applets in a package to the given mashup canvas.
     *
     * \param packageName the name of the package to instantiate applets from
     * \param canvasDBusAddress the D-Bus address of the canvas to instantiate to in service/path format
     */
    void instantiateAppletsInPackage(const QString &packageName, const QString &canvasDBusAddress);

private:
    // The private class
    DuiAppletInstantiatorPrivate *const d_ptr;

    Q_DECLARE_PRIVATE(DuiAppletInstantiator)
    Q_PRIVATE_SLOT(d_func(), void receivePackageData(QDBusPendingCallWatcher *))
};
//! \internal_end

#endif /* DUIAPPLETINSTANTIATOR_H_ */
