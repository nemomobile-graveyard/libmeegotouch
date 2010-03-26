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

#include "duiapplicationpage.h"
#include "duiapplicationpage_p.h"
#include <duiapplicationpagemodel.h>
#include <DuiDebug>
#include <DuiPannableViewport>
#include <QGraphicsLinearLayout>
#include <DuiTheme>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiSceneManager>
#include <DuiEscapeButtonPanel>
#include <DuiAction>
#include <DuiScene>
#include <duidockwidget.h>
#include <duiapplicationwindow_p.h>

#include "duiondisplaychangeevent.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiApplicationPage)

DuiApplicationPagePrivate::DuiApplicationPagePrivate() :
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

void DuiApplicationPagePrivate::init()
{
    Q_Q(DuiApplicationPage);

    contentCreated = false;
    backEnabled = false;
    rememberPosition = true;

    QGraphicsLinearLayout *layout = createLayout();
    q->setLayout(layout);

    pannableViewPort = new DuiPannableViewport(q);
    pannableViewPort->setClipping(false);
    layout->addItem(pannableViewPort);

    mainWidget = new DuiWidget;

    pannableViewPort->setWidget(mainWidget);

    mainLayout = createLayout();
    mainWidget->setLayout(mainLayout);

    topSpacer = createSpacer(mainWidget);
    setSpacerHeight(topSpacer, 0);
    mainLayout->addItem(topSpacer);

    centralWidget = new DuiWidget(mainWidget);
    mainLayout->addItem(centralWidget);

    bottomSpacer = createSpacer(mainWidget);
    setSpacerHeight(bottomSpacer, 0);
    mainLayout->addItem(bottomSpacer);
}

QGraphicsLinearLayout *DuiApplicationPagePrivate::createLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0.0);
    return layout;
}

DuiWidget *DuiApplicationPagePrivate::createSpacer(QGraphicsItem *parent)
{
    DuiWidget *spacer = new DuiWidget(parent);
    return spacer;
}

void DuiApplicationPagePrivate::setSpacerHeight(DuiWidget *spacer, qreal height)
{
    spacer->setMinimumHeight(height);
    spacer->setMaximumHeight(height);
    spacer->setPreferredHeight(height);
}

void DuiApplicationPagePrivate::deleteCurrentCentralWidget()
{
    if (centralWidget) {
        mainLayout->removeItem(centralWidget);
        delete centralWidget;
        centralWidget = 0;
    }
}

void DuiApplicationPagePrivate::placeCentralWidget(QGraphicsWidget *widget)
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

void DuiApplicationPagePrivate::updatePannableViewportPosition()
{
    if (!rememberPosition) {
        pannableViewPort->setPosition(QPointF(0, 0));
    }
}

void DuiApplicationPagePrivate::propagateOnDisplayChangeEvent(bool visible)
{
    Q_Q(DuiApplicationPage);

    if (q->scene() == 0 || q->sceneManager() == 0)
        return;

    if (centralWidget) {
        QRectF viewRect(QPointF(0, 0), q->sceneManager()->visibleSceneSize());
        if (visible) {
            DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::MustBeResolved, viewRect);
            q->scene()->sendEvent(centralWidget, &ev);
        } else {
            DuiOnDisplayChangeEvent ev(false, viewRect);
            q->scene()->sendEvent(centralWidget, &ev);
        }
    }
}

DuiApplicationPage::DuiApplicationPage(QGraphicsItem *parent)
    : DuiSceneWindow(new DuiApplicationPagePrivate, new DuiApplicationPageModel,
                     DuiSceneWindow::ApplicationPage, QString(), parent)
{
    Q_D(DuiApplicationPage);

    d->init();
}

DuiApplicationPage::~DuiApplicationPage()
{
}

void DuiApplicationPage::setCentralWidget(QGraphicsWidget *centralWidget)
{
    Q_D(DuiApplicationPage);

    d->deleteCurrentCentralWidget();
    d->placeCentralWidget(centralWidget);
    d->propagateOnDisplayChangeEvent(isOnDisplay());
}

QGraphicsWidget *DuiApplicationPage::centralWidget()
{
    Q_D(DuiApplicationPage);

    return d->centralWidget;
}

DuiApplicationWindow *DuiApplicationPage::applicationWindow()
{
    DuiApplicationWindow *applicationWindow = 0;

    if (scene() != 0) {
        QList<QGraphicsView *> viewsList = scene()->views();
        int i = 0;

        while (applicationWindow == 0 && i < viewsList.count()) {

            applicationWindow = qobject_cast<DuiApplicationWindow *>(viewsList[i]);
            ++i;
        }
    }

    return applicationWindow;
}

// TODO: Remove that now useless method override after API freeze period
void DuiApplicationPage::appear(DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appear(policy);
}

// TODO: Remove that now useless method after API freeze period
void DuiApplicationPage::appear(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appear(window, policy);
}

// TODO: Remove that now useless method override after API freeze period
void DuiApplicationPage::appearNow(DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appearNow(policy);
}

// TODO: Remove that now useless method after API freeze period
void DuiApplicationPage::appearNow(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    DuiSceneWindow::appearNow(window, policy);
}

// TODO: Remove that now useless method override after API freeze period
void DuiApplicationPage::disappear()
{
    DuiSceneWindow::disappear();
}

// TODO: Remove that now useless method override after API freeze period
void DuiApplicationPage::disappearNow()
{
    DuiSceneWindow::disappearNow();
}

const QString DuiApplicationPage::title() const
{
    return model()->title();
}

void DuiApplicationPage::setTitle(const QString &title)
{
    model()->setTitle(title);
}

void DuiApplicationPage::createContent()
{
    Q_D(DuiApplicationPage);

    d->contentCreated = true;
}

bool DuiApplicationPage::isContentCreated() const
{
    Q_D(const DuiApplicationPage);

    return d->contentCreated;
}

void DuiApplicationPage::setRememberPosition(bool remember)
{
    Q_D(DuiApplicationPage);

    d->rememberPosition = remember;
}

bool DuiApplicationPage::rememberPosition() const
{
    Q_D(const DuiApplicationPage);

    return d->rememberPosition;
}

void DuiApplicationPage::setPannableAreaInteractive(bool interactive)
{
    Q_D(DuiApplicationPage);

    d->pannableViewPort->setEnabled(interactive);
}

bool DuiApplicationPage::isPannableAreaInteractive() const
{
    Q_D(const DuiApplicationPage);

    return d->pannableViewPort->isEnabled();
}

Qt::Orientations DuiApplicationPage::pannableAreaDirection() const
{
    Q_D(const DuiApplicationPage);

    return d->pannableViewPort->panDirection();
}

void DuiApplicationPage::setPannableAreaDirection(Qt::Orientations directions)
{
    Q_D(DuiApplicationPage);

    d->pannableViewPort->setPanDirection(directions);
}

void DuiApplicationPage::actionEvent(QActionEvent *e)
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

// FIXME: After API freeze move that code to enterDisplayEvent().
// Didn't enterDisplayEvent() make createContent() obsolete/redundant altogether?
// Note: Called by DuiApplicationWindow
void DuiApplicationPagePrivate::prepareForAppearance()
{
    Q_Q(DuiApplicationPage);

    if (!contentCreated) {
        contentCreated = true;
        q->createContent();
    }

    updatePannableViewportPosition();
}

void DuiApplicationPagePrivate::updateAutoMarginsForComponents(Dui::Orientation orientation,
        qreal statusBarHeight,
        qreal navigationBarHeight,
        qreal dockWidgetHeight,
        bool dockWidgetVisible)
{
    Q_Q(DuiApplicationPage);

    qreal topHeight = statusBarHeight;
    qreal bottomHeight = 0.0;

    if (q->autoMarginsForComponentsEnabled()) {

        if (q->componentDisplayMode(DuiApplicationPage::NavigationBar) == DuiApplicationPageModel::Show) {
            topHeight += navigationBarHeight;
        }

        if (orientation == Dui::Portrait && dockWidgetVisible) {
            bottomHeight += dockWidgetHeight;
        }
    }

    setSpacerHeight(topSpacer, topHeight);
    setSpacerHeight(bottomSpacer, bottomHeight);
}

DuiApplicationPageModel::ComponentDisplayMode DuiApplicationPage::componentDisplayMode(
    Component component) const
{
    DuiApplicationPageModel::ComponentDisplayMode displayMode;

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

void DuiApplicationPage::setComponentsDisplayMode(Components components,
        DuiApplicationPageModel::ComponentDisplayMode displayMode)
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

bool DuiApplicationPage::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void DuiApplicationPage::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void DuiApplicationPage::setAutoMarginsForComponentsEnabled(bool enabled)
{
    model()->setAutoMarginsForComponentsEnabled(enabled);
}

bool DuiApplicationPage::autoMarginsForComponentsEnabled() const
{
    return model()->autoMarginsForComponentsEnabled();
}

DuiApplicationPageModel::PageEscapeMode DuiApplicationPage::escapeMode() const
{
    return model()->escapeMode();
}

void DuiApplicationPage::setEscapeMode(DuiApplicationPageModel::PageEscapeMode mode)
{
    model()->setEscapeMode(mode);
}

#include "moc_duiapplicationpage.cpp"
