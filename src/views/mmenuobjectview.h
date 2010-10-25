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

#ifndef MMENUOBJECTVIEW_H
#define MMENUOBJECTVIEW_H

#include "mscenewindowview.h"
#include <mapplicationmenustyle.h>
#include <mapplicationmenumodel.h>
#include <mnamespace.h>

class MApplicationMenu;
class MMenuObjectViewPrivate;

/*!
 * \class MMenuObjectView
 * \brief MMenuObjectView implements an object view for the MMenu widget
 */

class M_VIEWS_EXPORT MMenuObjectView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MApplicationMenuModel, MApplicationMenuStyle)

protected:
    MMenuObjectViewPrivate *const d_ptr;

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the MMenu
     */
    MMenuObjectView(MApplicationMenu *controller);

    /*!
     * \brief Destructor
     */
    virtual ~MMenuObjectView();

protected:

    //! \reimp
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

    MMenuObjectView(MMenuObjectViewPrivate &dd, MApplicationMenu *controller);

private:
    Q_DISABLE_COPY(MMenuObjectView)
    Q_DECLARE_PRIVATE(MMenuObjectView)

#ifdef UNIT_TEST
    friend class Pt_MMenu;
#endif

};

#endif
