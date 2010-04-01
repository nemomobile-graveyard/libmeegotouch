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

#ifndef DUIBUTTONGROUPLAYOUTPOLICY_P_H
#define DUIBUTTONGROUPLAYOUTPOLICY_P_H

#include <DuiLinearLayoutPolicy>
#include <DuiTheme>

class DuiButtonGroupLayoutPolicy : public DuiLinearLayoutPolicy
{
public:
    DuiButtonGroupLayoutPolicy(DuiLayout *layout, Qt::Orientation orientation);

    virtual ~DuiButtonGroupLayoutPolicy();

    virtual void insertItem(int index, QGraphicsLayoutItem *item);
    virtual void removeAt(int index);

protected:
    void updateButtonsViewTypes();
    void setButtonViewType(int index, const DuiTheme::ViewType &viewType);

private:
    Q_DISABLE_COPY(DuiButtonGroupLayoutPolicy)
};

#endif // DUIBUTTONGROUPLAYOUTPOLICY_P_H
