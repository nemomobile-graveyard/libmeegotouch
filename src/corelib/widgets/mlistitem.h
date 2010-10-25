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

#ifndef MLISTITEM_H
#define MLISTITEM_H

#include <MWidgetController>

#include "mlistitemmodel.h"

class MLayout;
class MListItemPrivate;

/*!
    \class MListItem
    \brief MListItem is a simple widget for displaying content with basic functionality
    like background rendering with differend object modes.
    
    \ingroup widgets
    
    \sa MListItemView
*/
class M_CORE_EXPORT MListItem : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MListItem)

public:
    /*!
      \brief Constructs a MListItem with a \a parent.
      \param parent Parent object.
    */
    MListItem(QGraphicsItem *parent = 0);
    /*!
      \brief Destructor.
    */
    virtual ~MListItem();

public Q_SLOTS:
    /*!
      \brief Makes the list cell to send clicked() signal.
    */
    void click();

    /*!
      \brief Makes the list cell to send longTapped signal.
      \param pos The position of the tap.
    */
    void longTap(const QPointF &pos);
    
Q_SIGNALS:
    /*!
      \brief The signal is emitted when the list cell is clicked.
    */
    void clicked();

    /*!
      \brief The signal is emitted when the list cell has been tapped and holded.
    */
    void longTapped(const QPointF &pos);

protected:
    /*!
       Handler of notifications of new receivers connected to MListItem signals.
    */
    virtual void connectNotify(const char *signal);

    /*!
       Handler of notifications of receivers disconnecting from MListItem signals.
    */
    virtual void disconnectNotify(const char *signal);

    
private:   
    Q_DECLARE_PRIVATE(MListItem)
    Q_DISABLE_COPY(MListItem)
};

#endif // MLISTITEM_H
