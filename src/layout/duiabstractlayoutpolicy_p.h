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

#ifndef DUIABSTRACTLAYOUTPOLICY_P_H
#define DUIABSTRACTLAYOUTPOLICY_P_H

#include "duiabstractlayoutpolicy.h"
#include <QString>
#include <QList>

class DuiAbstractLayoutPolicyStyle;
class DuiLayout;
/** Private policy class. */
class DuiAbstractLayoutPolicyPrivate
{
    Q_DECLARE_PUBLIC(DuiAbstractLayoutPolicy)

public:
    /** Constructor */
    explicit DuiAbstractLayoutPolicyPrivate(DuiLayout *);
    /** Destructor */
    virtual ~DuiAbstractLayoutPolicyPrivate();

    void aboutToBeRemovedFromLayout(const QGraphicsLayoutItem *item);

    /** The layout we are associated with. */
    DuiLayout *const layout;

    /** Left margin */
    qreal marginLeft;

    /** Top margin */
    qreal marginTop;

    /** Right margin */
    qreal marginRight;

    /** Bottom margin */
    qreal marginBottom;

    /** Horizontal spacing between items */
    qreal horizontalSpacing;

    /** Vertical spacing between items */
    qreal verticalSpacing;

    /** Whether the user has manually called setVerticalSpacing */
    bool userSetVerticalSpacing;

    /** Whether the user has manually called setHorizontalSpacing */
    bool userSetHorizontalSpacing;

    /** Whether the user has manually called setContentsMargins */
    bool userSetContentsMargins;

    /** Style for the abstract policy, but can be also used for inheriting policies */
    const DuiAbstractLayoutPolicyStyle *style;

    /** Name for this policy for DuiStyle.  Similar to the QObject::objectName() property */
    QString objectName;

    /** List of items in this policy, as an index of layout */
    QList<int> items;

    /** This is true if we are currently removing an item from the layout */
    bool removingFromLayout;

protected:
    // Shared d_ptr related code:
    DuiAbstractLayoutPolicy *q_ptr;
};

#endif // Header Guard
