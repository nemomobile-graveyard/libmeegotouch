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

#ifndef MCOLORCOMBOBOX_H
#define MCOLORCOMBOBOX_H

#include <MWidgetController>
#include <QList>
#include <QColor>
#include <QObject>

#include "mcolorcomboboxmodel.h"

class MColorComboBoxPrivate;

//! \internal
/**
 * \brief A combobox for picking colors from a list of predefined colors.
 * \note INTERNAL, only for rich text formatting dialog
 */
class M_CORE_EXPORT MColorComboBox : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MColorComboBox)
    Q_DECLARE_PRIVATE(MColorComboBox)
    Q_DISABLE_COPY(MColorComboBox)

public:

    MColorComboBox(QGraphicsItem *parent = 0);

    ~MColorComboBox();

    QString title();

    void setTitle(const QString &title);

    QColor pickedColor();

    void setPickedColor(const QColor &color);

signals:

    void colorPicked(const QColor &color);

protected slots:

    //! \reimp

    virtual void updateData(const QList<const char*> &modifications);

    //! \reimp_end

};
//! \internal_end

#endif
