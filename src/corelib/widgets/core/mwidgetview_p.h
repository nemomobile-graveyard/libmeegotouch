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

class M_CORE_EXPORT MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MWidgetView)

protected:
    MWidgetView *q_ptr;
public: // public to inherited classes
    MWidgetViewPrivate();
    virtual ~MWidgetViewPrivate();

    void setActive(bool active);
    void setEnabled(bool enabled);
    void setSelected(bool selected);
    void setPressed(bool pressed);

    static void reloadChildItemsStyles(QGraphicsItem* item);

    const MWidgetStyle *currentStyle() const;

//private: // public only to MWidgetView
    MWidgetController     *controller;
private:// public only to MWidgetView
    MWidgetModel *model;
    MWidgetStyleContainer *styleContainer;
    const MStyle *appliedStyle;
    QMargins margins;
    QMargins reactiveMargins;
};

#endif // MWIDGETVIEW_P_H
