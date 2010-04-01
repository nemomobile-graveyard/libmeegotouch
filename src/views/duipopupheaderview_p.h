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

#ifndef DUIPOPUPHEADERVIEW_P_H
#define DUIPOPUPHEADERVIEW_P_H

#include "private/duiwidgetview_p.h"

class QGraphicsGridLayout;
class DuiPopupHeader;
class DuiImage;
class DuiLabel;

class DuiPopupHeaderViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiPopupHeaderView)

public:
    DuiPopupHeaderViewPrivate();
    virtual ~DuiPopupHeaderViewPrivate();

    virtual void init();

    virtual void initLayout();

    DuiPopupHeader    *controller;
    QGraphicsGridLayout    *layout;
    DuiLabel       *title;
    DuiLabel       *subtitle;
    DuiImage       *thumbnail;
};

#endif
