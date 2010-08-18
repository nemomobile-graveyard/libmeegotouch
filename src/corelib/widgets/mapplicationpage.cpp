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

#include "mapplicationpage.h"
#include "mapplicationpage_p.h"
#include <mapplicationpagemodel.h>
#include <MDebug>
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include <MTheme>
#include <MApplication>
#include <MApplicationWindow>
#include <MPannableViewport>
#include <MSceneManager>
#include <MEscapeButtonPanel>
#include <MAction>
#include <MScene>
#include <MPositionIndicator>
#include <mdockwidget.h>
#include <mapplicationwindow_p.h>

#include "mondisplaychangeevent.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MApplicationPage)

MApplicationPagePrivate::MApplicationPagePrivate() :
    rememberPosition(false),
    topSpacer(NULL),
    bottomSpacer(NULL),
    mainWidget(NULL),
    mainLayout(NULL),
    pannableViewPort(NULL),
    centralWidget(NULL),
    contentCreated(false),
    backEnabled(false)
{}

void MApplicationPagePrivate::init()
{
    Q_Q(MApplicationPage);

    contentCreated = false;
    backEnabled = false;
    rememberPosition = true;

    QGraphicsLinearLayout *layout = createLayout();
    q->setLayout(layout);

    pannableViewPort = new MPannableViewport(q);
    pannableViewPort->setClipping(false);
    layout->addItem(pannableViewPort);

    mainWidget = new MWidget;

    pannableViewPort->setWidget(mainWidget);

    mainLayout = createLayout();
    mainWidget->setLayout(mainLayout);

    topSpacer = createSpacer(mainWidget);
    setWidgetHeight(topSpacer, 0);
    mainLayout->addItem(topSpacer);

    centralWidget = new MWidget(mainWidget);
    mainLayout->addItem(centralWidget);

    bottomSpacer = createSpacer(mainWidget);
    setWidgetHeight(bottomSpacer, 0);
    mainLayout->addItem(bottomSpacer);
}

QGraphicsLinearLayout *MApplicationPagePrivate::createLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0.0);
    return layout;
}

MWidget *MApplicationPagePrivate::createSpacer(QGraphicsItem *parent)
{
    MWidget *spacer = new MWidget(parent);
    return spacer;
}

void MApplicationPagePrivate::setWidgetHeight(MWidget *w, qreal height)
{
    w->setMinimumHeight(height);
    w->setMaximumHeight(height);
    w->setPreferredHeight(height);
}

void MApplicationPagePrivate::deleteCurrentCentralWidget()
{
    if (centralWidget) {
        mainLayout->removeItem(centralWidget);
        delete centralWidget;
        centralWidget = 0;
    }
}

void MApplicationPagePrivate::placeCentralWidget(QGraphicsWidget *widget)
{
    if (widget) {
        // insert the new central widget between top and bottom spacers
        if (mainLayout->count() > 0)
            mainLayout->insertItem(1,widget);
        else
            mainLayout->addItem(widget);
        centralWidget = widget;
    }
}

void MApplicationPagePrivate::updatePannableViewportPosition()
{
    if (!rememberPosition) {
        pannableViewPort->setPosition(QPointF(0, 0));
    }
}

void MApplicationPagePrivate::propagateOnDisplayChangeEvent(bool visible)
{
    Q_Q(MApplicationPage);

    if (q->scene() == 0 || q->sceneManager() == 0)
        return;

    if (centralWidget) {
        QRectF viewRect(QPointF(0, 0), q->sceneManager()->visibleSceneSize());
        if (visible) {
            MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::MustBeResolved, viewRect);
            q->scene()->sendEvent(centralWidget, &ev);
        } else {
            MOnDisplayChangeEvent ev(false, viewRect);
            q->scene()->sendEvent(centralWidget, &ev);
        }
    }
}

MApplicationPage::MApplicationPage(QGraphicsItem *parent)
    : MSceneWindow(new MApplicationPagePrivate, new MApplicationPageModel,
                     MSceneWindow::ApplicationPage, QString(), parent)
{
    Q_D(MApplicationPage);

    d->init();
}

MApplicationPage::~MApplicationPage()
{
}

void MApplicationPage::setCentralWidget(QGraphicsWidget *centralWidget)
{
    Q_D(MApplicationPage);

    d->deleteCurrentCentralWidget();
    d->placeCentralWidget(centralWidget);
    d->propagateOnDisplayChangeEvent(isOnDisplay());
}

QGraphicsWidget *MApplicationPage::centralWidget()
{
    Q_D(MApplicationPage);

    return d->centralWidget;
}

MApplicationWindow *MApplicationPage::applicationWindow()
{
    MApplicationWindow *applicationWindow = 0;

    if (scene() != 0) {
        QList<QGraphicsView *> viewsList = scene()->views();
        int i = 0;

        while (applicationWindow == 0 && i < viewsList.count()) {

            applicationWindow = qobject_cast<MApplicationWindow *>(viewsList[i]);
            ++i;
        }
    }

    return applicationWindow;
}

const QString MApplicationPage::title() const
{
    return model()->title();
}

void MApplicationPage::setTitle(const QString &title)
{
    model()->setTitle(title);
}

void MApplicationPage::createContent()
{
    Q_D(MApplicationPage);

    d->contentCreated = true;
}

bool MApplicationPage::isContentCreated() const
{
    Q_D(const MApplicationPage);

    return d->contentCreated;
}

void MApplicationPage::setRememberPosition(bool remember)
{
    Q_D(MApplicationPage);

    d->rememberPosition = remember;
}

bool MApplicationPage::rememberPosition() const
{
    Q_D(const MApplicationPage);

    return d->rememberPosition;
}

void MApplicationPage::setPannable(bool pannable)
{
    Q_D(MApplicationPage);

    d->pannableViewPort->setEnabled(pannable);
}

bool MApplicationPage::isPannable() const
{
    Q_D(const MApplicationPage);

    return d->pannableViewPort->isEnabled();
}

Qt::Orientations MApplicationPage::panningDirection() const
{
    Q_D(const MApplicationPage);

    return d->pannableViewPort->panDirection();
}

void MApplicationPage::setPanningDirection(Qt::Orientations directions)
{
    Q_D(MApplicationPage);

    d->pannableViewPort->setPanDirection(directions);
}

void MApplicationPage::actionEvent(QActionEvent *e)
{
    QAction *action = e->action();
    switch (e->type()) {
    case QEvent::ActionRemoved: {
        if (action)
            action->disconnect(this);
        //fall through is intentional.
    }
    case QEvent::ActionChanged:
    case QEvent::ActionAdded: {
        emit actionUpdated(e);
        break;
    }
    default: {
        break;
    }
    }
}

void MApplicationPage::enterDisplayEvent()
{
    Q_D(MApplicationPage);

    if (!d->contentCreated) {
        d->contentCreated = true;
        createContent();
    }

    d->updatePannableViewportPosition();
}

void MApplicationPagePrivate::setExposedContentRect(const QRectF &exposedContentRect)
{
    Q_Q(MApplicationPage);

    if (exposedContentRect != this->exposedContentRect) {
        this->exposedContentRect = exposedContentRect;
        emit q->exposedContentRectChanged();

        updateAutoMarginsForComponents();
    }
}

QRectF MApplicationPage::exposedContentRect() const
{
    Q_D(const MApplicationPage);

    return d->exposedContentRect;
}

void MApplicationPagePrivate::updateAutoMarginsForComponents()
{
    Q_Q(MApplicationPage);

    qreal topMargin = 0.0;
    qreal bottomMargin = 0.0;

    if (q->autoMarginsForComponentsEnabled()) {
        topMargin = exposedContentRect.y();

        qreal exposedContentRectBottomEdge = exposedContentRect.y() + exposedContentRect.height();
        bottomMargin = q->boundingRect().height() - exposedContentRectBottomEdge;
    }

    pannableViewPort->positionIndicator()->setPos(exposedContentRect.topLeft());
    setWidgetHeight(pannableViewPort->positionIndicator(), exposedContentRect.height());

    setWidgetHeight(topSpacer, topMargin);
    setWidgetHeight(bottomSpacer, bottomMargin);

    // FIXME: This activation is needed so input widget relocator can correctly calculate
    // positions after decorations hide. Correct place for this would be after all
    // decorations have been hidden. Only one activation is required.
    pannableViewPort->widget()->layout()->activate();
}

MApplicationPageModel::ComponentDisplayMode MApplicationPage::componentDisplayMode(
    Component component) const
{
    MApplicationPageModel::ComponentDisplayMode displayMode;

    switch (component) {
    case NavigationBar:
        displayMode = model()->navigationBarDisplayMode();
        break;

    case HomeButton:
        displayMode = model()->homeButtonDisplayMode();
        break;

    default:
        displayMode = model()->escapeButtonDisplayMode();
    };

    return displayMode;
}

void MApplicationPage::setComponentsDisplayMode(Components components,
        MApplicationPageModel::ComponentDisplayMode displayMode)
{
    if (components.testFlag(NavigationBar)) {
        model()->setNavigationBarDisplayMode(displayMode);
    }

    if (components.testFlag(HomeButton)) {
        model()->setHomeButtonDisplayMode(displayMode);
    }

    if (components.testFlag(EscapeButton)) {
        model()->setEscapeButtonDisplayMode(displayMode);
    }
}

bool MApplicationPage::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void MApplicationPage::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void MApplicationPage::setAutoMarginsForComponentsEnabled(bool enabled)
{
    model()->setAutoMarginsForComponentsEnabled(enabled);
}

bool MApplicationPage::autoMarginsForComponentsEnabled() const
{
    return model()->autoMarginsForComponentsEnabled();
}

MApplicationPageModel::PageEscapeMode MApplicationPage::escapeMode() const
{
    return model()->escapeMode();
}

void MApplicationPage::setEscapeMode(MApplicationPageModel::PageEscapeMode mode)
{
    model()->setEscapeMode(mode);
}

MPannableViewport * MApplicationPage::pannableViewport()
{
    Q_D(MApplicationPage);
    return d->pannableViewPort;
}

#include "moc_mapplicationpage.cpp"
