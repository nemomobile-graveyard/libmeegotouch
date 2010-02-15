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

#ifndef DUIPANNABLEWIDGETVIEW_H
#define DUIPANNABLEWIDGETVIEW_H

#include "duiwidgetview.h"
#include <duipannablewidgetmodel.h>
#include <duipannablewidgetstyle.h>

class DuiPannableWidgetViewPrivate;
class DuiPannableWidget;

/*!
 * \class DuiPannableWidgetView
 * \brief DuiPannableWidgetView implements the view for pannable widget
 */
class DUI_EXPORT DuiPannableWidgetView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiPannableWidgetModel, DuiPannableWidgetStyle)

public:
    DuiPannableWidgetView(DuiPannableWidget *controller);
    virtual ~DuiPannableWidgetView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end

protected:
    DuiPannableWidgetViewPrivate *const d_ptr;
private:
    Q_DECLARE_PRIVATE(DuiPannableWidgetView)
    Q_DISABLE_COPY(DuiPannableWidgetView)
};

#endif
