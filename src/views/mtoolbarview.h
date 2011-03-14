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

#ifndef MTOOLBARVIEW_H
#define MTOOLBARVIEW_H

#include "mwidgetview.h"
#include <mwidgetmodel.h>
#include <mtoolbarstyle.h>
#include <mnamespace.h>

class MToolBar;
class MToolBarViewPrivate;

/*!
    \class MToolBarView
    \brief View class for MToolBar widget.

    \ingroup views

    \section MToolBarViewOverview Overview
        MToolBarView is used to visualize actions placed into the MToolBar.

        The outlook of toolbar can be changed using the styling attributes defined
        in MToolbarStyle and MWidgetStyle.

    \section MToolBarInteractions Interactions
        - The toolbar has currently no interactions by itself: the interactions are
          determined by the UI controls that are placed inside the toolbar.
        - The toolbar can be affected by possible interactions of going to and from the
          full screen mode; this would set the visibility of the toolbar on and off.
        - Setting the visibility of the toolbar on/off should be accompanied by a
          (sliding) transition, hiding and revealing the toolbar.

    \section MToolBarOpenIssues Open issues
        - Sliding transitions for hiding and revealing the toolbar are not supported yet.

    \sa MToolBar MToolbarStyle
*/

class M_VIEWS_EXPORT MToolBarView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, MToolbarStyle)

public:

    /*!
        \brief Constructs toolbar view.
        \param Pointer to the controller.
     */
    MToolBarView(MToolBar *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MToolBarView();

    MWidget *getWidget(QAction *action) const;

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end

    /*!
        \brief protected constructor
        \param dd Shared private class
        \param controller The controller associated with the view.
    */
    MToolBarView(MToolBarViewPrivate &dd, MToolBar *controller);
    MToolBarViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MToolBarView)
    Q_DECLARE_PRIVATE(MToolBarView)

    Q_PRIVATE_SLOT(d_func(), void _q_groupButtonClicked(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_groupActionToggled(bool))

#ifdef UNIT_TEST
    friend class Ut_MToolBarView;
#endif
};

#endif
