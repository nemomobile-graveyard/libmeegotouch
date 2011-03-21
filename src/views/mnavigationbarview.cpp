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

#include "mnavigationbarview.h"
#include "mnavigationbarview_p.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

#include "mapplication.h"
#include "mnavigationbar.h"
#include "mnavigationbar_p.h"
#include "mapplicationmenubutton.h"
#include "mtoolbar.h"
#include "mviewcreator.h"
#include "mdeviceprofile.h"
#include "mscalableimage.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"

// --------------------------------------------------------------------------
// MNavigationBarViewPrivate
// --------------------------------------------------------------------------

MNavigationBarViewPrivate::MNavigationBarViewPrivate()
    : content(0),
      layout(0),
      menuToolbarEscapePolicy(0),
      escapeToolbarMenuPolicy(0),
      escapeToolbarPolicy(0),
      toolbarPolicy(0),
      toolbarMenuPolicy(0),
      toolBarSlot(0),
      toolBarLayout(0),
      applicationMenuButton(0),
      toolBar(0),
      escapeButtonSlot(0),
      backButton(0),
      closeButton(0),
      q_ptr(0)
{
}

MNavigationBarViewPrivate::~MNavigationBarViewPrivate()
{
    if (toolBar) {
        toolBarLayout->removeItem(toolBar);
        toolBar->setParentItem(0);
        if (qobject_cast<MNavigationBar *>(controller) && controller->scene())
            controller->scene()->removeItem(toolBar);
    }

    // Check if the controller is also being destroyed.
    if (qobject_cast<MNavigationBar *>(controller)) {
        setCustomContent(0); // avoid customContent from being deleted along with the view.
    }

    delete applicationMenuButton;
    delete escapeButtonSlot;
    delete toolBarSlot;
    delete toolBarIsEmptyWatcher;
}

void MNavigationBarViewPrivate::init()
{
    Q_Q(MNavigationBarView);

    QGraphicsLinearLayout* contentLayout = new QGraphicsLinearLayout(controller);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    content = new QGraphicsWidget;
    content->setObjectName("ContentToAnimate");
    contentLayout->addItem(content);

    applicationMenuButton = new MApplicationMenuButton(content);
    applicationMenuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    escapeButtonSlot = new MEscapeButtonSlot(content);
    escapeButtonSlot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    closeButton = new MButton(escapeButtonSlot);
    closeButton->setViewType("icon");

    backButton = new MButton(escapeButtonSlot);
    backButton->setViewType("icon");

    layout = new MLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    content->setLayout(layout);

    toolBarSlot = new QGraphicsWidget(content);
    toolBarSlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBarLayout = new QGraphicsLinearLayout;
    toolBarLayout->setContentsMargins(0, 0, 0, 0);
    toolBarLayout->setSpacing(0);
    toolBarSlot->setLayout(toolBarLayout);

    menuToolbarEscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    menuToolbarEscapePolicy->setSpacing(0);
    menuToolbarEscapePolicy->addItem(applicationMenuButton);
    menuToolbarEscapePolicy->addItem(toolBarSlot);
    menuToolbarEscapePolicy->addItem(escapeButtonSlot);

    escapeToolbarMenuPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    escapeToolbarMenuPolicy->setSpacing(0);
    escapeToolbarMenuPolicy->addItem(escapeButtonSlot);
    escapeToolbarMenuPolicy->addItem(toolBarSlot);
    escapeToolbarMenuPolicy->addItem(applicationMenuButton);

    escapeToolbarPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    escapeToolbarPolicy->setSpacing(0);
    escapeToolbarPolicy->addItem(escapeButtonSlot);
    escapeToolbarPolicy->addItem(toolBarSlot);

    toolbarPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    toolbarPolicy->setSpacing(0);
    toolbarPolicy->addItem(toolBarSlot);

    toolbarMenuPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    toolbarMenuPolicy->setSpacing(0);
    toolbarMenuPolicy->addItem(toolBarSlot);
    toolbarMenuPolicy->addItem(applicationMenuButton);

    customContentPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    customContentPolicy->setSpacing(0);

    layout->setPolicy(toolbarPolicy);

    // Connects button signals
    QObject::connect(applicationMenuButton, SIGNAL(clicked()), controller, SIGNAL(viewmenuTriggered()));
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(closeButtonClicked()));
    QObject::connect(backButton, SIGNAL(clicked()), controller, SIGNAL(backButtonClicked()));

    toolBarIsEmptyWatcher = new MDynamicPropertyWatcher;
    toolBarIsEmptyWatcher->setPropertyName("isEmpty");
    q->connect(toolBarIsEmptyWatcher, SIGNAL(propertyChanged()),
               SLOT(_q_updateIsEmptyAndJustEscapeButtonProperties()));
}

void MNavigationBarViewPrivate::notificationFlagChanged()
{
    // FIXME: Add notification support!
}

void MNavigationBarViewPrivate::updateDockedToolBar()
{
    Q_Q(MNavigationBarView);

    MToolBar *nextToolBar = q->model()->toolBar();

    if (nextToolBar == toolBar)
        return;

    // Make sure the last toolbar is removed first...
    if (toolBar) {
        toolBarIsEmptyWatcher->watch(0);
        q->disconnect(toolBar->model(), SIGNAL(modified(QList<const char*>)));
        toolBarLayout->removeItem(toolBar);
        // previous toolBar is not ours anymore, so clean property we added
        toolBar->setProperty("widgetAlignment", QVariant::Invalid);
        toolBar->setParentItem(0);
    }

    if (nextToolBar) {
        toolBarLayout->addItem(nextToolBar);
        nextToolBar->show();
        toolBarIsEmptyWatcher->watch(nextToolBar);
        q->connect(nextToolBar->model(), SIGNAL(modified(QList<const char*>)),
                   SLOT(_q_toolBarModelChanged(QList<const char*>)));
    }

    toolBar = nextToolBar;
}

void MNavigationBarViewPrivate::updateEscapeButton()
{
    Q_Q(MNavigationBarView);

    backButton->setVisible(q->model()->escapeButtonMode() == MNavigationBarModel::EscapeButtonBack);

    closeButton->setVisible(q->model()->escapeButtonMode() == MNavigationBarModel::EscapeButtonClose
                            && q->style()->hasCloseButton());
}

void MNavigationBarViewPrivate::updateMenuButton()
{
    Q_Q(MNavigationBarView);

    if (q->style()->hasTitle()) {
        applicationMenuButton->setTextVisible(true);
        applicationMenuButton->setArrowIconVisible(q->model()->arrowIconVisible());
    } else {
        applicationMenuButton->setTextVisible(false);
        applicationMenuButton->setArrowIconVisible(true);
        applicationMenuButton->setVisible(q->model()->arrowIconVisible());
    }
}

bool MNavigationBarViewPrivate::isEscapeButtonVisible()
{
    Q_Q(MNavigationBarView);

    return q->model()->escapeButtonVisible() &&
        (q->style()->hasCloseButton() ||
         q->model()->escapeButtonMode() == MNavigationBarModel::EscapeButtonBack);
}

void MNavigationBarViewPrivate::updateLayout()
{
    Q_Q(MNavigationBarView);

    if (q->model()->customContent() != 0) {
        layout->setPolicy(customContentPolicy);
        currentCustomContent.data()->show();
    } else if (q->style()->hasTitle()) {
        layout->setPolicy(menuToolbarEscapePolicy);
    } else {
        bool escapeVisible = isEscapeButtonVisible();
        bool menuVisible = q->model()->arrowIconVisible();

        if (menuVisible && escapeVisible) {
            layout->setPolicy(escapeToolbarMenuPolicy);
        } else if (menuVisible && !escapeVisible) {
            layout->setPolicy(toolbarMenuPolicy);
        } else if (!menuVisible && escapeVisible) {
            layout->setPolicy(escapeToolbarPolicy);
        } else if (!menuVisible && !escapeVisible) {
            layout->setPolicy(toolbarPolicy);
        }
    }
}

void MNavigationBarViewPrivate::_q_updateIsEmptyAndJustEscapeButtonProperties()
{
    Q_Q(MNavigationBarView);
    bool hasContent;
    bool justEscapeButton;
    bool menuButtonVisible = q->model()->arrowIconVisible();

    hasContent = q->model()->customContent() != 0
                 || q->style()->hasTitle()
                 || menuButtonVisible
                 || (toolBar && !toolBar->property("isEmpty").toBool());

    justEscapeButton = !hasContent
            && (q->style()->hasCloseButton()
                || q->model()->escapeButtonMode() == MNavigationBarModel::EscapeButtonBack);

    controller->setProperty("isEmpty", !hasContent && !isEscapeButtonVisible());
    controller->setProperty("justEscapeButton", justEscapeButton);
}

void MNavigationBarViewPrivate::_q_toolBarModelChanged(const QList<const char*>& modifications)
{
    if (modifications.contains(MWidgetModel::ViewType)) {
        updateLayout();
        updateToolBarAlignment();
    }
}

void MNavigationBarViewPrivate::updateToolBarAlignment()
{
    Q_Q(MNavigationBarView);

    if (!toolBar)
        return;

    QVariant alignment = QVariant::Invalid;

    if (!q->style()->hasTitle()) {
        bool escapeVisible = isEscapeButtonVisible();
        bool menuVisible = q->model()->arrowIconVisible();

        if (menuVisible && escapeVisible) {
            alignment = Qt::AlignHCenter;
        } else if (menuVisible && !escapeVisible) {
            alignment = Qt::AlignLeft;
        } else if (!menuVisible && escapeVisible) {
            alignment = Qt::AlignRight;
        } else if (!menuVisible && !escapeVisible) {
            alignment = Qt::AlignJustify;
        }
    }

    toolBar->setProperty("widgetAlignment", alignment);
}

void MNavigationBarViewPrivate::setCustomContent(QGraphicsWidget *customContent)
{
    if (customContent == currentCustomContent.data())
        return;

    if (!currentCustomContent.isNull()) {
        Q_ASSERT(customContentPolicy->count() == 1);

        customContentPolicy->removeItem(currentCustomContent.data());
        layout->removeItem(currentCustomContent.data());

        currentCustomContent.data()->setParent(0);
        currentCustomContent.data()->setParentItem(0);


        if (controller->scene()) {
            controller->scene()->removeItem(currentCustomContent.data());
        }

        currentCustomContent.clear();
    }

    if (customContent) {
        customContentPolicy->addItem(customContent);
        currentCustomContent = customContent;
    }
}

bool MNavigationBarViewPrivate::isToolbarLandscapeTabbar()
{
    return (toolBar && toolBar->viewType() == MToolBar::tabType &&
            controller->sceneManager() && controller->sceneManager()->orientation() == M::Landscape);
}


// --------------------------------------------------------------------------
// MNavigationBarView
// --------------------------------------------------------------------------

MNavigationBarView::MNavigationBarView(MNavigationBar *controller) :
    MSceneWindowView(*(new MNavigationBarViewPrivate()), controller)
{
    Q_D(MNavigationBarView);
    d->q_ptr = this;
    d->init();
}

MNavigationBarView::~MNavigationBarView()
{
}

QRectF MNavigationBarView::boundingRect() const
{
    QRectF br = MWidgetView::boundingRect();
    if( style()->dropShadowImage() ) {
        br.setHeight(br.height() + style()->dropShadowImage()->pixmap()->size().height());
    }
    return br;
}

QPainterPath MNavigationBarView::shape() const
{
    QPainterPath path;
    // Get the base bounding rect, which excludes the shadow
    QRectF br = MWidgetView::boundingRect();

    // The shape is not expanded by the margins+reactiveMargins, as the bar is
    // non-interactive by design
    path.addRect(br);
    return path;
}

void MNavigationBarView::updateData(const QList<const char *>& modifications)
{
    Q_D(MNavigationBarView);
    MSceneWindowView::updateData(modifications);

    bool layoutNeedsUpdate = false;
    bool toolBarAlignmentNeedsUpdate = false;

    foreach( const char *member, modifications) {
        if (member == MNavigationBarModel::NotifyUser) {
            d->notificationFlagChanged();
        } else if (member == MNavigationBarModel::ViewMenuDescription) {
            d->applicationMenuButton->setText(model()->viewMenuDescription());
        } else if (member == MNavigationBarModel::ViewMenuIconID) {
            d->applicationMenuButton->setIconID(model()->viewMenuIconID());
        } else if (member == MNavigationBarModel::ProgressIndicatorVisible) {
            d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
        } else if (member == MNavigationBarModel::ArrowIconVisible) {
            d->updateMenuButton();
            layoutNeedsUpdate = true;
            toolBarAlignmentNeedsUpdate = true;
        } else if (member == MNavigationBarModel::ToolBar) {
            d->updateDockedToolBar();
            layoutNeedsUpdate = true;
            toolBarAlignmentNeedsUpdate = true;
        } else if (member == MNavigationBarModel::EscapeButtonMode) {
            d->updateEscapeButton();
            layoutNeedsUpdate = true;
            toolBarAlignmentNeedsUpdate = true;
        } else if (member == MNavigationBarModel::EscapeButtonEnabled) {
            d->escapeButtonSlot->setEnabled(model()->escapeButtonEnabled());
        } else if (member == MNavigationBarModel::EscapeButtonVisible) {
            d->escapeButtonSlot->setVisible(model()->escapeButtonVisible());
            layoutNeedsUpdate = true;
            toolBarAlignmentNeedsUpdate = true;
        } else if (member == MNavigationBarModel::CustomContent) {
            d->setCustomContent(model()->customContent());
            layoutNeedsUpdate = true;
        }
    }

    if (layoutNeedsUpdate) {
        d->updateLayout();
        d->_q_updateIsEmptyAndJustEscapeButtonProperties();
    }
    if (toolBarAlignmentNeedsUpdate)
        d->updateToolBarAlignment();
}

void MNavigationBarView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MNavigationBarView);
    d->applicationMenuButton->setText(model()->viewMenuDescription());
    d->applicationMenuButton->setIconID(model()->viewMenuIconID());
    d->applicationMenuButton->setProgressIndicatorVisible(model()->progressIndicatorVisible());
    d->escapeButtonSlot->setEnabled(model()->escapeButtonEnabled());
    d->escapeButtonSlot->setVisible(model()->escapeButtonVisible());
    d->setCustomContent(model()->customContent());
}

void MNavigationBarView::applyStyle()
{
    MSceneWindowView::applyStyle();

    Q_D(MNavigationBarView);

    const MNavigationBarStyle *s = static_cast<const MNavigationBarStyle*>(style().operator ->());

    d->applicationMenuButton->setStyleName(s->menuButtonStyleName());
    d->escapeButtonSlot->setStyleName(s->escapeButtonSlotStyleName());
    d->closeButton->setStyleName(s->closeButtonStyleName());
    d->backButton->setStyleName(s->backButtonStyleName());

    d->updateEscapeButton();
    d->updateMenuButton();
    d->updateDockedToolBar();
    d->updateLayout();
    d->_q_updateIsEmptyAndJustEscapeButtonProperties();
    d->updateToolBarAlignment();
}

void MNavigationBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MWidgetView::mousePressEvent(event);

    bool transparent = qFuzzyIsNull(style()->backgroundOpacity());

    // Don't let it propagate to widgets below if background is not transparent
    if (!transparent)
        event->accept();
}

void MNavigationBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MWidgetView::mouseReleaseEvent(event);

    bool transparent = qFuzzyIsNull(style()->backgroundOpacity());

    // Don't let it propagate to widgets below if background is not transparent
    if (!transparent)
        event->accept();
}

void MNavigationBarView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    const MNavigationBarStyle *s = static_cast<const MNavigationBarStyle*>(style().operator ->());

    //draw shadow under the actual navigationbar
    if (s->dropShadowImage() ) {
        s->dropShadowImage()->draw(0.0, size().height(), boundingRect().width(),  (qreal)s->dropShadowImage()->pixmap()->size().height(), painter);
    }

    MWidgetView::drawBackground(painter, option);
}

M_REGISTER_VIEW_NEW(MNavigationBarView, MNavigationBar)

#include "moc_mnavigationbarview.cpp"
