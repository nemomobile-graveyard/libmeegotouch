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

#include "qtmaemo6titlebar.h"
#include "qtmaemo6clicklabel.h"
#include "qtmaemo6style_p.h"

#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

#include <duiapplicationmenubuttonstyle.h>
#include <DuiScalableImage>
#include <DuiTheme>

QtMaemo6TitleBar::QtMaemo6TitleBar(QWidget *parent) : QWidget(parent)
{
    setObjectName(QString("Qt_Maemo6_TitleBar"));

    QToolButton *minimize_button(new QToolButton(this));
    minimize_button->setText("MinimizeButton");
    minimize_button->setIcon(QPixmap(":/Icon-home.png"));
    minimize_button->setAutoRaise(true);
    minimize_button->setIconSize(QSize(48, 48));

    connect(minimize_button, SIGNAL(clicked()), this, SIGNAL(minimizeButtonClicked()));

    m_titleLabel = new QtMaemo6ClickLabel(this);
    connect(m_titleLabel, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));

    m_titleLabelMenuButton = new QtMaemo6ClickLabel(this);

    QStyleOption option;
    option.initFrom(this);

    const DuiApplicationMenuButtonStyle *iconStyle =
        static_cast<const DuiApplicationMenuButtonStyle *>(QtMaemo6StylePrivate::duiStyle(option.state,
                "DuiApplicationMenuButtonStyle", "NavigationBarMenuButton"));
    if (iconStyle) {
        m_titleLabelMenuButton->setPixmap(*DuiTheme::pixmap(iconStyle->arrowIcon(), iconStyle->arrowIconSize()));
    }
    connect(m_titleLabelMenuButton, SIGNAL(clicked()), this, SIGNAL(menuLabelClicked()));


    QSpacerItem *spacer = new QSpacerItem(0, 0);
    const DuiWidgetStyle *spacerStyle =
        static_cast<const DuiWidgetStyle *>(QtMaemo6StylePrivate::duiStyle(option.state,
                                            "DuiWidgetStyle", "NavigationBarMenuButtonArrowImage"));
    if (spacerStyle) {
        spacer->changeSize(spacerStyle->marginLeft(), 0);
    }

    QToolButton *close_button(new QToolButton(this));
    close_button->setText("CloseButton");
    close_button->setIcon(QPixmap(":/Icon-close.png"));
    //FIXME: remove magic numbers!
    close_button->setIconSize(QSize(48, 48));
    close_button->setAutoRaise(true);

    connect(close_button, SIGNAL(clicked()), this, SIGNAL(closeButtonClicked()));

    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->setMargin(0);
    m_buttonsLayout->setSpacing(0);
    m_buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_titleBarLayout = new QHBoxLayout(this);
    m_titleBarLayout->setMargin(0);
    m_titleBarLayout->setSpacing(0);
    m_titleBarLayout->addWidget(minimize_button);
    m_titleBarLayout->addWidget(m_titleLabel);
    m_titleBarLayout->addItem(spacer);
    m_titleBarLayout->addWidget(m_titleLabelMenuButton);
    m_titleBarLayout->addLayout(m_buttonsLayout);
    m_titleBarLayout->addWidget(close_button);

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

QString QtMaemo6TitleBar::title() const
{
    return m_titleLabel->text();
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
