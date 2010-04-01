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

#ifndef DUIWIDGETRECYCLER_H
#define DUIWIDGETRECYCLER_H


#include "duiexport.h"
#include <QString>

class DuiWidgetRecyclerPrivate;
class DuiWidget;

/*!
  \class DuiWidgetRecycler
  \brief DuiWidgetRecycler allows to reuse widgets when they are created
  and deleted frequently, for instance in DuiList.

  When widgets in DuiList become invisible during panning,
  they are added to the recycler. New widgets that become visible are
  not constructed from scratch, instead the widgets that have been
  added to the recycler are reused.

  DuiWidgetRecycler "replaces" <code>new</code> and <code>delete</code> keywords. E.g.
  when object needs to be delete it should be added to recycler:
  \code
  ....
  // Checking if recycler has any objects with classname "DuiWidget". It doesn't
  // have to be class name, but it has to identify DuiWidget uniqely.
  DuiWidget * newItem = qobject_cast<DuiWidget*>(recycler->take(DuiWidget::metaObject()->className()));
  if(newItem == NULL)
    newItem = new DuiWidget;
  ....
  recycler->recycle(someItem); // item is not needed anymore, saving for future use
  ...

  \endcode

  DuiWidgetRecycler will delete widgets if there is not enough space. To change maximum number of
  widgets in recycler check setMaxItemsPerClass(int).
*/

class DUI_EXPORT DuiWidgetRecycler
{
public:

    /*!
      \brief Returns the unique instance of DuiWidgetRecycler.
    */
    static DuiWidgetRecycler *instance();

    /*!
      \brief DuiWidgetRecycler constructor. To get system wide recycler please check
      instance(), however it's perfectly fine to create recycler for special
      */
    DuiWidgetRecycler();

    /*!
      \brief Puts a new widget to the recycler.

      Widget ownership is transferred to recycler. If the widget recycler is full,
      widget will be deleted.
    */
    void recycle(DuiWidget *widget);

    /*!
      \brief Returns a widget from the recycler, if available.

      Ownership of returned widget is transferred to the user.
      Returns 0 if no widget of the specified class is available.
    */
    DuiWidget *take(const QString &className);

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
    virtual ~DuiWidgetRecycler();

private:
    Q_DISABLE_COPY(DuiWidgetRecycler)

    DuiWidgetRecyclerPrivate *const d_ptr;
};

#endif
