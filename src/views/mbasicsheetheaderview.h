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


#ifndef MBASICSHEETHEADERVIEW_H
#define MBASICSHEETHEADERVIEW_H

#include <MWidgetView>

#include "mbasicsheetheaderstyle.h"
#include "mbasicsheetheadermodel.h"

class MBasicSheetHeader;
class MBasicSheetHeaderViewPrivate;

class M_VIEWS_EXPORT MBasicSheetHeaderView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MBasicSheetHeaderModel, MBasicSheetHeaderStyle)

public:
    MBasicSheetHeaderView(MBasicSheetHeader *controller);
    virtual ~MBasicSheetHeaderView();

protected:
    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    virtual void applyStyle();
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MBasicSheetHeaderView)
    MBasicSheetHeaderViewPrivate *d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_refreshPositiveAction())
    Q_PRIVATE_SLOT(d_func(), void _q_refreshNegativeAction())

#ifdef UNIT_TEST
    friend class Ut_MBasicSheetHeaderView;
#endif // UNIT_TEST
};

#endif // MBASICSHEETHEADERVIEW_H
