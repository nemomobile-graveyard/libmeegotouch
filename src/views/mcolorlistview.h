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

#ifndef MCOLORLISTVEIW_H
#define MCOLORLISTVEIW_H

#include <MWidgetView>
#include <QObject>

#include "mcolorlistmodel.h"
#include "mcolorliststyle.h"

class MButton;
class MColorList;
class MColorListViewPrivate;

/**
 * \brief View class for MColorList.
 */
class M_VIEWS_EXPORT MColorListView : public MWidgetView
{
    Q_OBJECT
    MColorListViewPrivate *d_ptr;
    M_VIEW(MColorListModel, MColorListStyle)
    Q_DISABLE_COPY(MColorListView)
    Q_DECLARE_PRIVATE(MColorListView)

public:

    MColorListView(MColorList *controller);

    virtual ~MColorListView();

protected:

    //! \reimp

    virtual void applyStyle();

    virtual void setupModel();

    virtual void updateData(const QList<const char*> &modifications);

    //! \reimp_end

private slots:

    void buttonClicked(MButton *button);
};

#endif
