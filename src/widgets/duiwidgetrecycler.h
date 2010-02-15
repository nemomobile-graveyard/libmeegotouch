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
  and deleted frequently, for instance in DuiList or DuiGrid.

  When widgets in DuiList/DuiGrid become invisible during panning,
  they are added to the recycler. New widgets that become visible are
  not constructed from scratch, instead the widgets that have been
  added to the recycler are reused.

  The item model class 'data' function must be implemented as in the
  following example. If the recycler has a widget of the needed class
  already available, the data function must use that widget.
  Otherwise the data function creates a new widget.

  Example code with DuiButtons that have the row number as their text.

  \code

  QVariant ExampleModel::data(const QModelIndex &index, int role) const
  {
      if (role == Qt::DisplayRole) {
          DuiButton* item;

          DuiWidgetRecycler* recycler = DuiWidgetRecycler::instance();
          DuiWidget* widget = recycler->take("DuiButton");

          item = dynamic_cast<DuiButton*>(widget);
          if (item == 0) {
              item = new DuiButton;
          }

          QString s;
          s.setNum(index.row());
          item->setText(s);

          QVariant v;
          v.setValue(item);
          return v;
      }

      return QVariant();
  }

  \endcode
*/

class DUI_EXPORT DuiWidgetRecycler
{
public:

    /*!
      \brief Returns the unique instance of DuiWidgetRecycler.
    */
    static DuiWidgetRecycler *instance();

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
