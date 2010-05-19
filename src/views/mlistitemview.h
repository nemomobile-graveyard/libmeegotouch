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

#ifndef MLISTITEMVIEW_H
#define MLISTITEMVIEW_H

#include <MWidgetView>

#include <mlistitemmodel.h>
#include <mlistitemstyle.h>

class MListItemViewPrivate;

class MListItemView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MListItemModel, MListItemStyle)

public:
    MListItemView(MWidgetController *controller);
   
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
    void cancelEvent(MCancelEvent *event);
      
private:
    Q_DISABLE_COPY(MListItemView)
    Q_DECLARE_PRIVATE(MListItemView)
};

#endif // MLISTITEMVIEW_H
