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


#ifndef MBASICSHEETHEADERVIEW_P_H
#define MBASICSHEETHEADERVIEW_P_H

#include <QObject>

class MBasicSheetHeader;
class MBasicSheetHeaderView;
class MButton;
class MProgressIndicator;

class QAction;
class QGraphicsAnchorLayout;

class MBasicSheetHeaderViewPrivate
{
public:
    MBasicSheetHeaderViewPrivate();
    virtual ~MBasicSheetHeaderViewPrivate();

    void createLayout();

    void setPositiveAction(QAction *action);
    void setNegativeAction(QAction *action);
    void setProgressIndicatorVisible(bool visible);

    void _q_refreshPositiveAction();
    void _q_refreshNegativeAction();

    void refreshStyles();

    Q_DECLARE_PUBLIC(MBasicSheetHeaderView)
    MBasicSheetHeaderView *q_ptr;

    MBasicSheetHeader* controller;
    QGraphicsAnchorLayout *layout;

    MButton *positiveButton;
    MButton *negativeButton;
    MProgressIndicator *progressIndicator;
};

#endif // MBASICSHEETHEADERVIEW_P_H
