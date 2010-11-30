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

#ifndef MBUTTONTABVIEW_H
#define MBUTTONTABVIEW_H

#include <mbuttonview.h>
#include <mbuttonmodel.h>
#include <mbuttonstyle.h>

class MButtonTabViewPrivate;
class MButton;

class QGraphicsSceneResizeEvent;

/*!
    \class MButtonTabView
    \brief View class for tab bar buttons.

    \ingroup views

    This view class provides a view for a button that's placed in a tab bar.
    In comparison to standard button reacting on mouse release, it reacts on mouse press,
    just like tabs in common toolkits.

    In order for this view to work correctly, the button must be checkable.

    \sa MButton MButtonView MButtonStyle MToolBar
*/

class M_VIEWS_EXPORT MButtonTabView : public MButtonView
{
    Q_OBJECT
    M_VIEW(MButtonModel, MButtonStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    MButtonTabView(MButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MButtonTabView();

protected:

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    //! \reimp_end

    //! \internal
    MButtonTabView(MButtonTabViewPrivate &dd, MButton *controller);
    //! \internal_end

private:
    Q_DISABLE_COPY(MButtonTabView)
    Q_DECLARE_PRIVATE(MButtonTabView)

};

#endif
