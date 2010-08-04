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


MInputWidgetRelocator::MInputWidgetRelocator(const QGraphicsScene *scene,
                                             const QGraphicsItem *rootElement,
                                             M::Orientation initialOrientation)
    : scene(scene),
      rootElement(rootElement),
      orientation(initialOrientation),
      cachedExposedRect(0, 0, -1, -1),
      relocating(false),
      blockRelocationRequests(false)
{
    Q_ASSERT(rootElement);

    // Always update when input method area changes.
    connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
            this, SLOT(update()));
}

MInputWidgetRelocator::~MInputWidgetRelocator()
{
}

void MInputWidgetRelocator::update()
{
    // The 'relocating' check is only for stopping recursion, and every re-entry that may occur must
    // be caused by conscious actions we take in the relocation process.
    if (blockRelocationRequests || relocating) {
        return;
    }

    relocating = true;

    const QGraphicsWidget *inputWidget = focusedWidget();

    // Update input method area
    inputPanelRect = MInputMethodState::instance()->inputMethodArea();

    // Always update screen space
    updateScreenArea();

    if (inputWidget) {
        ensurePannableViewportIsPannable(*inputWidget);
        ensureInputWidgetVisible(*inputWidget);
    } else {
        restoreRelocations();
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

void MInputWidgetRelocator::ensurePannableViewportIsPannable(const QGraphicsWidget &inputWidget)
{
    // Find topmost pannable viewport.
    QGraphicsWidget *parent = inputWidget.parentWidget();
    MPannableViewport *pannable = 0;
    while (parent) {
        if (qobject_cast<MPannableViewport *>(parent)) {
            pannable = static_cast<MPannableViewport *>(parent);
        }
        parent = parent->parentWidget();
    }

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

    // Only if page contains the current focus item we use its exposedContentRect.
    if (!currentPage.isNull() && currentPage->focusItem()) {
        MApplicationPage *page = static_cast<MApplicationPage *>(currentPage.data());
        cachedExposedRect = rootElement->mapRectFromItem(page, page->exposedContentRect()).toRect();
    } else {
        cachedExposedRect = visibleSceneRect();
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

QPoint MInputWidgetRelocator::targetWidgetPosition(const QGraphicsWidget &inputWidget,
                                                   const QRect &exposedRect,
                                                   const QRect &microRect)
{
    // The optimal position could be asked from the input widget itself with
    // custom inputMethodQuery. The widget should know the exposedRect for this.

    // For now, we just center the micro focus rectangle.

    const QPoint widgetPos = rootElement->mapFromItem(&inputWidget, 0, 0).toPoint();
    return widgetPos + (exposedRect.center() - microRect.center());
}

void MInputWidgetRelocator::ensureInputWidgetVisible(const QGraphicsWidget &inputWidget)
{
    // We're in root element's coordinates unless otherwise stated.
    const QRect exposedRect = exposedContentRect();
    const QRect microRect = microFocusRect();
    // If the microfocus is invalid then the focused widget does not want to be relocated.
    // Also check if micro focus is already inside exposed rectangle.
    if (!microRect.isValid()
        || exposedRect.contains(microRect, true)) {
        // No need to relocate.
        return;
    }

    const QPoint targetPosition = targetWidgetPosition(inputWidget, exposedRect, microRect);

    bool isAlreadyVisible = false;

    QGraphicsWidget *parent = inputWidget.parentWidget();
    while (parent) {

        // Map target position to parent coordinates. The point targetPosition is in root element coordinates.
        const QPoint parentTargetPos = parent->mapFromItem(rootElement, targetPosition).toPoint();

        // Map input widget's current position to parent coordinates.
        const QPoint parentCurrentPos = parent->mapFromItem(&inputWidget, 0, 0).toPoint();

        // How much parent should translate its contents.
        const QPoint contentsOffset = parentTargetPos - parentCurrentPos;

        if (!isAlreadyVisible) {
            isAlreadyVisible = exposedRect.contains(microFocusRect(), true);
        }

        relocate(parent, contentsOffset, isAlreadyVisible);

        const QPoint resultingPos = inputWidget.mapToItem(rootElement, 0, 0).toPoint();

        if (resultingPos == targetPosition) {
            break;
        }

        parent = parent->parentWidget();
    }
}

void MInputWidgetRelocator::relocate(QGraphicsWidget *delegate,
                                     const QPoint &contentsOffset,
                                     bool isAlreadyVisible)
{
    if (qobject_cast<MPannableViewport *>(delegate)) {
        relocateByPannableViewport(static_cast<MPannableViewport *>(delegate), contentsOffset);
    } else if (qobject_cast<MSceneWindow *>(delegate)) {
        relocateBySceneWindow(static_cast<MSceneWindow *>(delegate), contentsOffset, isAlreadyVisible);
    }
}

void MInputWidgetRelocator::relocateByPannableViewport(MPannableViewport *viewport,
                                                       const QPoint &contentsOffset)
{
    if (!viewport->isEnabled()) {
        return;
    }

    QPointF panningPos(viewport->position() - contentsOffset);

    // Calculate allowed range for position to be used with MPannableWidget::setPosition().
    QRectF posRange = viewport->range();
    posRange.adjust(0, 0, -viewport->size().width(), -viewport->size().height());

    // ...and limit our panning accordingly. If panningPos is affected by this, it means
    // that we could not finish the relocation completely.
    panningPos.rx() = qBound(posRange.left(), panningPos.x(), posRange.right());
    panningPos.ry() = qBound(posRange.top(), panningPos.y(), posRange.bottom());

    viewport->setPosition(panningPos);

    // Disables kinetic scrolling until next pointer event.
    viewport->physics()->stop();
}

void MInputWidgetRelocator::relocateBySceneWindow(MSceneWindow *sceneWindow,
                                                  const QPoint &offset,
                                                  bool isAlreadyVisible)
{
    // Don't relocate by moving scene window unless really needed.
    if (isAlreadyVisible) {
        return;
    }

    // Don't move navigation bar. Focus can also be in dock widget but because it can get
    // obstructed by input panel we need to move it.
    if (sceneWindow->windowType() == MSceneWindow::NavigationBar) {
        return;
    }

    // Moving currently only in vertical direction.

    // Assuming panel is at the bottom of scene.

    const int sceneWindowBottom = rootElement->mapRectFromItem(sceneWindow, sceneWindow->rect()).toRect().bottom();
    const int distanceFromBottom = visibleSceneRect().height() - 1 - sceneWindowBottom;

    // Negative offset increases distance from bottom.
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
    blockRelocationRequests = true;
}

void MInputWidgetRelocator::handleRotationFinished(M::Orientation orientation)
{
    this->orientation = orientation;
    blockRelocationRequests = false;
    update();
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
