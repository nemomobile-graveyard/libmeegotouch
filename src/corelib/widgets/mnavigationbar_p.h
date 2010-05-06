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

#ifndef MNAVIGATIONBAR_P_H
#define MNAVIGATIONBAR_P_H

#include <mscenewindow_p.h>

#include "mnavigationbar.h"
#include "mhomebuttonpanel.h"

#include <QPointer>

class QGraphicsLinearLayout;
class QGraphicsGridLayout;
class MButton;
class QGraphicsWidget;
class MStyle;
class QGraphicsItemAnimation;
class QTimeLine;

class MNavigationBarPrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MNavigationBar)

public:
    MNavigationBarPrivate();
    virtual ~MNavigationBarPrivate();

    void init();

    QPointer<MHomeButtonPanel> homeButtonPanel;
};

#endif
