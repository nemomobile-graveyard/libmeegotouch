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

#ifndef MEXTENSIONAREA_H
#define MEXTENSIONAREA_H

#include <MWidgetController>
#include "mextensionareamodel.h"

class MDataStore;
class MExtensionAreaPrivate;

/*!
 * MExtensionArea is a baseclass for widgets that can load application extensions
 */
class M_EXTENSIONS_EXPORT MExtensionArea : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MExtensionArea)

public:
    /*!
     * Default constructor
     * \param parent Optional Object's parent
     */
    explicit MExtensionArea(QGraphicsItem *parent = NULL);

    /*!
     * Default destructor
     */
    virtual ~MExtensionArea();

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    MExtensionArea(MExtensionAreaPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);
    MExtensionAreaPrivate *const d_ptr;

protected Q_SLOTS:
    /*!
     * addWidget performs setup of an extension widget and its datastore when the widget is being added to
     * the extension area. The ownership of the widget remains on the caller of this slot.
     * This slot can be overridden in specialized extension areas to provide additional setup.
     * \param widget Widget to be added onto the MExtensionArea.
     * \param store This MDataStore object can be used to store permanent extension area data related
     * to this particular application extension instance. When the same application extension instance is
     * reinstantiated, this API will be called with the data that was stored to the permanent storage the
     * last time around. This can be used to store for instance layout data of an application extension instance
     * or any other extension area specific data.
     */
    virtual void addWidget(QGraphicsWidget *widget, MDataStore &store);

    /*!
     * removeWidget performs cleanup related to a widget that is being removed.
     * The widget itself should not be deleted in this method, it will be deleted by the caller.
     *
     * \param widget The widget to be removed from the system.
     */
    virtual void removeWidget(QGraphicsWidget *widget);

private:
    Q_DECLARE_PRIVATE(MExtensionArea)
    Q_DISABLE_COPY(MExtensionArea)
};

#endif // MEXTENSIONAREA_H
