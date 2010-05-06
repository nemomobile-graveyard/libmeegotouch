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

#ifndef MWIDGETVIEW_P_H
#define MWIDGETVIEW_P_H

#include "mwidgetview.h"

class MWidgetController;
class QPixmap;
class MScalableImage;
class MAbstractWidgetAnimation;

class M_EXPORT MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MWidgetView)

protected:
    MWidgetView *q_ptr;
public: // public to inherited classes
    MWidgetViewPrivate();
    virtual ~MWidgetViewPrivate();

//private: // public only to MWidgetView
    MWidgetController     *controller;
private:// public only to MWidgetView
    MWidgetModel *model;
    MWidgetStyleContainer *styleContainer;
};

#endif // MWIDGETVIEW_P_H
