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

#ifndef MSHEETVIEW_H
#define MSHEETVIEW_H

#include <msheetmodel.h>
#include <msheetstyle.h>
#include "mscenewindowview.h"

class MSheet;
class MSheetViewPrivate;

class M_VIEWS_EXPORT MSheetView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSheetModel, MSheetStyle)

public:
    MSheetView(MSheet *controller);
    virtual ~MSheetView();

protected:
    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DISABLE_COPY(MSheetView)
    Q_DECLARE_PRIVATE(MSheetView)
};

#endif // MSHEETVIEW_H
