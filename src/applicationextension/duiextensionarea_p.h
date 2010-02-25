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

#ifndef DUIEXTENSIONAREA_P_H
#define DUIEXTENSIONAREA_P_H

#include "private/duiwidgetcontroller_p.h"
#include "duiextensionarea.h"

class DuiDataStore;

/*!
 * Private class for DuiExtensionArea.
 */
class DuiExtensionAreaPrivate : public DuiWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(DuiExtensionArea)

public:
    /*!
     * Constructor.
     */
    DuiExtensionAreaPrivate();

    /*!
     * Destructor.
     */
    virtual ~DuiExtensionAreaPrivate();

    /*!
     * Adds a widget to the canvas.
     * \see DuiExtensionArea::addWidget()
     */
    virtual void addWidget(DuiWidget *widget, DuiDataStore &store);

    /*!
     * Removes a widget from the canvas.
     * \see DuiExtensionArea::removeWidget()
     */
    virtual void removeWidget(DuiWidget *widget);

    //! Map to maintain DuiDataStore objects of associated widgets in.
    QMap<DuiWidget *, DuiDataStore *> dataStores;

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     */
    void init();
};

#endif // DUIEXTENSIONAREA_P_H
