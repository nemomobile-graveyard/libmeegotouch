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
#include "mapplicationpagemodel.h"
#include "mapplicationwindow.h"
#include "mwidgetview.h"
#include "mondisplaychangeevent.h"
#include "mwidgetcreator.h"
#include "mpannableviewport.h"

#include <QAction>
#include <QGraphicsLinearLayout>

M_REGISTER_WIDGET(MApplicationPage)

MApplicationPagePrivate::MApplicationPagePrivate() :
    pannableViewport(0),
    panDirection(0),
    contentCreated(false)
{}

void MApplicationPagePrivate::init()
{
    Q_Q(MApplicationPage);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0.0);

    pannableViewport = new MPannableViewport;
    pannableViewport->setClipping(false);
    panDirection = pannableViewport->panDirection();

    layout->addItem(pannableViewport);

    q->setLayout(layout);

    q->connect(q,
               SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState,
                                              MSceneWindow::SceneWindowState)),
               SLOT(_q_onSceneWindowStateChanged(MSceneWindow::SceneWindowState,
                                                 MSceneWindow::SceneWindowState)));
}

void MApplicationPagePrivate::_q_onCentralWidgetDestroyed()
{
    Q_Q(MApplicationPage);
    q->model()->setCentralWidget(0);
}

void MApplicationPagePrivate::_q_onSceneWindowStateChanged(
        MSceneWindow::SceneWindowState newState, MSceneWindow::SceneWindowState oldState)
{
    Q_Q(MApplicationPage);

    if (oldState == MSceneWindow::Disappeared &&
            (newState == MSceneWindow::Appearing || newState == MSceneWindow::Appeared)) {

        if (!contentCreated) {
            contentCreated = true;
            q->createContent();
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
    Q_D(MApplicationPage);
    setCentralWidget(0);

    // If the application quits while this page is being displayed, it might happen
    // that MNavigationBar gets destroyed before this page. In that case, since
    // the custom content will be a child of MNavigationBar in the scene graph, it
    // could have been detroyed along with it.
    // Therefore, we only delete the custom content if we still have it.
    if (!d->customNavigationBarContent.isNull()) {
        delete d->customNavigationBarContent.data();
        d->customNavigationBarContent.clear();
    }
}

void MApplicationPage::setCentralWidget(QGraphicsWidget *centralWidget)
{
    if (model()->centralWidget())
        delete model()->centralWidget();

    model()->setCentralWidget(centralWidget);

    if (centralWidget)
        connect(centralWidget, SIGNAL(destroyed()), SLOT(_q_onCentralWidgetDestroyed()));
}

QGraphicsWidget *MApplicationPage::centralWidget()
{
    return model()->centralWidget();
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
    model()->setRememberPosition(remember);
}

bool MApplicationPage::rememberPosition() const
{
    return model()->rememberPosition();
}

void MApplicationPage::setPannable(bool pannable)
{
    Q_D(MApplicationPage);

    if (pannable)
        d->pannableViewport->setPanDirection(d->panDirection);
    else
        d->pannableViewport->setPanDirection(0);
}

bool MApplicationPage::isPannable() const
{
    Q_D(const MApplicationPage);

    return d->pannableViewport->panDirection() != 0;
}

void MApplicationPage::setPanningDirection(Qt::Orientations direction)
{
    Q_D(MApplicationPage);

    d->panDirection = direction;
    d->pannableViewport->setPanDirection(direction);
}

Qt::Orientations MApplicationPage::panningDirection() const
{
    Q_D(const MApplicationPage);

    return d->pannableViewport->panDirection();
}

void MApplicationPage::actionEvent(QActionEvent *e)
{
    switch (e->type()) {
    case QEvent::ActionRemoved:
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
    MSceneWindow::enterDisplayEvent();
}

void MApplicationPagePrivate::doEnterDisplayEvent()
{
    Q_Q(MApplicationPage);

    if (!contentCreated) {
        contentCreated = true;
        q->createContent();
    }

    MWidgetPrivate::doEnterDisplayEvent();
}

void MApplicationPagePrivate::setExposedContentRect(const QRectF &exposedContentRect)
{
    Q_Q(MApplicationPage);

    if (exposedContentRect != q->model()->exposedContentRect()) {
        q->model()->setExposedContentRect(exposedContentRect);
        emit q->exposedContentRectChanged();
    }
}

QRectF MApplicationPage::exposedContentRect() const
{
    return model()->exposedContentRect();
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

    return d->pannableViewport;
}

void MApplicationPage::setCustomNavigationBarContent(QGraphicsWidget *customNavigationBarContent)
{
    Q_D(MApplicationPage);
    QGraphicsWidget *oldCustomNavigationBarContent = 0;

    if (customNavigationBarContent == d->customNavigationBarContent.data())
        return;

    oldCustomNavigationBarContent = d->customNavigationBarContent.data();

    d->customNavigationBarContent = customNavigationBarContent;

    // Let MApplicationWindow remove this content from the navigation bar
    emit customNavigationBarContentChanged();

    if (oldCustomNavigationBarContent)
        delete oldCustomNavigationBarContent;
}

QGraphicsWidget *MApplicationPage::customNavigationBarContent()
{
    Q_D(MApplicationPage);
    return d->customNavigationBarContent.data();
}

#include "moc_mapplicationpage.cpp"
