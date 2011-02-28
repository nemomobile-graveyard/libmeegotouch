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

#ifndef MCOLORLISTVIEW_P_H
#define MCOLORLISTVIEW_P_H

#include <QList>
#include <QColor>

class MColorList;
class MColorListView;
class MButtonGroup;
class MGridLayoutPolicy;

//! \internal

/**
 * \brief Private part of MColorListView.
 */
class MColorListViewPrivate
{
    MColorListView *q_ptr;
    Q_DISABLE_COPY(MColorListViewPrivate)
    Q_DECLARE_PUBLIC(MColorListView)

public:

    MColorListViewPrivate(MColorListView *p, MColorList *controller);

    ~MColorListViewPrivate();

    void updateColors();

    void updateSelectedColor();

    MColorList *controller;

    MGridLayoutPolicy *landscapePolicy;

    MGridLayoutPolicy *portraitPolicy;

    MButtonGroup *buttonGroup;

    QList<QColor> colors;
};

//! \internal_end

#endif
