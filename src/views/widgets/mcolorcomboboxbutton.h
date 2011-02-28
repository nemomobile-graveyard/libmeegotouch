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

#ifndef MCOLORCOMBOBOXBUTTON_H
#define MCOLORCOMBOBOXBUTTON_H

#include "mlistitem.h"

class MLabel;
class QColor;
class QString;
class MColorWidget;
class QGraphicsItem;
class QGraphicsLinearLayout;

//! \internal

class MColorComboBoxButton : public MListItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MColorComboBoxButton)

public:

    MColorComboBoxButton(QGraphicsItem *parent = 0);

    void setTitle(const QString &title);

    void setTitleStyleName(const QString &name);

    void setColor(const QColor &color);

    void setColorStyleName(const QString &name);

private:

    MLabel *titleLabel;

    MColorWidget *colorWidget;

    QGraphicsLinearLayout *layout;
};

//! \internal_end

#endif
