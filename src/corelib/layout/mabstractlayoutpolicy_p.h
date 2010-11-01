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

#ifndef MABSTRACTLAYOUTPOLICY_P_H
#define MABSTRACTLAYOUTPOLICY_P_H

#include "mabstractlayoutpolicy.h"
#include <QString>
#include <QList>

class MAbstractLayoutPolicyStyle;
class MLayout;
/** Private policy class. */
class MAbstractLayoutPolicyPrivate
{
    Q_DECLARE_PUBLIC(MAbstractLayoutPolicy)

public:
    /** Constructor */
    explicit MAbstractLayoutPolicyPrivate(MLayout *);
    /** Destructor */
    virtual ~MAbstractLayoutPolicyPrivate();

    void aboutToBeRemovedFromLayout(int index);

    /** The layout we are associated with. */
    MLayout *const layout;

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
    const MAbstractLayoutPolicyStyle *style;

    /** Name for this policy for MStyle.  Similar to the MWidgetController::styleName() property */
    QString styleName;

    /** List of items in this policy, as an index of layout */
    QList<int> items;

    /** This is true if we are currently removing an item from the layout */
    bool removingFromLayout;

    /** True if the height depends on the width */
    bool hasHeightForWidth;

protected:
    // Shared d_ptr related code:
    MAbstractLayoutPolicy *q_ptr;
};

#endif // Header Guard
