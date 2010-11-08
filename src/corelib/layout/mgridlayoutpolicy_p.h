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

#ifndef MGRIDLAYOUTPOLICY_P_H
#define MGRIDLAYOUTPOLICY_P_H

#include "mabstractlayoutpolicy_p.h"
#include "mgridlayoutpolicy.h"

class MLayout;
class QGraphicsWidget;
class QGraphicsGridLayout;
/**
    This is the private implementation class for the grid layout policy.
    The real work is done in the QGraphicsGridLayout to which a pointer is
    held here.
*/
class MGridLayoutPolicyPrivate : public MAbstractLayoutPolicyPrivate
{
    Q_DECLARE_PUBLIC(MGridLayoutPolicy)

public:
    /** Constructor */
    explicit MGridLayoutPolicyPrivate(MLayout *l);
    /** Destructor */
    virtual ~MGridLayoutPolicyPrivate();

    void refreshEngine();
    void refreshWidget();

    QGraphicsWidget *const engineWidget;
    QGraphicsGridLayout *const engine;
};

#endif // Header Guard
