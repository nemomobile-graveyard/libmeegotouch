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

#ifndef MCOLORWIDGET_H
#define MCOLORWIDGET_H

#include "mstylablewidget.h"
#include "mcolorwidgetstyle.h"

#include <QColor>

//! \internal

/**
 * \brief Simple stylable widget to provide just a colored rectangle.
 */

class MColorWidget : public MStylableWidget
{
    Q_OBJECT
    M_STYLABLE_WIDGET(MColorWidgetStyle)
    Q_DISABLE_COPY(MColorWidget)

public:

    MColorWidget(QGraphicsItem *parent);

    void setColor(const QColor &color);

    QColor color() const;

protected:

    //! \reimp

    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    //! \reimp_end

private:

    QColor m_color;
};

//! \internal_end

#endif
