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

#ifndef MCOLORLIST_H
#define MCOLORLIST_H

#include <MWidgetController>
#include <QColor>
#include <QObject>

#include "mcolorlistmodel.h"

class MColorListPrivate;

/**
 * \brief A widget for picking colors from a list of predefined colors.
 */
class M_CORE_EXPORT MColorList : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MColorList)
    Q_DECLARE_PRIVATE(MColorList)
    Q_DISABLE_COPY(MColorList)

public:
    /**
     * \brief Constructor.
     */
    MColorList(QGraphicsItem *parent = 0);

    /**
     * \brief Destructor.
     */
    virtual ~MColorList();

    /**
     * \brief Get selected color.
     *
     * \return Selected color.
     */
    QColor selectedColor();

    /**
     * \brief Set selected color.
     *
     * \param color Selected color.
     */
    void setSelectedColor(const QColor &color);

protected slots:

    //! \reimp

    virtual void updateData(const QList<const char*> &modifications);

    //! \reimp_end

signals:
    /**
     * \brief Emitted when the user has picked a color.
     */
    void colorPicked();

};

#endif
