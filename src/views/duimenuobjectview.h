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

#ifndef DUIMENUOBJECTVIEW_H
#define DUIMENUOBJECTVIEW_H

#include "duiscenewindowview.h"
#include <duiapplicationmenustyle.h>
#include <duiapplicationmenumodel.h>
#include <duinamespace.h>

class DuiApplicationMenu;
class DuiMenuObjectViewPrivate;

/*!
 * \class DuiMenuObjectView
 * \brief DuiMenuObjectView implements an object view for the DuiMenu widget
 */

class DUI_EXPORT DuiMenuObjectView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiApplicationMenuModel, DuiApplicationMenuStyle)

protected:
    DuiMenuObjectViewPrivate *const d_ptr;

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the DuiMenu
     */
    DuiMenuObjectView(DuiApplicationMenu *controller);

    /*!
     * \brief Destructor
     */
    virtual ~DuiMenuObjectView();

protected:

    //! \reimp
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

    DuiMenuObjectView(DuiMenuObjectViewPrivate &dd, DuiApplicationMenu *controller);

private:
    Q_DISABLE_COPY(DuiMenuObjectView)
    Q_DECLARE_PRIVATE(DuiMenuObjectView)

#ifdef UNIT_TEST
    friend class Pt_DuiMenu;
#endif

};

#endif
