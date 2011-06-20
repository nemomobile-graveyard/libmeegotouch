/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef LABELEDCHECKBOX_H
#define LABELEDCHECKBOX_H

#include <MButton>
#include <MLabel>
class QGraphicsLinearLayout;

class LabeledCheckbox : public QGraphicsWidget
{
    Q_OBJECT
public:
    LabeledCheckbox(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~LabeledCheckbox();

    MButton *button;
    MLabel *label;
    QGraphicsLinearLayout *roundedCornersCheckboxLayout;
};

#endif