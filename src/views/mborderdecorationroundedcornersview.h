/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MBORDERDECORATIONROUNDEDCORNERSVIEW_H
#define MBORDERDECORATIONROUNDEDCORNERSVIEW_H
//! \internal

#include <mscenewindowmodel.h>
#include <mborderdecorationroundedcornersstyle.h>
#include <mscenewindowview.h>
#include <mstylablewidget.h>
#include <mstylablewidgetstyle.h>


class MBorderDecoration;
class MRoundedCorner;

class MBorderDecorationRoundedCornersView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MBorderDecorationRoundedCornersStyle)

public:
    MBorderDecorationRoundedCornersView(MBorderDecoration *controller);
    virtual ~MBorderDecorationRoundedCornersView();

protected:
    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(MBorderDecorationRoundedCornersView)
    MRoundedCorner *leftCornerWidget;
    MRoundedCorner *rightCornerWidget;
};

class MRoundedCorner : public MStylableWidget
{
  Q_OBJECT

public:
    MRoundedCorner(QGraphicsItem *parent = 0);
    virtual ~MRoundedCorner();

    M_STYLABLE_WIDGET(MStylableWidgetStyle)
};

//! \internal_end
#endif // MBORDERDECORATIONROUNDEDCORNERSVIEW_H
