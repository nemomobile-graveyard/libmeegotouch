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

#ifndef MEXTENSIONAREA_P_H
#define MEXTENSIONAREA_P_H

#include "mextensionarea.h"

#include <QTapAndHoldGesture>

class MDataStore;

/*!
 * Private class for MExtensionArea.
 */
class MExtensionAreaPrivate
{
    Q_DECLARE_PUBLIC(MExtensionArea)

public:
    /*!
     * Constructor.
     */
    MExtensionAreaPrivate();

    /*!
     * Destructor.
     */
    virtual ~MExtensionAreaPrivate();

    /*!
     * Adds a widget to the canvas.
     * \see MExtensionArea::addWidget()
     */
    virtual void addWidget(QGraphicsWidget *widget, MDataStore &store);

    /*!
     * Removes a widget from the canvas.
     * \see MExtensionArea::removeWidget()
     */
    virtual void removeWidget(QGraphicsWidget *widget);

    virtual void gestureEvent(QGestureEvent *event);
    virtual void tapAndHoldGesture(QGestureEvent *event, QTapAndHoldGesture *state);

    //! Map to maintain MDataStore objects of associated widgets in.
    QMap<QGraphicsWidget *, MDataStore *> dataStores;

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     */
    void init();
protected:
    MExtensionArea *q_ptr;
};

#endif // MEXTENSIONAREA_P_H

