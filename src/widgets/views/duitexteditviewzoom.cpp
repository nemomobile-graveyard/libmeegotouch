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

#include "duitexteditviewzoom.h"

#include <QTimer>
#include <QTimeLine>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

#include "duitextedit.h"
#include "duitexteditview.h"
#include "duitexteditview_p.h"
#include "duideviceprofile.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include <DuiOverlay>

namespace
{
    const int ZoomTimeDuration = 500; // zooming time in msecs
    const int MaxZoomTimeFrames = 50; // maximum frames to use for zooming
    const int ZoomZValue = 100000;  // Z value for the zoom view
    const int MaxZoomingFactor = 2; // size where to zoom
    const int VerticalZoomOffset = 15; // zooming target is viewed a little higher

    const int ScrollMargin = 20; // number of pixels on the edges where to scroll in
    const int ScrollInterval = 100; // interval between scrolling steps
    const int ScrollStep = 15; // size of step in duitexteditview coordinates
}


DuiTextEditViewZoom::DuiTextEditViewZoom(DuiTextEditView *textEditView, const QPointF &start)
    : zoomableView(textEditView),
      zoomFactor(1.0),
      zoomInTimeLine(ZoomTimeDuration),
      zoomOutTimeLine(ZoomTimeDuration),
      parent(new DuiOverlay)
{
    QPointF startPoint(start);

    // kludge
    DuiTextEditViewPrivate *const textEditViewD = textEditView->d_func();
    //set the parent to a toplevel duiscenewindow

    // FIXME - verify if it's the correct scene manager
    textEditViewD->controller->sceneManager()->showWindow(parent);
    this->setParentItem(parent);

    // set zoom target lower if possible -> currently focused point goes up a bit
    startPoint.ry() += VerticalZoomOffset;
    if (startPoint.y() > textEditView->geometry().height()) {
        startPoint.setY(textEditView->geometry().height());
    }

    zoomTarget = startPoint;

    // get the mouse events until mouse is released
    grabMouse();
    setZValue(ZoomZValue);

    // initialize timers
    connect(&zoomInTimeLine, SIGNAL(frameChanged(int)), this, SLOT(zoom(int)));
    zoomInTimeLine.setFrameRange(0, MaxZoomTimeFrames);
    zoomInTimeLine.setLoopCount(1);
    zoomInTimeLine.setCurveShape(QTimeLine::LinearCurve);

    connect(&zoomOutTimeLine, SIGNAL(frameChanged(int)), this, SLOT(zoom(int)));
    zoomOutTimeLine.setFrameRange(0, MaxZoomTimeFrames);
    zoomOutTimeLine.setLoopCount(1);
    zoomOutTimeLine.setCurveShape(QTimeLine::LinearCurve);
    zoomOutTimeLine.setDirection(QTimeLine::Backward);

    // when zoomed out, we are done with this widget
    connect(&zoomOutTimeLine, SIGNAL(finished()), this, SLOT(deleteLater()));

    scrollTimer.setInterval(ScrollInterval);
    connect(&scrollTimer, SIGNAL(timeout()), this, SLOT(scroll()));

    zoomInTimeLine.start();

    // because the zoomingview is a graphicsitem, and not added to layout, so we have to calculate
    // the initial pos here manually. The position is a relative pos, the relative x, y
    // coordinates come from the comparing of its parent's scenePos and contoller(textedit)'s
    // scenePos
    QPointF viewPos = textEditViewD->controller->scenePos();
    viewPos = this->parentItem()->mapFromScene(viewPos);
    setPos(viewPos);
}


DuiTextEditViewZoom::~DuiTextEditViewZoom()
{
    if (scene() && scene()->items().contains(parent)) {
        this->setParentItem(0);
        delete parent;
        parent = 0;
    }
}


QRectF DuiTextEditViewZoom::boundingRect() const
{
    return zoomableView->boundingRect();
}


void DuiTextEditViewZoom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    zoomableView->paint(painter, option, widget);
}


void DuiTextEditViewZoom::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QWidget *widget = event->widget();

    if (widget == 0) {
        return;
    }

    // map screen position to window position
    QPoint windowPos = widget->mapFromGlobal(event->screenPos());

    DuiDeviceProfile *deviceProfile = DuiDeviceProfile::instance();

    // scroll near the window edges
    if (windowPos.x() < ScrollMargin
            || windowPos.x() > deviceProfile->resolution().width() - ScrollMargin
            || windowPos.y() < ScrollMargin
            || windowPos.y() > deviceProfile->resolution().height() - ScrollMargin) {

        if (scrollTimer.isActive() == false) {
            scrollTimer.start();
        }

    } else {
        scrollTimer.stop();
    }

    // shamelessly using Qt's internal API to adjust the event point for vertical offset
    QPointF pos = event->pos();
    pos.ry() -= VerticalZoomOffset;
    event->setPos(pos);

    DuiTextEditViewPrivate *const textEditViewD = zoomableView->d_func();
    textEditViewD->mouseMoveInZooming(event);

    mousePoint = windowPos;
}


void DuiTextEditViewZoom::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    grabMouse();

    // change back to zoom in if we caught press when zooming out
    if (zoomOutTimeLine.state() == QTimeLine::Running) {
        zoomOutTimeLine.stop();
        zoomInTimeLine.setCurrentTime(zoomOutTimeLine.currentTime());
        zoomInTimeLine.start();
    }

    // FIXME: should do scrolling? kindof corner case
}


void DuiTextEditViewZoom::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    ungrabMouse();

    // shouldn't scroll anymore
    scrollTimer.stop();

    // shouldn't either scroll the text inside the view
    DuiTextEditViewPrivate *const textEditViewD = zoomableView->d_func();
    textEditViewD->stopScrolling();

    // start zooming out
    if (zoomInTimeLine.state() == QTimeLine::Running) {
        // if currently zooming in, start zoom out at the same position
        zoomInTimeLine.stop();
        zoomOutTimeLine.setCurrentTime(zoomInTimeLine.currentTime());
        zoomOutTimeLine.resume();
    } else {
        zoomOutTimeLine.start();
    }

}


void DuiTextEditViewZoom::zoom(int frame)
{
    Q_UNUSED(frame);
    zoomFactor = static_cast<qreal>(frame) / MaxZoomTimeFrames * (MaxZoomingFactor - 1) + 1;
    updateTransform();
}


// Really do scrolling.
void DuiTextEditViewZoom::scroll()
{
    // This currently relies on scene coordinates corresponding the resolution
    // of the device.

    // TODO: FIXME - provide a private class to be able to use d->controller->scene()
    DuiWindow *window = DuiApplication::activeWindow();
    if (window == 0)
        return;


    DuiDeviceProfile *deviceProfile = DuiDeviceProfile::instance();

    if (mousePoint.x() < ScrollMargin) {
        // if left is outside view
        if (sceneBoundingRect().left() < 0) {
            switch (window->orientationAngle()) {
            case Dui::Angle90:
                zoomTarget.ry() += ScrollStep;
                break;
            case Dui::Angle180:
                zoomTarget.rx() += ScrollStep;
                break;
            case Dui::Angle270:
                zoomTarget.ry() -= ScrollStep;
                break;
            case Dui::Angle0:
            default:
                zoomTarget.rx() -= ScrollStep;
                break;
            }
        }

    } else if (mousePoint.x() > (deviceProfile->resolution().width() - ScrollMargin)) {
        // map widget height to scene and check if it goes past right edge
        if (sceneBoundingRect().right() > deviceProfile->resolution().width()) {
            switch (window->orientationAngle()) {
            case Dui::Angle90:
                zoomTarget.ry() -= ScrollStep;
                break;
            case Dui::Angle180:
                zoomTarget.rx() -= ScrollStep;
                break;
            case Dui::Angle270:
                zoomTarget.ry() += ScrollStep;
                break;
            case Dui::Angle0:
            default:
                zoomTarget.rx() += ScrollStep;
                break;
            }
        }
    }

    if (mousePoint.y() < ScrollMargin) {
        if (sceneBoundingRect().top() < 0) {
            switch (window->orientationAngle()) {
            case Dui::Angle90:
                zoomTarget.rx() -= ScrollStep;
                break;
            case Dui::Angle180:
                zoomTarget.ry() += ScrollStep;
                break;
            case Dui::Angle270:
                zoomTarget.rx() += ScrollStep;
                break;
            case Dui::Angle0:
            default:
                zoomTarget.ry() -= ScrollStep;
                break;
            }
        }

    } else if (mousePoint.y() > deviceProfile->resolution().height() - ScrollMargin) {
        if (sceneBoundingRect().bottom() > deviceProfile->resolution().height()) {
            switch (window->orientationAngle()) {
            case Dui::Angle90:
                zoomTarget.rx() += ScrollStep;
                break;
            case Dui::Angle180:
                zoomTarget.ry() -= ScrollStep;
                break;
            case Dui::Angle270:
                zoomTarget.rx() -= ScrollStep;
                break;
            case Dui::Angle0:
            default:
                zoomTarget.ry() += ScrollStep;
                break;
            }
        }
    }

    updateTransform();
}


void DuiTextEditViewZoom::updateTransform()
{
}
