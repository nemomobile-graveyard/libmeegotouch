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

#include "qtmaemo6titlebar.h"
#include "qtmaemo6clicklabel.h"
#include "qtmaemo6style_p.h"

#include "qtmaemo6clicklabel.h"
#include <QCoreApplication>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

#include <mapplicationmenubuttonstyle.h>
#include <MScalableImage>
#include <MTheme>

QtMaemo6TitleBar::QtMaemo6TitleBar(QWidget *parent) : QWidget(parent)
{
    setObjectName(QString("Qt_Maemo6_TitleBar"));

    QtMaemo6ClickLabel * minimizeButton = new QtMaemo6ClickLabel(this);
    minimizeButton->setObjectName( QString( "Qt_Maemo6_TitleBar_Home" ) );
/*
    minimize_button->setText("MinimizeButton");
    minimize_button->setIcon(QPixmap(":/Icon-home.png"));
    minimize_button->setAutoRaise(true);
    minimize_button->setIconSize(QSize(48, 48));
    */
    connect(minimizeButton, SIGNAL(clicked()), this, SIGNAL(minimizeButtonClicked()));

    m_titleLabel = new QtMaemo6ClickLabel(this);
    connect(m_titleLabel, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));

    m_titleLabelMenuButton = new QtMaemo6ClickLabel(this);

    QStyleOption option;
    option.initFrom(this);

    const MApplicationMenuButtonStyle *iconStyle =
        static_cast<const MApplicationMenuButtonStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MApplicationMenuButtonStyle", "NavigationBarMenuButton"));
    if (iconStyle) {
        m_titleLabelMenuButton->setPixmap(*MTheme::pixmapCopy(iconStyle->arrowIcon(), iconStyle->arrowIconSize()));
    }
    connect(m_titleLabelMenuButton, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));


    QSpacerItem *spacer = new QSpacerItem(0, 0);
    const MWidgetStyle *spacerStyle =
        static_cast<const MWidgetStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                                            "MWidgetStyle", "NavigationBarMenuButtonArrowImage"));
    if (spacerStyle) {
        spacer->changeSize(spacerStyle->marginLeft(), 0);
    }

    QtMaemo6ClickLabel * closeButton = new QtMaemo6ClickLabel(this);
    closeButton->setObjectName( QString( "Qt_Maemo6_TitleBar_Close" ) );
 /*
    close_button->setText("CloseButton");
    close_button->setIcon(QPixmap(":/Icon-close.png"));
    //FIXME: remove magic numbers!
    close_button->setIconSize(QSize(48, 48));
    close_button->setAutoRaise(true);
    */

    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeButtonClicked()));

    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->setMargin(0);
    m_buttonsLayout->setSpacing(0);
    m_buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_titleBarLayout = new QHBoxLayout(this);
    m_titleBarLayout->setMargin(0);
    m_titleBarLayout->setSpacing(0);
    m_titleBarLayout->addWidget(minimizeButton);
    m_titleBarLayout->addWidget(m_titleLabel);
    m_titleBarLayout->addItem(spacer);
    m_titleBarLayout->addWidget(m_titleLabelMenuButton);
    m_titleBarLayout->addLayout(m_buttonsLayout);
    m_titleBarLayout->addWidget(closeButton);
}

QtMaemo6TitleBar::~QtMaemo6TitleBar()
{

}

void QtMaemo6TitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QStyleOption option;
    option.initFrom(this);

    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void QtMaemo6TitleBar::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void QtMaemo6TitleBar::setTitleColor(const QColor &color)
{
    QPalette pal;
    pal.setColor( QPalette::Foreground, color );
    m_titleLabel->setPalette( pal );
}

void QtMaemo6TitleBar::setMargin(int margin)
{
    m_titleBarLayout->setMargin( margin );
}

void QtMaemo6TitleBar::setItemSpacing(int spacing)
{
    m_titleBarLayout->setSpacing( spacing );
}

QString QtMaemo6TitleBar::title() const
{
    return m_titleLabel->text();
}

QColor QtMaemo6TitleBar::titleColor() const
{
    return m_titleLabel->palette().color( QPalette::Foreground );
}

int QtMaemo6TitleBar::margin() const
{
    return m_titleBarLayout->margin();
}

int QtMaemo6TitleBar::itemSpacing() const
{
    return m_titleBarLayout->spacing();
}

void QtMaemo6TitleBar::setMenuButtonVisible(bool visible)
{
    m_titleLabelMenuButton->setVisible(visible);
}

void QtMaemo6TitleBar::addAction(QAction *action)
{
    QToolButton *tbtn = new QToolButton(this);
    tbtn->setDefaultAction(action);
    tbtn->setIconSize(QSize(48, 48));
    tbtn->setAutoRaise(true);

    addButton(tbtn);
}

void QtMaemo6TitleBar::addButton(QToolButton *button)
{
    m_buttonsLayout->addWidget(button);
    m_buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    qDebug("Added ToolButton %s to TitleBar", button->text().toLocal8Bit().constData());
}
