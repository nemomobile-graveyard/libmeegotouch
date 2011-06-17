/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MPOSITIONINDICATORVIEW_H
#define MPOSITIONINDICATORVIEW_H

#include "mwidgetview.h"
#include <mpositionindicatormodel.h>
#include <mpositionindicatorstyle.h>

class MPositionIndicator;
class QGraphicsSceneResizeEvent;
class QRegion;
class MPositionIndicatorViewPrivate;

/*!
  \class MPositionIndicatorView
  \brief MPositionIndicatorView implements a view for MPositionIndicator

  This class draws indicators on the right edge of a pannable viewport
  (vertical panning), on the bottom edge (horizontal panning), or
  both. Position indicators are drawn only when the panned widget is
  moving, and the indicator hides itself when the panned widget stops
  moving.

*/

class M_VIEWS_EXPORT MPositionIndicatorView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MPositionIndicatorModel, MPositionIndicatorStyle)

    /*!
      \property MPositionIndicatorView::contentOpacity
      \brief Opacity of the position-indicator content. This property is
             used instead of QGraphicsObject::opacity because of the undocumented
             Qt behavior described at QTBUG-18267: Graphics widgets with
             an opacity of 0 are handled like invisible widgets and hence don't
             receive any events.
    */
    Q_PROPERTY(qreal contentOpacity READ contentOpacity WRITE setContentOpacity)

public:
    /*!
      \brief Constructor
    */
    MPositionIndicatorView(MPositionIndicator *controller);

    /*!
      \brief Destructor
    */
    virtual ~MPositionIndicatorView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void changeEvent(QEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    qreal contentOpacity() const;
    void setContentOpacity(qreal opacity);

private Q_SLOTS:
    /*!
      \brief Hides the position indicator
     */
    virtual void hide();

private:
    /*!
      \brief Restarts timer which is used to hide position indicator after pannable widget has stopped moving
     */
    void resetHideTimer();

    Q_DISABLE_COPY(MPositionIndicatorView)
    Q_DECLARE_PRIVATE(MPositionIndicatorView)

    Q_PRIVATE_SLOT(d_func(), void _q_displayEntered())
    Q_PRIVATE_SLOT(d_func(), void _q_displayExited())
    Q_PRIVATE_SLOT(d_func(), void _q_visibleChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_showTemporarily())

    friend class  Ut_MPositionIndicatorView;
};

#endif
