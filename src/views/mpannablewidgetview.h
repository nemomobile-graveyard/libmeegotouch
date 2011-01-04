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

#ifndef MPANNABLEWIDGETVIEW_H
#define MPANNABLEWIDGETVIEW_H

#include "mwidgetview.h"
#include <mpannablewidgetmodel.h>
#include <mpannablewidgetstyle.h>

class MPannableWidgetViewPrivate;
class MPannableWidget;

/*!
 * \class MPannableWidgetView
 * \brief MPannableWidgetView implements the view for pannable widget
 */
class M_VIEWS_EXPORT MPannableWidgetView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MPannableWidgetModel, MPannableWidgetStyle)

public:
    MPannableWidgetView(MPannableWidget *controller);
    virtual ~MPannableWidgetView();

protected:
    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end
    MPannableWidgetViewPrivate *const d_ptr;

private:

    Q_PRIVATE_SLOT(d_func(), void _q_applyStyleToPhysics())

    Q_DECLARE_PRIVATE(MPannableWidgetView)
    Q_DISABLE_COPY(MPannableWidgetView)
};

#endif
