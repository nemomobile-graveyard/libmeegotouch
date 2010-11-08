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

#ifndef MLINEARLAYOUTPOLICY_P_H
#define MLINEARLAYOUTPOLICY_P_H

#include "mlinearlayoutpolicy.h"
#include "mabstractlayoutpolicy_p.h"
#include "mnamespace.h"

class QGraphicsLinearLayout;
class QGraphicsWidget;
class MLayout;
class ProxyItem;

/**
    This is the private implementation class for the grid layout policy.
    The real work is done in the QGridLayoutEngine to which a pointer is
    held here.
*/
class MLinearLayoutPolicyPrivate : public MAbstractLayoutPolicyPrivate
{
    Q_DECLARE_PUBLIC(MLinearLayoutPolicy)

public:
    /** Constructor */
    MLinearLayoutPolicyPrivate(MLayout *l, Qt::Orientation o);
    /** Destructor */
    virtual ~MLinearLayoutPolicyPrivate();

    void fixIndex(int *index) const;
    void refreshEngine();
    void refreshWidget();

    void notifyWidgetOfLayoutPosition(int index, M::Position position);
    void notifyAffectedWidgetsOfLayoutPosition(int index, bool add);
    void notifyAllWidgetsOfLayoutPosition();

    QGraphicsWidget *const engineWidget;
    QGraphicsLinearLayout *const engine;
    /** We need to keep track of the number of rows/columns in the layout, since
     *  QGraphicsLinearLayout does not expose this information to us.  This is
     *  basically q->count() + the number of stretches added/inserted. */
    int rowCount;

    bool notifyWidgetsOfLayoutPositionEnabled;
};

#endif // Header Guard
