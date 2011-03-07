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

#include "minputwidgetrelocator.h"
#include "mapplication.h"
#include "mapplicationpage.h"
#include "mapplicationwindow.h"
#include "mapplicationwindow_p.h"
#include "mdeviceprofile.h"
#include "minputmethodstate.h"
#include "mkeyboardstatetracker.h"
#include "mrelocatorstyle.h"
#include "mscenemanager.h"
#include "mscene.h"
#include "mscrollchain.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

namespace {

    const char * const BottomDockingProperty = "dockBottom";

    bool widgetDoesNotWantToBeScrolled(const QGraphicsWidget *widget)
    {
        // We don't know where to scroll QGraphicsWebView. We would only make things worse by trying.
        // Using QObject::inherits() instead of type cast to prevent dependency to QtWebKit.
        return widget->inherits("QGraphicsWebView");
    }
}

Q_DECLARE_OPERATORS_FOR_FLAGS(PostponeRelocationFlags)

MInputWidgetRelocator::MInputWidgetRelocator(MSceneManager &sceneManager,
                                             const QGraphicsItem *rootElement,
                                             M::Orientation initialOrientation)
    : scene(sceneManager.scene()),
      rootElement(rootElement),
      orientation(initialOrientation),
      cachedExposedRect(0, 0, -1, -1),
      oldChain(0),
      relocating(false),
      updatePending(false),
      numOfDisappearingSceneWindows(0),
      numOfAppearingSceneWindows(0),
      postponeFlags(0),
      styleContainer(new MRelocatorStyleContainer())
{
    Q_ASSERT(rootElement);

    // Always update when input method area changes.
    // Important: layouting is started immediately at signal emission. We use QueuedConnection
    // to allow them to start before triggering relocation.
    connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
            this, SLOT(handleInputMethodAreaChange(QRect)),
            Qt::QueuedConnection);

    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(handleKeyboardStateChange()));

    // At this point, if there is no MComponenteData::activeWindow the style container will not use
    // correct orientation. Therefore we set scene manager for it which is used instead.
    // Do before initialize() so orientation gets updated right from the beginning.
    styleContainer->setSceneManager(&sceneManager);
    styleContainer->initialize(objectName(), "", 0);

    // Set initial style mode if needed.
    if (MKeyboardStateTracker::instance()->isOpen()) {
        styleContainer->setModePhysicalKeyboard();
    }
}

MInputWidgetRelocator::~MInputWidgetRelocator()
{
    delete oldChain;
    oldChain = 0;
}

const MRelocatorStyleContainer &MInputWidgetRelocator::style() const
{
    return *styleContainer;
}

void MInputWidgetRelocator::handleKeyboardStateChange()
{
    if (MKeyboardStateTracker::instance()->isOpen()) {
        styleContainer->setModePhysicalKeyboard();
    } else {
        styleContainer->setModeDefault();
    }
    update();
}

void MInputWidgetRelocator::handleInputMethodAreaChange(const QRect &rect)
{
    if (inputPanelRect != rect) {
        inputPanelRect = rect;
        update();
    }
}

void MInputWidgetRelocator::update()
{
    // The 'relocating' check is only for stopping recursion, and every re-entry that may occur must
    // be caused by conscious actions we take in the relocation process.
    if (relocating) {
        return;
    } else if (postponeFlags) {
        updatePending = true;
        return;
    }

    updatePending = false;
    relocating = true;

    const QGraphicsWidget *inputWidget = focusedWidget();

    // Always update screen space
    updateScreenArea();

    if (inputWidget
        && rootElement->isAncestorOf(inputWidget)
        && !widgetDoesNotWantToBeScrolled(inputWidget)) {

        MScrollChain *newChain(new MScrollChain(inputWidget, rootElement));
        if (oldChain) {
            oldChain->restore(newChain);
            delete oldChain;
            oldChain = 0;
        }

        const QRect microRect(microFocusRect(inputWidget));

        if (microRect.isValid() && newChain->count() > 0) {
            const bool widgetWasDocked = scrollDockedWidget(newChain, inputWidget, microRect);

            // If widget was not docked then use the regular rules with nogo zones
            // and an anchor point.
            if (!widgetWasDocked) {
                if (needsRelocation(inputWidget, microRect)) {

                    // Calculate anchor point in root coordinates
                    const QPoint anchorPoint(rootElement->mapFromItem(inputWidget, microRect.topLeft()).x(),
                                             exposedContentRect().top() + style()->verticalAnchorPosition());

                    // First, center context widget to anchorPoint but ensure always top edge
                    // is within exposed content rect. The term "context widget" refers to the first
                    // scrollable parent widget of the input widget, and therefore it can be
                    // used to show user "context" or visibility around the focused input widget.
                    centerContextWidgetToAnchorPoint(newChain, anchorPoint, inputWidget);

                    const QRect targetRect(anchorPoint, microRect.size());
                    newChain->addBottomUpScroll(targetRect, microRect.topLeft());

                    newChain->applyScrolling();
                } else {
                    // No scrolling was needed to be done. However, some earlier scrolling animations might
                    // still be ongoing, moving the input widget to an improper place.
                    newChain->stopScrolling();
                }
            }
        }
        oldChain = newChain;
    } else if (oldChain) {
        oldChain->restore();
        delete oldChain;
        oldChain = 0;
    }

    // Invalidate cache
    cachedExposedRect.setHeight(-1);

    relocating = false;
}

void MInputWidgetRelocator::setCurrentPage(const QPointer<MSceneWindow> &page)
{
    currentPage = page;
    cachedExposedRect.setHeight(-1);
}

const QRect &MInputWidgetRelocator::exposedContentRect()
{
    if (cachedExposedRect.isValid()) {
        return cachedExposedRect;
    }

    cachedExposedRect = visibleSceneRect();

    // Focus may not be in the current page in case input widget is in toolbar or dialog, for example.
    if (!currentPage.isNull() && currentPage->focusItem()) {
        MApplicationPage *page = static_cast<MApplicationPage *>(currentPage.data());
        cachedExposedRect &= rootElement->mapRectFromItem(page, page->exposedContentRect()).toRect();
    } else {
        // check if focused widget is in MSheet
        QGraphicsWidget *inputWidget = focusedWidget();
        QGraphicsWidget *sheetPannableViewport = inputWidget ? inputWidget->parentWidget() : 0;
        while (sheetPannableViewport && sheetPannableViewport->objectName() != "MSheetCentralSlotPannableViewport")
            sheetPannableViewport = sheetPannableViewport->parentWidget();

        if (sheetPannableViewport)
            cachedExposedRect &= rootElement->mapRectFromItem(sheetPannableViewport,
                                                              sheetPannableViewport->rect()).toRect();
    }

    // Map input panel rectangle to rootElement's orientation.
    const QRect mappedPanelRect(rootElement->mapRectFromScene(inputPanelRect).toRect());

    // Need to also handle the case where part of the input panel is outside of the visible scene
    // size, hence the intersection. It is also assumed that the input panel always pops up from
    // the bottom of the visible scene area.
    const int obstructedHeight(mappedPanelRect.intersect(cachedExposedRect).height());
    cachedExposedRect.setHeight(cachedExposedRect.height() - obstructedHeight);

    return cachedExposedRect;
}

void MInputWidgetRelocator::handleRotationBegin(M::Orientation orientation)
{
    postponeFlags |= WaitForRotationFinished;

    if (this->orientation != orientation) {
        this->orientation = orientation;

        // Application orientation and software input panel (SIP) area are out of sync at the
        // time of orientation change. It takes time for the new SIP area to arrive (via d-bus).
        // Therefore the current MInputMethodState::inputMethodArea() is for wrong orientation.
        // Better to use empty.
        //
        // TODO: This makes the assumption that SIP rectangle is different in each orientation.
        //       Otherwise, we are left with the null rect set here because we don't receive any
        //       further inputMethodAreaChanged() notifications after orientation is applied.
        //       Proper fix would be to communicate the aligment and/or angle of the SIP along
        //       with the rect itself.
        inputPanelRect = QRect();
    }
}

void MInputWidgetRelocator::handleRotationFinished()
{
    // Update to portrait or landscape style.
    style().updateCurrentStyle();

    updatePending = true; // Update always after rotation.
    clearPostponeRelocationFlag(WaitForRotationFinished);
}

void MInputWidgetRelocator::sceneWindowStateHasChanged(MSceneWindow *,
                                                       MSceneWindow::SceneWindowState newState,
                                                       MSceneWindow::SceneWindowState oldState)
{
    if (newState == MSceneWindow::Appearing) {
        ++numOfAppearingSceneWindows;
    } else if (newState == MSceneWindow::Disappearing) {
        ++numOfDisappearingSceneWindows;
    }

    if (oldState == MSceneWindow::Appearing) {
        --numOfAppearingSceneWindows;
    } else if (oldState == MSceneWindow::Disappearing) {
        --numOfDisappearingSceneWindows;
    }

    const bool animating = (numOfAppearingSceneWindows > 0)
                           || (numOfDisappearingSceneWindows > 0);

    if (animating) {
        postponeFlags |= WaitForAnimationsFinished;
    } else {
        clearPostponeRelocationFlag(WaitForAnimationsFinished);
    }
}

void MInputWidgetRelocator::clearPostponeRelocationFlag(PostponeRelocationFlag flag)
{
    postponeFlags &= ~flag;

    if (updatePending && !postponeFlags) {
        update();
    }
}

QGraphicsWidget *MInputWidgetRelocator::focusedWidget() const
{
    QGraphicsWidget *focused = 0;

    // Avoid relocation to take place if scene is not viewed by any view.
    // FIXME: Scene still does keep track of last focus item which may become
    // visible and focused at later point in time. Need a trigger for this.
    if (scene->isActive()) {
        QGraphicsItem *focusItem = scene->focusItem();
        if (focusItem) {
            focused = focusItem->isWidget() ?
                          static_cast<QGraphicsWidget *>(focusItem) : focusItem->parentWidget();

            // The widget must accept input method.
            if (focused && !focused->flags().testFlag(QGraphicsItem::ItemAcceptsInputMethod)) {
                focused = 0;
            }
        }
    }
    return focused;
}

QRect MInputWidgetRelocator::microFocusRect(const QGraphicsWidget *inputWidget) const
{
    // It is only possible to retrieve microfocus via scene. Need to map it back to input widget.
    const QRectF sceneRect = scene->inputMethodQuery(Qt::ImMicroFocus).toRectF();
    return inputWidget->mapRectFromScene(sceneRect).toRect();
}

QRect MInputWidgetRelocator::visibleSceneRect() const
{
    // Assuming our scene uses full screen resolution.
    const QRect sceneRect = QRect(QPoint(), MDeviceProfile::instance()->resolution());
    return rootElement->mapRectFromScene(sceneRect).toRect();
}

const MSceneWindow *MInputWidgetRelocator::toolbarParentSceneWindow(const QGraphicsWidget *child) const
{
    const MSceneWindow *result = 0;
    while(child) {
        const MSceneWindow *candidate = qobject_cast<const MSceneWindow *>(child);
        if (candidate) {
            if (candidate->windowType() == MSceneWindow::NavigationBar
                || candidate->windowType() == MSceneWindow::DockWidget) {
                result = candidate;
            }
            // Already found one scene window. Don't look further.
            break;
        }
        child = child->parentWidget();
    }
    return result;
}

bool MInputWidgetRelocator::needsRelocation(const QGraphicsWidget *inputWidget,
                                            const QRect &localRect)
{
    // In case we're relocating an MNavigationBar or an MDockWidget which
    // has a toolbar we need some special handling:
    // - nogo zones are not used
    // - scene window always sits on top of software input panel (bottom-aligned),
    //   or is not relocated at all (top-aligned)
    const MSceneWindow *toolbarSceneWindow = toolbarParentSceneWindow(inputWidget);
    if (toolbarSceneWindow) {
        // Don't relocate top aligned scene windows, and for bottom aligned
        // make sure they share the bottom edge with exposed content rectangle.
        return ((toolbarSceneWindow->alignment() & Qt::AlignBottom)
                && (rootElement->mapRectFromItem(toolbarSceneWindow,
                                                 toolbarSceneWindow->rect()).toRect().bottom()
                    != exposedContentRect().bottom()));
    }

    // The normal case. Check whether widget's rect fits inside allowed area,
    // or whether it is obscured.

    const QRect allowedRect(exposedContentRect().adjusted(0, style()->topNoGoMargin(),
                                                          0, -style()->bottomNoGoMargin()));

    const QRect rect(rootElement->mapRectFromItem(inputWidget, localRect).toRect());

    return !allowedRect.contains(rect)
           || !isWidgetRectFullyVisible(inputWidget, localRect);
}

bool MInputWidgetRelocator::isWidgetRectFullyVisible(const QGraphicsWidget *widget,
                                                     const QRect &localRect) const
{
    // Widget can be clipped by a nested pannable viewport.
    return !widget->isClipped()
           || widget->clipPath().boundingRect().contains(localRect);
}

bool MInputWidgetRelocator::scrollDockedWidget(MScrollChain *chain,
                                               const QGraphicsWidget *inputWidget,
                                               const QRect &microRect)
{
    // Find if we have scroller that wants to be docked to bottom.
    const QGraphicsWidget *dockWidget = 0;
    int dockWidgetId = -1;
    for (int i = 0; i < chain->count(); ++i) {
        QVariant dockBottom(chain->widgetAt(i)->property(BottomDockingProperty));
        if (dockBottom.isValid()
            && dockBottom.toBool()) {
            dockWidgetId = i;
            dockWidget = chain->widgetAt(i);
            break;
        }
    }

    if (!dockWidget) {
        return false;
    }

    // This is the rectangle we need to dock and preferably even keep it fully visible.
    const QRect dockWidgetRect(dockWidget->rect().toRect());

    // Origin of the rectangle has to be given to the chain in inputWidget coordinates.
    const QPoint originPoint = inputWidget->mapFromItem(dockWidget,
                                                        dockWidgetRect.topLeft()).toPoint();

    // Calculate target rectangle for the widget to be docked.
    QRect targetRect = rootElement->mapRectFromItem(dockWidget, dockWidgetRect).toRect();

    bool chainChanged = false;

    // Dock the widget if only it's not already at correct position.
    if (targetRect.bottom() != exposedContentRect().bottom()
        || !isWidgetRectFullyVisible(dockWidget, dockWidgetRect)) {

        // Move it to bottom of exposed content rectangle.
        targetRect.moveBottom(exposedContentRect().bottom());

        // Docking is done by parents of the dockWidget, therefore start at the next scroller.
        chain->addBottomUpScroll(targetRect, originPoint, dockWidgetId + 1);

        chainChanged = true;
    }

    // We still need to bring cursor visible, although we cannot guarantee it because
    // docking at a right place is of higher priority. Use dockWidget and its children
    // scrollers to scroll cursor rectangle visible. Don't touch parents of dockWidget.
    if (!isWidgetRectFullyVisible(inputWidget, microRect)) {
        chain->addMinimalScroll(microRect, 0, dockWidgetId);
        chainChanged = true;
    }

    if (chainChanged) {
        chain->applyScrolling();
    }

    return true;
}

void MInputWidgetRelocator::centerContextWidgetToAnchorPoint(MScrollChain *newChain,
                                                             const QPoint &anchorPoint,
                                                             const QGraphicsWidget *inputWidget)
{
    const QGraphicsWidget *contextWidget = newChain->widgetAt(0);
    const int yTarget = qMax<int>(exposedContentRect().top(),
                                  anchorPoint.y() - contextWidget->size().height() / 2);
    const QRect targetRect(QPoint(anchorPoint.x(), yTarget), contextWidget->size().toSize());

    const QPoint originPoint(contextWidget->mapToItem(inputWidget, 0, 0).toPoint());
    newChain->addBottomUpScroll(targetRect, originPoint, 1);
}

bool MInputWidgetRelocator::needsMoreScreenArea() const
{
    // We need to maximize available screen area when software input panel (SIP) pops up
    // because it takes a lot of screen space itself.
    // It is assumed that when orientation is Portrait we have enough room.
    // Also, never request more room when hardware keyboard is open.

    bool isInputPanelPresent = !inputPanelRect.isEmpty();
    const bool hwKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();

    // How to know if panel is temporarily hidden?
    if (!hwKeyboardOpen
        && !isInputPanelPresent
        && scene->hasFocus()) {
        QVariant visualPrioritized = scene->inputMethodQuery(
            static_cast<Qt::InputMethodQuery>(M::VisualizationPriorityQuery));
        if (visualPrioritized.isValid()
            && visualPrioritized.toBool()) {
            // Input panel is there, it is just hidden temporarily.
            isInputPanelPresent = true;
        }
    }

    return orientation == M::Landscape
           && !hwKeyboardOpen
           && isInputPanelPresent;
}

void MInputWidgetRelocator::updateScreenArea()
{
    if (needsMoreScreenArea()) {
        maximizeScreenArea();
    } else {
        restoreScreenArea();
    }
}

void MInputWidgetRelocator::maximizeScreenArea()
{
    // Do here whatever can be done to gain more screen space.

    foreach (QGraphicsView *view, scene->views()) {
        // If we have an application window then hide its decorations.
        MApplicationWindow *appWnd = qobject_cast<MApplicationWindow *>(view);
        if (appWnd) {
            appWnd->d_func()->maximizePageArea();
        }
    }
}

void MInputWidgetRelocator::restoreScreenArea()
{
    foreach (QGraphicsView *view, scene->views()) {
        // If we have an application window then restore its decorations.
        MApplicationWindow *appWnd = qobject_cast<MApplicationWindow *>(view);
        if (appWnd) {
            appWnd->d_func()->restorePageArea();
        }
    }
}
