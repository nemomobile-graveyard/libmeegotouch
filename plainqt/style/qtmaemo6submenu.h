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

#ifndef QTMAEMO6SUBMENU_H
#define QTMAEMO6SUBMENU_H

#include "qtmaemo6window.h"

#include <QListWidget>

class QMenu;
class QListWidgetItem;

/*!
 * This class shows a submenu out of the menubar
 */
class QtMaemo6SubMenu : public QListWidget
{
    Q_OBJECT
public:
    explicit QtMaemo6SubMenu(QMenu *m, QWidget *parent);
    virtual ~QtMaemo6SubMenu();
protected Q_SLOTS:
    void listItemClicked(QListWidgetItem *);
protected:
    void fillListWidget(QMenu *m);
private:
    QMap<QListWidgetItem *, QAction *> m_actionItemList;
};

#endif
