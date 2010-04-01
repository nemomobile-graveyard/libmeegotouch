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

#ifndef DUISEPARATOR_H
#define DUISEPARATOR_H

#include <DuiWidgetController>
#include <duiexport.h>
#include "duiseparatormodel.h"

class DuiSeparatorPrivate;

/*!
 * \class DuiSeparator
 * \brief DuiSeparator is a generic separator widget.
 */
class DUI_EXPORT DuiSeparator : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSeparator)

    /*!
        \property DuiSeparator::orientation
        \brief See DuiSeparatorModel::orientation
    */
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
    /*!
        \brief Constructs the separator.
     */
    DuiSeparator(QGraphicsItem *parent = 0, Qt::Orientation orientation = Qt::Horizontal);


    /*!
        \brief Destroys the separator.
     */
    virtual ~DuiSeparator();

    /*!
        \brief Gets the orientation.
     */
    Qt::Orientation orientation();

public Q_SLOTS:
    /*!
        \brief Set the orientation.
     */
    void setOrientation(Qt::Orientation orientation);

private:
    Q_DECLARE_PRIVATE(DuiSeparator)
    Q_DISABLE_COPY(DuiSeparator)

};

#endif // DUISEPARATOR_H

