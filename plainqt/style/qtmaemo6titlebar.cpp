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
#include <mhomebuttonpanelstyle.h>
#include <mescapebuttonpanelstyle.h>
#include <mlabelstyle.h>
#include <MScalableImage>
#include <MTheme>
#include <mnavigationbarstyle.h>
#include <mlabelstyle.h>


//FIXME: this is not defined by any MStyle right now and also hardcoded in
//       libmeegotouch. If this is finally defined anywhere else, remove this.
const int QtMaemo6TitleBar::titleBarHeight = 70;

QtMaemo6TitleBar::QtMaemo6TitleBar(QWidget *parent) : QWidget(parent)
{
    setObjectName(QString("Qt_Maemo6_TitleBar"));

    QStyleOption option;
    option.initFrom(this);

    m_minimizeButton = new QtMaemo6ClickLabel(this);
    m_minimizeButton->setObjectName( QString( "Qt_Maemo6_TitleBar_Home" ) );
    const MHomeButtonPanelStyle *homeButtonStyle =
        static_cast<const MHomeButtonPanelStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MHomeButtonPanelStyle", ""));
    if(homeButtonStyle) {
        m_minimizeButton->setPixmap(*MTheme::pixmapCopy(homeButtonStyle->homeButtonIconId()));
    }
    connect(m_minimizeButton, SIGNAL(clicked()), this, SIGNAL(minimizeButtonClicked()));

    m_titleLabel = new QtMaemo6ClickLabel(this);
    connect(m_titleLabel, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));

    m_titleLabelMenuButton = new QtMaemo6ClickLabel(this);
    m_titleLabelMenuButton->setObjectName("Qt_Maemo6_TitleBar_Menu");

    const MApplicationMenuButtonStyle *iconStyle =
        static_cast<const MApplicationMenuButtonStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MApplicationMenuButtonStyle", "NavigationBarMenuButton"));
    if (iconStyle) {
        if(!iconStyle->arrowIcon().isEmpty())
            m_titleLabelMenuButton->setPixmap(*MTheme::pixmapCopy(iconStyle->arrowIcon(), iconStyle->arrowIconSize()));
    }
    connect(m_titleLabelMenuButton, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));

    const MLabelStyle *menuButtonLabelStyle =
        static_cast<const MLabelStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MLabelStyle", "NavigationBarMenuButtonLabel"));
    if(menuButtonLabelStyle) {
        qCritical() << menuButtonLabelStyle->color();
        QPalette pal = m_titleLabel->palette();
        pal.setBrush(m_titleLabel->foregroundRole(), menuButtonLabelStyle->color());
        m_titleLabel->setPalette(pal);
    }

    m_closeButton = new QtMaemo6ClickLabel(this);
    m_closeButton->setObjectName( QString( "Qt_Maemo6_TitleBar_Close" ) );
    const MEscapeButtonPanelStyle *escapeButtonStyle =
        static_cast<const MEscapeButtonPanelStyle *>(QtMaemo6StylePrivate::mStyle(option.state,
                "MEscapeButtonPanelStyle", ""));
    if(escapeButtonStyle) {
        m_closeButton->setPixmap(*MTheme::pixmapCopy(escapeButtonStyle->closeButtonIconId()));
    }
    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(closeButtonClicked()));

    m_titleBarLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_titleBarLayout->setMargin(0);
    m_titleBarLayout->setSpacing(0);
    m_titleBarLayout->addWidget(m_minimizeButton);
    m_titleBarLayout->addWidget(m_titleLabel);
    m_titleBarLayout->addWidget(m_titleLabelMenuButton);
    m_titleBarLayout->addStretch();

    //m_titleBarLayout->addLayout(m_buttonsLayout);
    m_titleBarLayout->addWidget(m_closeButton);


    // apply properties of the navigation bar style
    const MNavigationBarStyle *style =
        static_cast<const MNavigationBarStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                "MNavigationBarStyle"));
    Q_UNUSED( style );
    const MLabelStyle *menuStyle =
        static_cast<const MLabelStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_None,
                "MLabelStyle", "NavigationBarMenuButtonLabel"));
    Q_UNUSED( menuStyle );
    //TODO: use style and menuStyle once the properties inside work actually.
    // This would also remove the magic numbers.
    setMargin( 10 );
    setItemSpacing( 20 );

    setOrientation(M::Angle0);

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
    Q_UNUSED(button);
    /*
    m_buttonsLayout->addWidget(button);
    m_buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    qDebug("Added ToolButton %s to TitleBar", button->text().toLocal8Bit().constData());
    */
}

void QtMaemo6TitleBar::setOrientation(M::OrientationAngle angle) {
    QBoxLayout::Direction direction;
    switch(angle) {
        default:
        case M::Angle0:
            direction = QBoxLayout::LeftToRight;
            setFixedHeight(titleBarHeight);
            setMaximumWidth(1000);
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            break;
        case M::Angle90:
            direction = QBoxLayout::TopToBottom;
            setFixedWidth(titleBarHeight);
            setMaximumHeight(1000);
            setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            break;
        case M::Angle180:
            direction = QBoxLayout::RightToLeft;
            setFixedHeight(titleBarHeight);
            setMaximumWidth(1000);
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            break;
        case M::Angle270:
            direction = QBoxLayout::BottomToTop;
            setFixedWidth(titleBarHeight);
            setMaximumHeight(1000);
            setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
            break;
    }
    update();

    m_titleBarLayout->setDirection(direction);

    m_titleBarLayout->setDirection(direction);
    m_minimizeButton->setOrientation(angle);
    m_titleLabel->setOrientation(angle);
    m_titleLabelMenuButton->setOrientation(angle);
    m_closeButton->setOrientation(angle);
    m_orientationAngle = angle;

    layout()->activate();
}
