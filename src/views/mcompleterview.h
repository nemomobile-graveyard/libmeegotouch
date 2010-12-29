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

#ifndef MCOMPLETERVIEW_H
#define MCOMPLETERVIEW_H

#include "mscenewindowview.h"
#include "mcompletermodel.h"
#include "mcompleterstyle.h"

class MCompleter;
class MCompleterViewPrivate;

class M_VIEWS_EXPORT MCompleterView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MCompleterModel, MCompleterStyle)

public:
    MCompleterView(MCompleter *controller);
    virtual ~MCompleterView();

    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    MCompleterViewPrivate *const d_ptr;

    Q_DISABLE_COPY(MCompleterView)
    Q_DECLARE_PRIVATE(MCompleterView)
};

#endif

