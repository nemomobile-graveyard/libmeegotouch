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

#ifndef DUICOMPLETERVIEW_H
#define DUICOMPLETERVIEW_H

#include "duiscenewindowview.h"
#include "duicompletermodel.h"
#include "duicompleterstyle.h"

class DuiCompleter;
class DuiCompleterViewPrivate;

class DUI_EXPORT DuiCompleterView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiCompleterModel, DuiCompleterStyle)

public:
    DuiCompleterView(DuiCompleter *controller);
    virtual ~DuiCompleterView();

    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

private:
    DuiCompleterViewPrivate *const d_ptr;

    Q_DISABLE_COPY(DuiCompleterView)
    Q_DECLARE_PRIVATE(DuiCompleterView)
};

#endif

