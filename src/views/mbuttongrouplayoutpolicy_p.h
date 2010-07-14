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

#ifndef MBUTTONGROUPLAYOUTPOLICY_P_H
#define MBUTTONGROUPLAYOUTPOLICY_P_H

#include <MLinearLayoutPolicy>
#include <MTheme>

class MButtonGroupLayoutPolicy : public MLinearLayoutPolicy
{
public:
    MButtonGroupLayoutPolicy(MLayout *layout, Qt::Orientation orientation);

    virtual ~MButtonGroupLayoutPolicy();

    virtual void insertItem(int index, QGraphicsLayoutItem *item);
    virtual void removeAt(int index);
    void setButtonWidth(qreal width);

protected:
    void updateButtonsViewTypes();
    void setButtonViewType(int index, const MTheme::ViewType &viewType);

private:
    qreal buttonWidth;

    Q_DISABLE_COPY(MButtonGroupLayoutPolicy)
};

#endif // MBUTTONGROUPLAYOUTPOLICY_P_H
