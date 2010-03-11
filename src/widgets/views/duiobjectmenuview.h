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

#ifndef DUIOBJECTMENUVIEW_H
#define DUIOBJECTMENUVIEW_H

#include "duiscenewindowview.h"
#include "duiobjectmenumodel.h"
#include "duiobjectmenustyle.h"
#include <duinamespace.h>

class DuiObjectMenu;
class QGraphicsSceneMouseEvent;
class DuiObjectMenuViewPrivate;

/*!
 * \class DuiObjectMenuView
 * \brief DuiObjectMenuView implements an object view "frogfoot" for the DuiApplicationMenu widget
 */

class DUI_EXPORT DuiObjectMenuView : public DuiSceneWindowView
{
    Q_OBJECT
    Q_DISABLE_COPY(DuiObjectMenuView)
    Q_DECLARE_PRIVATE(DuiObjectMenuView)
    DUI_VIEW(DuiObjectMenuModel, DuiObjectMenuStyle)

protected:
    DuiObjectMenuView(DuiObjectMenuViewPrivate &dd, DuiObjectMenu *controller);

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the DuiObjectMenu
     */
    DuiObjectMenuView(DuiObjectMenu *controller);

    /*!
     * \brief Destructor
     */
    virtual ~DuiObjectMenuView();

protected:

    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

protected slots:
    void actionAdded(DuiAction *action);
    void actionModified(DuiAction *action);
    void actionRemoved(DuiAction *action);
private:
    Q_PRIVATE_SLOT(d_func(), void contentActionTriggered())
};

#endif
