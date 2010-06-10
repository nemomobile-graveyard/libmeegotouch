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

#include "qtmaemo6dialogtitle.h"
#include "qtmaemo6style_p.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <mescapebuttonpanelstyle.h>
#include <mlabelstyle.h>
#include <MTheme>


QtMaemo6DialogTitle::QtMaemo6DialogTitle(QWidget *parent) : QWidget(parent)
{
    setObjectName(QString("Qt_Maemo6_DialogTitle"));

    QStyleOption option;
    option.initFrom(this);

    const MLabelStyle *titleBarStyle =
        static_cast<const MLabelStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MLabelStyle", "MDialogTitleLabel"));

    m_titleLabel = new QLabel(this);
    if ( titleBarStyle ) {
      m_titleLabel->setFont(titleBarStyle->font());

      QPalette pal(palette());
      pal.setColor(foregroundRole(), titleBarStyle->color());
      setPalette(pal);
    }

    m_closeButton = new QtMaemo6ClickLabel(this);
    m_closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    m_closeButton->setMargin(8);
    m_closeButton->setObjectName( QString( "Qt_Maemo6_Dialog_CloseButton" ) );

    //currently meegotouch uses the same close button icon for dialogs as used in navigation bar
    const MEscapeButtonPanelStyle *escapeButtonStyle =
        static_cast<const MEscapeButtonPanelStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MEscapeButtonPanelStyle", ""));
    if(escapeButtonStyle) {
        m_closeButton->setPixmap(*MTheme::pixmapCopy(escapeButtonStyle->closeButtonIconId()));
    }
    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(closeRequest()));

    m_titleBarLayout = new QHBoxLayout(this);
    m_titleBarLayout->setMargin(0);
    m_titleBarLayout->setSpacing(0);
    m_titleBarLayout->addWidget(m_titleLabel);
    m_titleBarLayout->addWidget(m_closeButton);

}

QtMaemo6DialogTitle::~QtMaemo6DialogTitle()
{

}

void QtMaemo6DialogTitle::resizeEvent(QResizeEvent *) {
    if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(style()))
        s->setPaletteBackground(this, "MDialogStyle", "MDialogTitleBar");
}

void QtMaemo6DialogTitle::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
    m_titleLabel->setAlignment(Qt::AlignHCenter);
}

QString QtMaemo6DialogTitle::title() const
{
    return m_titleLabel->text();
}

void QtMaemo6DialogTitle::setPixmap(const QPixmap &icon)
{
    m_closeButton->setPixmap(icon);
}
