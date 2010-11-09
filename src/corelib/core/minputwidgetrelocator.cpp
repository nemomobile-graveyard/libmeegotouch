#include "minputwidgetrelocator.h"

#include "mapplication.h"
#include "mapplicationpage.h"
#include "mapplicationwindow.h"
#include "mapplicationwindow_p.h"
#include "mdeviceprofile.h"
#include "minputmethodstate.h"
#include "mkeyboardstatetracker.h"
#include "mpannableviewport.h"
#include "mpannableviewport_p.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QTimer>

namespace {
    bool widgetDoesNotSupportRelocation(QGraphicsWidget *widget)
    {
        // We don't know how to scroll QGraphicsWebView. We would only make things worse by trying.
        // Using QObject::inherits() instead of type cast to prevent dependency to QtWebKit.
        return widget->inherits("QGraphicsWebView");
    }
}

Q_DECLARE_OPERATORS_FOR_FLAGS(PostponeRelocationFlags)

MInputWidgetRelocator::MInputWidgetRelocator(const QGraphicsScene *scene,
                                             const QGraphicsItem *rootElement,
                                             M::Orientation initialOrientation)
    : scene(scene),
      rootElement(rootElement),
      orientation(initialOrientation),
      cachedExposedRect(0, 0, -1, -1),
      cachedTopmostPannableViewport(0),
      relocating(false),
      updatePending(false),
      numOfDisappearingSceneWindows(0),
      numOfAppearingSceneWindows(0),
      postponeFlags(0)
{
    Q_ASSERT(rootElement);

    // Always update when input method area changes.
    connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
            this, SLOT(update()));

    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(update()));
}

MInputWidgetRelocator::~MInputWidgetRelocator()
{
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

    // Update input method area
    inputPanelRect = MInputMethodState::instance()->inputMethodArea();

    // Always update screen space
    updateScreenArea();

    if (inputWidget) {
        ensureInputWidgetVisible(*inputWidget);
    } else {
        restoreRelocations();
    }

    // Invalidate cache
    cachedExposedRect.setHeight(-1);
    cachedTopmostPannableViewport = 0;

    relocating = false;
}

void MInputWidgetRelocator::setCurrentPage(const QPointer<MSceneWindow> &page)
{
    currentPage = page;
    cachedExposedRect.setHeight(-1);
}

void MInputWidgetRelocator::buildRelocationOpList(const QGraphicsWidget &inputWidget,
                                                  QList<RelocationOp> &ops,
                                                  bool widgetInitiallyVisible)
{
    ops.clear();

    QGraphicsWidget *widget = inputWidget.parentWidget();
    while (widget) {
        if (qobject_cast<MPannableViewport *>(widget)) {
            cachedTopmostPannableViewport = static_cast<MPannableViewport *>(widget);
            ops.append(RelocationOp(RelocationByPanning, widget));
        } else if (qobject_cast<MSceneWindow *>(widget)) {

            RelocationOp op(RelocationByMovingWindow, widget);

            MSceneWindow *sceneWindow = static_cast<MSceneWindow *>(widget);
            if (sceneWindow->windowType() == MSceneWindow::Dialog) {
                // Actual dialog box of a dialog is defined by its view, if it even has one.
                // For this reason the geometry of a dialog box is not easily known. Still, we should
                // use the unused space often left above small dialogs. To solve the problem scene
                // window can be prepended to operations list.
                ops.prepend(op);
            } else if (widgetInitiallyVisible) {
                // Not a dialog and we are initially visible. Let's not do anything.
                // Dialogs are relocated because the visibility check is not good enough for them.
                ops.clear();
                break;
            } else {
                op.stopIfVisible = true;
                ops.append(op);
            }

            // Stop on first MSceneWindow. This should suffice. Also, we skip any LayerEffect parent MSW's this way.
            break;
        } else if (widgetDoesNotSupportRelocation(widget)) {
            ops.clear();
            break;
        }
        widget = widget->parentWidget();
    }
}

void MInputWidgetRelocator::ensureTopmostViewportIsPannable()
{
    MPannableViewport *pannable = cachedTopmostPannableViewport;

    if (pannable && pannable->isEnabled()) {
        // Topmost pannable viewport found. Set input method area so it knows how to adjust its range.
        QRect coveringPanelArea = pannable->mapRectFromScene(inputPanelRect).toRect() & pannable->rect().toRect();
        pannable->d_func()->setInputMethodArea(coveringPanelArea);
        pannable->physics()->stop();
        if (!pannablesToRestore.contains(pannable)) {
            pannablesToRestore << pannable;
        }
    }
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

void MInputWidgetRelocator::relocationRectangles(const QGraphicsWidget &inputWidget,
                                                 const QRect &microFocusRect,
                                                 QRect &targetRect,
                                                 QRect &localRect)
{
    // The optimal position could be asked from the input widget itself with
    // custom inputMethodQuery. The widget should know the exposedRect for this.

    // For now, we just center the micro focus rectangle.
    const QPoint offset(exposedContentRect().center() - microFocusRect.center());

    targetRect = microFocusRect.translated(offset);
    localRect = inputWidget.mapRectFromItem(rootElement, microFocusRect).toRect();
}

void MInputWidgetRelocator::ensureInputWidgetVisible(const QGraphicsWidget &inputWidget)
{
    const QRect microRect = microFocusRect();
    // If the microfocus is invalid then the focused widget does not want to be relocated.
    if (!microRect.isValid()) {
        return;
    }

    // Get the optimal target position
    QRect targetRect;
    QRect localRect;
    relocationRectangles(inputWidget, microRect,
                         targetRect, localRect);

    relocate(inputWidget, targetRect, localRect);
}

void MInputWidgetRelocator::relocate(const QGraphicsWidget &inputWidget,
                                     const QRect &targetRect,
                                     const QRect &localRect)
{
    bool isAlreadyVisible = !isObscured(inputWidget, localRect);

    QList<RelocationOp> opList;
    buildRelocationOpList(inputWidget, opList, isAlreadyVisible);

    foreach (const RelocationOp &op, opList) {

        if (isAlreadyVisible && op.stopIfVisible) {
            break;
        }

        // Map target rectangle and origin point to delegate's local coordinates.
        const QRect delegateTargetRect(op.delegate->mapRectFromItem(rootElement, targetRect).toRect());
        const QPoint delegateOriginPoint(op.delegate->mapFromItem(&inputWidget, localRect.topLeft()).toPoint());

        switch (op.type) {
        case RelocationByMovingWindow:
            relocateBySceneWindow(static_cast<MSceneWindow *>(op.delegate),
                                  delegateTargetRect, delegateOriginPoint);
            break;
        case RelocationByPanning:
            relocateByPannableViewport(static_cast<MPannableViewport *>(op.delegate),
                                       delegateTargetRect, delegateOriginPoint);
            break;
        }

        const QPoint resultingPos = inputWidget.mapToItem(rootElement, localRect.topLeft()).toPoint();

        if (resultingPos == targetRect.topLeft()) {
            break;
        }

        if (!isAlreadyVisible) {
            isAlreadyVisible = !isObscured(inputWidget, localRect);
        }
    }
}

void MInputWidgetRelocator::moveRectInsideArea(const QRect &area, QRect &rect) const
{
    rect.moveLeft(qBound(area.left(), rect.x(), area.x() + area.width() - rect.width()));
    rect.moveTop(qBound(area.top(), rect.top(), area.y() + area.height() - rect.height()));
}

void MInputWidgetRelocator::relocateByPannableViewport(MPannableViewport *viewport,
                                                       const QRect &targetRect,
                                                       const QPoint &originPoint)
{
    if (!viewport->isEnabled()) {
        return;
    }

    if (viewport == cachedTopmostPannableViewport) {
        // Have to update pannable viewport's range so that we can pan enough.
        ensureTopmostViewportIsPannable();
    }

    // First ensure that target rectangle is inside of area of the pannable viewport.
    // Note: We might even move against the wanted direction but this is required to
    // ensure the visibility of the area marked by target rectangle.
    QRect visibleTargetRect(targetRect);
    moveRectInsideArea(viewport->contentsRect().toRect(), visibleTargetRect);

    // Calculate how much pannable contents should be translated.
    const QPoint contentsOffset(visibleTargetRect.topLeft() - originPoint);

    // Calculate the new panning position, i.e. position of the pannable viewport
    // in panned widget coordinates.
    QPointF panningPos(viewport->position() - contentsOffset);

    // Get allowed range for position to be used with MPannableWidget::setPosition().
    QRectF posRange = viewport->range();

    // ...and limit our panning accordingly.
    panningPos.rx() = qBound(posRange.left(), panningPos.x(), posRange.right());
    panningPos.ry() = qBound(posRange.top(), panningPos.y(), posRange.bottom());

    viewport->setPosition(panningPos);

    // Disables kinetic scrolling until next pointer event.
    viewport->physics()->stop();
}

void MInputWidgetRelocator::relocateBySceneWindow(MSceneWindow *sceneWindow,
                                                  const QRect &targetRect,
                                                  const QPoint &originPoint)
{
    // Don't move navigation bar. Focus can also be in dock widget but because it can get
    // obscured by input panel we need to move it.
    if (sceneWindow->windowType() == MSceneWindow::NavigationBar) {
        return;
    }

    // Moving currently only in vertical direction.

    // Assuming panel is at the bottom of scene.

    const QRect sceneWindowRect(rootElement->mapRectFromItem(sceneWindow, sceneWindow->rect()).toRect());
    const QRect sceneRect(visibleSceneRect());

    const int distanceFromBottom = sceneRect.height() - 1 - sceneWindowRect.bottom();

    // Negative offset increases distance from bottom.
    const QPoint offset(targetRect.topLeft() - originPoint);
    int newDistanceFromBottom = distanceFromBottom - offset.y();

    if (newDistanceFromBottom > 0) {
        // Don't allow gap between panel top and bottom of scene window.
        const QRect mappedPanelRect = rootElement->mapRectFromScene(inputPanelRect).toRect();
        newDistanceFromBottom = qMin(newDistanceFromBottom, mappedPanelRect.height());
    } else {
        newDistanceFromBottom = 0;
    }

    const QPointF displacement(0, distanceFromBottom - newDistanceFromBottom);

    emit sceneWindowDislocationRequest(sceneWindow, displacement);

    // Save so we can restore it.
    if (!sceneWindowsToRestore.contains(sceneWindow)) {
        sceneWindowsToRestore << sceneWindow;
    }

    // If we have pannable viewport as a child we have also moved it relative to input panel.
    // Make sure it's still fully pannable.
    ensureTopmostViewportIsPannable();
}

void MInputWidgetRelocator::restoreRelocations()
{
    // Restore state only for those scene items that are not destroyed
    // and are still in the same scene.

    foreach (MPannableViewport *viewport, pannablesToRestore) {
        if (viewport && viewport->scene() == scene) {
            viewport->d_func()->setInputMethodArea(QRect());
        }
    }
    pannablesToRestore.clear();

    foreach (MSceneWindow *sceneWindow, sceneWindowsToRestore) {
        if (sceneWindow && sceneWindow->scene() == scene) {
            emit sceneWindowUndoDislocationRequest(sceneWindow);
        }
    }
    sceneWindowsToRestore.clear();
}

void MInputWidgetRelocator::handleRotationBegin()
{
    postponeFlags |= WaitForRotationFinished;
}

void MInputWidgetRelocator::handleRotationFinished(M::Orientation orientation)
{
    this->orientation = orientation;
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

QRect MInputWidgetRelocator::microFocusRect() const
{
    const QRectF sceneRect = scene->inputMethodQuery(Qt::ImMicroFocus).toRectF();
    return rootElement->mapRectFromScene(sceneRect).toRect();
}

QRect MInputWidgetRelocator::visibleSceneRect() const
{
    // Assuming our scene uses full screen resolution.
    const QRect sceneRect = QRect(QPoint(), MDeviceProfile::instance()->resolution());
    return rootElement->mapRectFromScene(sceneRect).toRect();
}

bool MInputWidgetRelocator::isObscured(const QGraphicsWidget &widget, const QRect &localRect)
{
    const QRect originRect(widget.mapRectToItem(rootElement, localRect).toRect());

    // We need to know if widget is even partially obscured. Therefore split the
    // local rect into pieces.
    const QRect localTopRect(localRect.topLeft(), QSize(1, 1));
    const QRect localBottomRect(localRect.bottomLeft(), QSize(1, 1));

    return !exposedContentRect().contains(originRect, true)
           || widget.isObscured(localTopRect)
           || widget.isObscured(localBottomRect);
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

MInputWidgetRelocator::RelocationOp::RelocationOp(RelocationOpType type, QGraphicsWidget *delegate)
    : type(type),
      delegate(delegate),
      stopIfVisible(false)
{
}
