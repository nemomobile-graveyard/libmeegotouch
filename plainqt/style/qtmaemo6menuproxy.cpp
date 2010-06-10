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

#include "qtmaemo6menuproxy.h"
#include "qtmaemo6menu.h"
#include "qtmaemo6style_p.h"

#include <QGridLayout>
#include <QWidget>
#include <QEvent>
#include <QCloseEvent>
#include <QStyleOption>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <mapplicationmenustyle.h>
#include <mwidgetfadeanimationstyle.h>
#include <QDebug>

/* unforunately this is required to force widgets drawing it's background, even
   if there are widget attributes are set, that suppress the drawing!
*/
class DrawBackgroundEventFilter : public QObject {
protected:
    bool eventFilter(QObject* obj, QEvent* e) {
        if(QPaintEvent* pe = dynamic_cast<QPaintEvent*>(e)) {
            if(QWidget* w = qobject_cast<QWidget*>(obj)) {
                qCritical() << "EventFilter:" << w->objectName() << w->geometry() << pe->rect();

                Q_UNUSED(pe);
                QPainter p(w);
                p.setPen(Qt::NoPen);
                p.setBrush(w->palette().window());
                p.drawRect(pe->rect());
            }
        }
        return QObject::eventFilter(obj, e);
    };
};

QtMaemo6MenuProxy::QtMaemo6MenuProxy(QMenuBar *mb, QWidget *parent)
    : QtMaemo6Window(NULL, parent),
      m_menuBar(mb)
{
    setObjectName("Qt_Maemo6_MenuProxy");

    //must be set, to activate the compositing
    setAttribute(Qt::WA_TranslucentBackground);
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::transparent);
    setPalette(palette);

    m_appArea = new QWidget();
    m_appArea->setObjectName("appArea");
    m_appArea->installEventFilter(new DrawBackgroundEventFilter());

    m_appArea->setAttribute(Qt::WA_TranslucentBackground);
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 192)));
    m_appArea->setPalette(palette);

    QGridLayout* gridLayout = new QGridLayout(m_appArea);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    m_menu = new QtMaemo6Menu(mb, NULL);
    m_menu->setAttribute(Qt::WA_TranslucentBackground, false);
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 0)));
    m_menu->setPalette(palette);
    setCentralWidget(m_menu);

    QStyleOption option;
    option.initFrom(mb);

    const MApplicationMenuStyle *style =
        static_cast<const MApplicationMenuStyle *>(
            QtMaemo6StylePrivate::mStyle(option.state, "MApplicationMenuStyle"));

    QSpacerItem *topSpacer = new QSpacerItem(1, style->paddingTop());
    QSpacerItem *rightSpacer = new QSpacerItem(style->paddingRight(), 1);
    QSpacerItem *bottomSpacer = new QSpacerItem(1, style->paddingBottom());
    QSpacerItem *leftSpacer = new QSpacerItem(style->paddingLeft(), 1);

    gridLayout->addItem(leftSpacer,        0, 0, 1, 1);
    gridLayout->addWidget(centralWidget(), 0, 1, 1, 1);
    gridLayout->addItem(rightSpacer,       0, 2, 1, 1);
    gridLayout->addItem(bottomSpacer,      1, 0, 1, 3);

    m_windowLayout->addItem(topSpacer, 0, 0, 1, 1);
    m_windowLayout->addWidget(m_appArea, 1, 0, 1, 1);
}

QtMaemo6MenuProxy::~QtMaemo6MenuProxy()
{
    //delete manually, because it is removed from the scroll area bevore deleting
    delete m_menu;
}

void QtMaemo6MenuProxy::resizeEvent(QResizeEvent *) {
    if(widget()) {
        if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(widget()->style())) {
            QStyleOption option;
            option.initFrom(widget());

            const MApplicationMenuStyle *style =
                    static_cast<const MApplicationMenuStyle *>(
                        QtMaemo6StylePrivate::mStyle(option.state, "MApplicationMenuStyle"));
            // draw widget background
            s->setPaletteBackground(widget(), style->canvasImage());
        }
    }
}

void QtMaemo6MenuProxy::mousePressEvent(QMouseEvent *event)
{
    hideWindow(); //this also closes the window, after hide animation is done.
    Q_UNUSED(event);
}

void QtMaemo6MenuProxy::showEvent(QShowEvent *event) {
    Q_UNUSED(event);

    const MWidgetFadeAnimationStyle *fadeInStyle =
        static_cast<const MWidgetFadeAnimationStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                "MWidgetFadeAnimationStyle", "In"));

    layout()->activate();
    QRect finalGeometry = QRect(m_menu->geometry().topLeft(), m_menu->sizeHint());
    qCritical() << m_menu->geometry();
    QRect startGeometry = finalGeometry;
    startGeometry.moveTo(finalGeometry.x(), finalGeometry.y() - finalGeometry.height());
    m_menu->setGeometry(startGeometry);

    QParallelAnimationGroup* animationGroup = new QParallelAnimationGroup();
    QPropertyAnimation *widgetFadeIn = new QPropertyAnimation(animationGroup);
    widgetFadeIn->setTargetObject(m_menu);
    widgetFadeIn->setPropertyName("geometry");
    widgetFadeIn->setDuration(fadeInStyle->duration());
    widgetFadeIn->setEasingCurve(fadeInStyle->easingCurve());
    widgetFadeIn->setStartValue(startGeometry);
    widgetFadeIn->setEndValue(finalGeometry);

    QPalette finalPalette = m_appArea->palette();
    QPalette startPalette = finalPalette;
    startPalette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 0)));

    QPropertyAnimation *backgroundFadeIn = new QPropertyAnimation(animationGroup);
    backgroundFadeIn->setTargetObject(m_appArea);
    backgroundFadeIn->setPropertyName("palette");
    backgroundFadeIn->setDuration(fadeInStyle->duration());
    backgroundFadeIn->setEasingCurve(fadeInStyle->easingCurve());
    backgroundFadeIn->setStartValue(startPalette);
    backgroundFadeIn->setEndValue(finalPalette);

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

    QtMaemo6Window::showEvent(event);
}

void QtMaemo6MenuProxy::hideWindow() {
    const MWidgetFadeAnimationStyle *fadeOutStyle =
        static_cast<const MWidgetFadeAnimationStyle *>(QtMaemo6StylePrivate::mStyle(QStyle::State_Active,
                "MWidgetFadeAnimationStyle", "Out"));

    QRect startGeometry = m_menu->geometry();
    QRect finalGeometry = startGeometry;
    finalGeometry.moveTo(finalGeometry.x(), finalGeometry.y() - finalGeometry.height());

    QParallelAnimationGroup* animationGroup = new QParallelAnimationGroup();
    QPropertyAnimation *widgetFadeOut = new QPropertyAnimation(animationGroup);
    widgetFadeOut->setTargetObject(m_menu);
    widgetFadeOut->setPropertyName("geometry");
    widgetFadeOut->setDuration(fadeOutStyle->duration());
    widgetFadeOut->setEasingCurve(fadeOutStyle->easingCurve());
    widgetFadeOut->setStartValue(startGeometry);
    widgetFadeOut->setEndValue(finalGeometry);

    QPalette startPalette = m_appArea->palette();
    QPalette finalPalette = startPalette;
    startPalette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 0)));

    QPropertyAnimation *backgroundFadeOut = new QPropertyAnimation(animationGroup);
    backgroundFadeOut->setTargetObject(m_appArea);
    backgroundFadeOut->setPropertyName("palette");
    backgroundFadeOut->setDuration(fadeOutStyle->duration());
    backgroundFadeOut->setEasingCurve(fadeOutStyle->easingCurve());
    backgroundFadeOut->setStartValue(startPalette);
    backgroundFadeOut->setEndValue(finalPalette);

    connect(animationGroup, SIGNAL(finished()), this, SLOT(close()));
    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}
