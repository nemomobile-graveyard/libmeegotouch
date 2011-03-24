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

#ifndef MOBJECTMENUVIEW_H
#define MOBJECTMENUVIEW_H

#include "mscenewindowview.h"
#include "mobjectmenumodel.h"
#include "mobjectmenustyle.h"
#include <mnamespace.h>

class MObjectMenu;
class QGraphicsSceneMouseEvent;
class MObjectMenuViewPrivate;

/*!
 * \class MObjectMenuView
 * \brief MObjectMenuView implements an object view "frogfoot" for the MApplicationMenu widget
 */

class M_VIEWS_EXPORT MObjectMenuView : public MSceneWindowView
{
    Q_OBJECT
    Q_DISABLE_COPY(MObjectMenuView)
    Q_DECLARE_PRIVATE(MObjectMenuView)
    M_VIEW(MObjectMenuModel, MObjectMenuStyle)

protected:
    MObjectMenuView(MObjectMenuViewPrivate &dd, MObjectMenu *controller);
    MObjectMenuViewPrivate *const d_ptr;

public:
    /*!
     * \brief Constructor
     * \param controller Pointer to the MObjectMenu
     */
    MObjectMenuView(MObjectMenu *controller);

    /*!
     * \brief Destructor
     */
    virtual ~MObjectMenuView();

protected:

    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

protected slots:
    void actionAdded(MAction *action);
    void actionModified(MAction *action);
    void actionRemoved(MAction *action);
private:
    Q_PRIVATE_SLOT(d_func(), void contentActionTriggered())
    Q_PRIVATE_SLOT(d_func(), void appearing())

#ifdef UNIT_TEST
    friend class Ut_MObjectMenuView;
#endif
};

#endif
