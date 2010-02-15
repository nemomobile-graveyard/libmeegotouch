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

#ifndef DUIPOSITIONINDICATORVIEW_H
#define DUIPOSITIONINDICATORVIEW_H

#include "duiwidgetview.h"
#include <duipositionindicatormodel.h>
#include <duipositionindicatorstyle.h>

class DuiPositionIndicator;
class QGraphicsSceneResizeEvent;
class QRegion;
class DuiPositionIndicatorViewPrivate;

/*!
  \class DuiPositionIndicatorView
  \brief DuiPositionIndicatorView implements a view for DuiPositionIndicator

  This class draws indicators on the right edge of a pannable viewport
  (vertical panning), on the bottom edge (horizontal panning), or
  both. Position indicators are drawn only when the panned widget is
  moving, and the indicator hides itself when the panned widget stops
  moving.

*/

class DUI_EXPORT DuiPositionIndicatorView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiPositionIndicatorModel, DuiPositionIndicatorStyle)

public:
    /*!
      \brief Constructor
    */
    DuiPositionIndicatorView(DuiPositionIndicator *controller);

    /*!
      \brief Destructor
    */
    virtual ~DuiPositionIndicatorView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end


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

    Q_DISABLE_COPY(DuiPositionIndicatorView)
    Q_DECLARE_PRIVATE(DuiPositionIndicatorView)

    friend class  Ut_DuiPositionIndicatorView;
};

#endif
