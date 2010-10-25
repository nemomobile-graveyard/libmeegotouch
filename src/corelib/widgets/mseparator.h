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

#ifndef MSEPARATOR_H
#define MSEPARATOR_H

#include <MWidgetController>
#include <mexport.h>
#include "mseparatormodel.h"

class MSeparatorPrivate;

/*!
 * \class MSeparator
 * \brief MSeparator is a generic separator widget.
 */
class M_CORE_EXPORT MSeparator : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MSeparator)

    /*!
        \property MSeparator::orientation
        \brief See MSeparatorModel::orientation
    */
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
    /*!
        \brief Constructs the separator.
     */
    explicit MSeparator(QGraphicsItem *parent = 0, Qt::Orientation orientation = Qt::Horizontal);


    /*!
        \brief Destroys the separator.
     */
    virtual ~MSeparator();

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
    Q_DECLARE_PRIVATE(MSeparator)
    Q_DISABLE_COPY(MSeparator)

};

#endif // MSEPARATOR_H

