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

#ifndef MBUBBLEITEMVIEW_H__
#define MBUBBLEITEMVIEW_H__

#include "mwidgetview.h"
#include <MImageWidget>
#include "mimagewidgetview.h"
#include "mbubbleitemmodel.h"
#include "mbubbleitemstyle.h"
#include "mbubbleitembackgroundstyle.h"

class MBubbleItem;
class MBubbleItemViewPrivate;

/*!
    \class MBubbleItemView
    \brief View class for MBubbleItem.

    \ingroup views

    \section MBubbleItemView Overview
        MBubbleItemView draws a MBubbleItem in 2 text lines and an avatar, and a speech bubble in the background.

        \image html bubble-item.png
  */
class M_VIEWS_EXPORT MBubbleItemView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MBubbleItemModel, MBubbleItemStyle)

public:
    MBubbleItemView(MBubbleItem *controller);
    virtual ~MBubbleItemView();

protected slots:
    virtual void updateData(const QList<const char *> &modifications);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected:
    //! \reimp
    virtual void setupModel();
    virtual void applyStyle();
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

    void setSelected(bool selected);

    //! \internal
    MBubbleItemView(MBubbleItemViewPrivate &dd, MBubbleItem *controller);
    //! \internal_end

private:
    Q_DISABLE_COPY(MBubbleItemView)
    Q_DECLARE_PRIVATE(MBubbleItemView)
};

#endif
