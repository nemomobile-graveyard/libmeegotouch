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

// Make doxygen skip this internal class
//! \cond
#ifndef DUIPANNABLEVIEWPORTLAYOUT_H
#define DUIPANNABLEVIEWPORTLAYOUT_H

#include <QGraphicsLinearLayout>

class DuiPannableViewportLayout : public QGraphicsLinearLayout
{
public:
    DuiPannableViewportLayout(QGraphicsLayoutItem *parent = 0);

    void setPanningDirections(const Qt::Orientations &panningDirections);

    void setWidget(QGraphicsWidget *widget);

    //! \reimp
    void setGeometry(const QRectF &rect);
    //! \reimp_end

private:
    QGraphicsWidget *pannedWidget;
    Qt::Orientations panningDirections;
};

#endif
//! \endcond
