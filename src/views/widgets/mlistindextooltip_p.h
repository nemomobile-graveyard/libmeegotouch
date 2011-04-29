/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 
#ifndef MLISTINDEXTOOLTIP_P_H
#define MLISTINDEXTOOLTIP_P_H

#include <QObject>

class MLabel;
class MListIndexTooltip;
class MWidget;

class QGraphicsWidget;
class QGraphicsAnchorLayout;
class QPropertyAnimation;

class MListIndexTooltipPrivate
{
public:
    MListIndexTooltipPrivate();
    virtual ~MListIndexTooltipPrivate();

    void init();

    void setTitle(const QString &title);
    MLabel *titleLabel();

    void _q_updateSizeToCentralWidget();

private:
    Q_DECLARE_PUBLIC(MListIndexTooltip)
    MListIndexTooltip *q_ptr;

    QGraphicsAnchorLayout *layout;
    MLabel *titleLabelWidget;

#ifdef UNIT_TEST
    friend class Ut_MListIndexFloatingView;
#endif
};

#endif // MLISTINDEXTOOLTIP_P_H
