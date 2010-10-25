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

#ifndef MWIDGETRECYCLER_H
#define MWIDGETRECYCLER_H


#include "mexport.h"
#include <QString>

class MWidgetRecyclerPrivate;
class MWidget;

/*!
  \class MWidgetRecycler
  \brief MWidgetRecycler allows to reuse widgets when they are created
  and deleted frequently, for instance in MList.

  When widgets in MList become invisible during panning,
  they are added to the recycler. New widgets that become visible are
  not constructed from scratch, instead the widgets that have been
  added to the recycler are reused.

  MWidgetRecycler "replaces" <code>new</code> and <code>delete</code> keywords. E.g.
  when object needs to be delete it should be added to recycler:
  \code
  ....
  // Checking if recycler has any objects with classname "MWidget".
  MWidget * newItem = qobject_cast<MWidget*>(recycler->take(MWidget::metaObject()->className()));
  if(newItem == NULL) {
    newItem = new MWidget;
  }
  ....
  recycler->recycle(someItem); // item is not needed anymore, saving for future use
  ...

  \endcode

  To use non default identifier (or identifiers) for recycled widgets following syntax could be used:
  \code

  ....
  // Checking if recycler has any objects with identifier "myItem"
  MWidget * newItem = qobject_cast<MWidget*>(recycler->take("myItem"));
  if(newItem == NULL) {
    newItem = new MWidget;
    newItem->setProperty(MWidgetRecycler::RecycledObjectIdentifier, "myItem");
  }
  ....
  recycler->recycle(someItem); // item is not needed anymore, saving for future use
  ...

  \endcode

  MWidgetRecycler will delete widgets if there is not enough space. To change maximum number of
  widgets in recycler check setMaxItemsPerClass(int).
*/

class M_CORE_EXPORT MWidgetRecycler
{
public:
    static const char * RecycledObjectIdentifier;

    /*!
      \brief Returns the unique instance of MWidgetRecycler.
    */
    static MWidgetRecycler *instance();

    /*!
      \brief MWidgetRecycler constructor. To get system wide recycler please check
      instance(), however it's perfectly fine to create recycler for special cases.
      */
    MWidgetRecycler();

    /*!
      \brief Puts a new widget to the recycler.

      Widget ownership is transferred to recycler. If the widget recycler is full,
      widget will be deleted.

      Recycler will check if widget has a property MWidgetRecycler::RecycledObjectIdentifier it will use it's
      value as a identifier, otherwise recycler will use class name.
    */
    void recycle(MWidget *widget);

    /*!
      \brief Returns a widget from the recycler, if available.

      Ownership of returned widget is transferred to the user.
      Returns 0 if no widget of the specified class is available.

      \sa recycle(MWidget *)
    */
    MWidget *take(const QString &recycleId);

    /*!
      \brief Sets the maximum number of widgets the recycler holds at one time per class. Each class
      has the same limit.
    */
    void setMaxItemsPerClass(int count);

    /*!
      \brief Returns possible maximum amount of widgets in the recycler per class, e.g. each class has
      the same limit.
      */
    int maxItemsPerClass() const;

    /*!
      \brief Destructor
    */
    virtual ~MWidgetRecycler();

private:
    Q_DISABLE_COPY(MWidgetRecycler)

    MWidgetRecyclerPrivate *const d_ptr;
};

#endif
