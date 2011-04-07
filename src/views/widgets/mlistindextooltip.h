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
 
#ifndef MLISTINDEXTOOLTIP_H
#define MLISTINDEXTOOLTIP_H

#include <MStylableWidget>
#include "mlistindextooltipstyle.h"

class MListIndexTooltipPrivate;

class MListIndexTooltip : public MStylableWidget
{
    Q_OBJECT

public:
    MListIndexTooltip(QGraphicsItem *parent=NULL);
    virtual ~MListIndexTooltip();

    void setTitle(const QString &title);

protected:
private:
    Q_DECLARE_PRIVATE(MListIndexTooltip)
    MListIndexTooltipPrivate *d_ptr;
    M_STYLABLE_WIDGET(MListIndexTooltipStyle)

#ifdef UNIT_TEST
    friend class Ut_MListIndexFloatingView;
#endif
};

#endif // MLISTINDEXTOOLTIP_H
