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

#ifndef MLISTINDEX_H
#define MLISTINDEX_H

#include "mwidgetcontroller.h"
#include "mlistindexmodel.h"

#include <QMap>

#include <MList>

class MListIndexPrivate;
class DuiList;

/*!
 * \class MListIndex
 * \brief MListIndex is a container for list indexes, for fast navigation through the
 * list content.
 *
 * MListIndex creates a stylable container on the right side of the window in which the list
 * is rendered. Its main duty is to provide a way for fast navigation through list contents.
 * The content of the list index is dynamic and is controlled by the list itself.
 * The list index widget creates a scene window container and places itself into it.
 * Thus the positioning and sizing of the list index are controlled by the scene window.
 * Its contents is controlled by the list.
 *
 */
class MListIndex : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MListIndex)

public:
    /*!
     * \brief Default constructor. Constructs the list index widget.
     * \param parent The list controller of the list index content.
     */
    MListIndex(MList *parent = NULL);

    /*!
     * \brief Default destructor. Cleanups and destroys the list index widget.
     */
    virtual ~MListIndex();

    /*!
     * \brief Sets the navigation indexes of the list to be displayed.
     * \param shortcuts The mapping between a string label and its index position in list.
     */
    void setShortcuts(const QMap<QModelIndex, QString> &shortcuts);

    /*!
     * \brief Sets the content controller of the list index.
     * \param list The list widget that the index widget provides fast navigation for.
     */
    void setList(MList *list);

    /*!
     * \brief Sets the automatic visibility to the list index.
     * \param displayMode The display mode of the list index bar.
     * \sa MList::DisplayMode
     */
    void setDisplayMode(MList::DisplayMode displayMode);

    MList::DisplayMode displayMode();

    void setOffset(const QPointF &offset);
    QPointF offset() const;

protected:
   /*! \reimp */
   void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
   /*! \reimp_end */

private:
    Q_PRIVATE_SLOT(d_func(), void _q_updateVisibility())

    Q_DECLARE_PRIVATE(MListIndex)
    MListIndexPrivate *d_ptr;

    Q_DISABLE_COPY(MListIndex)

#ifdef UNIT_TEST
    friend class Ut_MListIndexFloatingView;
#endif
};

#endif
