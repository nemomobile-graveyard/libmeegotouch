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

#include "qtmaemo6comboboxpopup.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6style_p.h"

#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCompleter>
#include <QPushButton>

//krazy:excludeall=qclasses

QtMaemo6ComboBoxPopup::QtMaemo6ComboBoxPopup(QComboBox *comboBox, QWidget *parent)
    : QWidget(parent),
    m_comboBox(comboBox),
    m_listView(NULL),
    m_lineEdit(NULL)
{
    setObjectName("Qt_Maemo6_ComboBox");
    m_listView = new QListView();
    if(comboBox)
        m_listView->setModel(comboBox->model());

    if(m_comboBox->isEditable()) {
        //m_lineEdit = m_comboBox->lineEdit();
        m_lineEdit = new QLineEdit();
        if(m_comboBox->completer()) {
            m_lineEdit->setCompleter(m_comboBox->completer());
            m_lineEdit->setText(m_comboBox->lineEdit()->text());
            m_comboBox->completer()->setWidget(m_comboBox);
        }
        connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(closePopup()));
    }

    QVBoxLayout* vLayout = new QVBoxLayout(this);

    if(m_lineEdit) {
        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->setMargin(0);
        hLayout->setSpacing(8);
        QPushButton* button = new QPushButton(qtTrId("qtn_comm_command_done"));
        hLayout->addWidget(m_lineEdit);
        hLayout->addWidget(button);

        vLayout->addLayout(hLayout);

        connect(button, SIGNAL(clicked()), this, SLOT(closePopup()));
    }
    vLayout->addWidget(m_listView);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_listView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectItem(QModelIndex)));
}

QtMaemo6ComboBoxPopup::~QtMaemo6ComboBoxPopup()
{

}

void QtMaemo6ComboBoxPopup::showEvent(QShowEvent *e) {
    Q_UNUSED(e);
    if(m_lineEdit) {
        m_lineEdit->setFocus();
        m_lineEdit->selectAll();
    }
}

void QtMaemo6ComboBoxPopup::selectItem(const QModelIndex & index)
{
    m_comboBox->setCurrentIndex(index.row());
    close();
}

void QtMaemo6ComboBoxPopup::closePopup() {
    if(m_comboBox->completer()->completionCount() > 0)
        //If there is a valid completion use this
        m_comboBox->lineEdit()->setText( m_comboBox->completer()->currentCompletion() );
    else
        //if there's no completion
        m_comboBox->lineEdit()->setText(m_lineEdit->text());
    close();
}
