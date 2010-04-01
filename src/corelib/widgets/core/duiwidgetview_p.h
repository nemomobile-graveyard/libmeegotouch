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

#ifndef DUIWIDGETVIEW_P_H
#define DUIWIDGETVIEW_P_H

#include "duiwidgetview.h"

class DuiWidgetController;
class QPixmap;
class DuiScalableImage;
class DuiAbstractWidgetAnimation;

class DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiWidgetView)

protected:
    DuiWidgetView *q_ptr;
public: // public to inherited classes
    DuiWidgetViewPrivate();
    virtual ~DuiWidgetViewPrivate();

//private: // public only to DuiWidgetView
    DuiWidgetController     *controller;
private:// public only to DuiWidgetView
    DuiWidgetModel *model;
    DuiWidgetStyleContainer *styleContainer;

    QPointF contentPosition;

    DuiAbstractWidgetAnimation *showAnimation;
    DuiAbstractWidgetAnimation *hideAnimation;
};

#endif // DUIWIDGETVIEW_P_H
