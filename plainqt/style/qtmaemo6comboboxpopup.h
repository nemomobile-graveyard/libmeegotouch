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

#ifndef QTMAEMO6COMBOBOXPOPUP_H
#define QTMAEMO6COMBOBOXPOPUP_H

#include "qtmaemo6window.h"

//krazy:excludeall=qclasses

#include <QWidget>

class QModelIndex;
class QListView;
class QComboBox;
class QLineEdit;

/*!
 * This class shows replaces combo box popups
 */
class QtMaemo6ComboBoxPopup : public QWidget
{
    Q_OBJECT
public:
    explicit QtMaemo6ComboBoxPopup(QComboBox *comboBox, QWidget *parent);
    virtual ~QtMaemo6ComboBoxPopup();
protected Q_SLOTS:
    void selectItem(const QModelIndex &);
    void closePopup();
protected:
    virtual void showEvent(QShowEvent *e);
private:
    QComboBox* m_comboBox;
    QListView* m_listView;
    QLineEdit* m_lineEdit;
};

#endif
