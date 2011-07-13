/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSLOTLAYOUT_H
#define MSLOTLAYOUT_H

#include <QGraphicsLayout>
#include <QGraphicsWidget>
#include <QWeakPointer>

//! \internal

/*!
  \class MSlotLayout

  A layout that contains only a single item, but has some differences
  on how it handles size policies compared to QGraphicsLinearLayout.

  Unlike a QGraphicsLinearLayout, it won't necessarily stretch its child item
  to fill all the available space. It will follow the childs' size policy.

  Another difference is that there are two ways of overriding the size policy
  of it child:

  1 - by assigning a proxy widget for size policies.
  2 - by telling the layout to explicitly ignore size policies, making it
      behave like a QGraphicsLinearLayout, stretching the child to fit all
      available space.
 */
class MSlotLayout : public QGraphicsLayout
{
public:
    MSlotLayout(QGraphicsLayoutItem *parent = 0);
    void setWidget(QGraphicsWidget *widget);
    void setSizePolicyProxyWidget(QGraphicsWidget *proxyWidget);
    void setIgnoreSizePolicies(bool enabled);
    virtual QGraphicsLayoutItem *itemAt(int i) const;
    virtual void removeAt(int index);
    virtual int count() const;
    virtual void setGeometry(const QRectF &rect);
protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
private:
    QGraphicsItem *parentItem();
    qreal calculateLength(QSizePolicy::Policy policy, qreal preferred, qreal available);
    QWeakPointer<QGraphicsWidget> widgetPointer;
    QWeakPointer<QGraphicsWidget> proxyWidgetPointer;
    bool ignoreSizePolicies;
};
//! \internal_end
#endif