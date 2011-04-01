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

#ifndef MAPPLICATIONBUTTONVIEW_P_H
#define MAPPLICATIONBUTTONVIEW_P_H

#include "mbuttonview_p.h"

class MLabel;
class MImageWidget;
class MProgressIndicator;
class MLayout;
class MGridLayoutPolicy;
class  QGraphicsLinearLayout;

class MApplicationMenuButtonViewPrivate : public MButtonViewPrivate
{
    Q_DECLARE_PUBLIC(MApplicationMenuButtonView)

public:
    MApplicationMenuButtonViewPrivate();
    virtual ~MApplicationMenuButtonViewPrivate();

    void init();
    void refreshLayout();
    void refreshIconImage();
    virtual void updateLabelStyle();
    virtual void updateItemsAfterModeChange();

    MImageWidget *iconImage;
    MImageWidget *arrowIconImage;
    MProgressIndicator *spinner;
    MLayout *layout;
    MGridLayoutPolicy *policy;
    QGraphicsWidget *labelSlot;
    QGraphicsLinearLayout *labelLayout;
};

#endif
