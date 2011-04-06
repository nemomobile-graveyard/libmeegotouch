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

#include <QList>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QFileInfo>
#include <QGesture>
#include <QGestureEvent>
#include <QGraphicsLayout>

#include "mdebug.h"
#include <mondisplaychangeevent.h>
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscene.h"
#include "mscenemanager.h"
#include "mwidget.h"
#include "mapplicationpage.h"
#include "mscene_p.h"
#include "mdeviceprofile.h"
#include "mcancelevent.h"
#include "morientationchangeevent.h"

static const QFont     TextFont                = QFont("Sans", 10);
static const QSize     FpsBoxSize              = QSize(100, 40);
static const QColor    FpsTextColor            = QColor(0xFFFF00);
static const QColor    stylesTextColor         = QColor(0xFF0000); //red
static const QColor    objectsTextColor        = QColor(0x0017FF); //blue
static const QFont     FpsFont                 = QFont("Sans", 15);
static const int       FpsRefreshInterval      = 1000;
static const QString   FpsBackgroundColor      = "#000000";
static const qreal     FpsBackgroundOpacity    = 0.35;
static const QString   BoundingRectLineColor   = "#00F000";
static const QString   BoundingRectFillColor   = "#00F000";
static const qreal     BoundingRectOpacity     = 0.1;
static const qreal     MarginBackgroundOpacity = 0.4;
static const QColor    MarginColor             = QColor(Qt::red);
static const int       MarginBorderWidth       = 2;
static const int       InitialPressTimeout     = 140; //msec

MScenePrivate::MScenePrivate() :
        q_ptr(0),
        manager(0),
        eventEater(new QGraphicsWidget),
        cancelSent(false),
        emuPoint1(1),
        emuPoint2(2),
        pinchEmulationEnabled(false),
        metrics(TextFont)
{
}

MScenePrivate::~MScenePrivate()
{
}

void MScenePrivate::setSceneManager(MSceneManager *sceneManager)
{
    manager = sceneManager;
}

void MScenePrivate::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    Q_Q(MScene);

    if (event->state() == MOnDisplayChangeEvent::FullyOnDisplay ||
            event->state() == MOnDisplayChangeEvent::FullyOffDisplay) {
        // Simple cases. Just forward the event as it is.
        sendEventToMWidgets(filterMWidgets(q->items()), event);
        return;
    }

    QList<MWidget *> fullyOnWidgets;
    QList<MWidget *> partiallyOnWidgets;
    QList<MWidget *> fullyOffWidgets;

    QList<MWidget *> intersectingWidgets = filterMWidgets(q->items(event->viewRect(),
            Qt::IntersectsItemBoundingRect));

    QList<MWidget *> allWidgets = filterMWidgets(q->items());

    // Find who is fully on, partially on and fully off

    QSet<MWidget *> fullyOffWidgetsSet = allWidgets.toSet().subtract(intersectingWidgets.toSet());
    fullyOffWidgets = fullyOffWidgetsSet.toList();

    int intersectingWidgetsCount = intersectingWidgets.count();
    MWidget *widget;
    for (int i = 0; i < intersectingWidgetsCount; i++) {
        widget = intersectingWidgets.at(i);
        if (event->viewRect().contains(widget->sceneBoundingRect())) {
            fullyOnWidgets << widget;
        } else {
            partiallyOnWidgets << widget;
        }
    }

    // Send the events to the corresponding MWidgets

    if (fullyOnWidgets.count() > 0) {
        MOnDisplayChangeEvent fullyOnEvent(MOnDisplayChangeEvent::FullyOnDisplay,
                                             event->viewRect());

        sendEventToMWidgets(fullyOnWidgets, &fullyOnEvent);
    }

    if (fullyOffWidgets.count() > 0) {
        MOnDisplayChangeEvent fullyOffEvent(MOnDisplayChangeEvent::FullyOffDisplay,
                                              event->viewRect());

        sendEventToMWidgets(fullyOffWidgets, &fullyOffEvent);
    }

    if (partiallyOnWidgets.count() > 0) {
        MOnDisplayChangeEvent partiallyOnEvent(MOnDisplayChangeEvent::PartiallyOnDisplay,
                event->viewRect());

        sendEventToMWidgets(partiallyOnWidgets, &partiallyOnEvent);
    }

}

QList<MWidget *> MScenePrivate::filterMWidgets(QList<QGraphicsItem *> itemsList)
{
    QGraphicsItem *item;
    MWidget *mWidget;
    QList<MWidget *> widgetsList;

    int itemsCount = itemsList.count();
    for (int i = 0; i < itemsCount; i++) {
        item = itemsList.at(i);
        if (item->isWidget()) {
            mWidget = qobject_cast<MWidget *>(static_cast<QGraphicsWidget *>(item));
            if (mWidget) {
                widgetsList << mWidget;
            }
        }
    }

    return widgetsList;
}

void MScenePrivate::sendEventToMWidgets(QList<MWidget *> widgetsList, QEvent *event)
{
    Q_Q(MScene);
    MWidget *widget;

    foreach(widget, widgetsList) {
        //we have to check if any of items was removed from scene or deleted
        //while previous ones were handling the event
        if(q->items().contains(widget)){
            q->sendEvent(widget, event);
        }
    }
}

void MScenePrivate::touchPointCopyPosToLastPos(QTouchEvent::TouchPoint &point)
{
    point.setLastPos(point.pos());
    point.setLastScenePos(point.scenePos());
    point.setLastScreenPos(point.screenPos());
}

void MScenePrivate::touchPointCopyMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event)
{
    point.setPos(event->pos());
    point.setScenePos(event->scenePos());
    point.setScreenPos(event->screenPos());
}

void MScenePrivate::touchPointCopyMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event)
{
    point.setStartPos(event->pos());
    point.setStartScenePos(event->scenePos());
    point.setStartScreenPos(event->screenPos());
}

void MScenePrivate::touchPointMirrorMousePosToPointPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event)
{
    Q_Q(MScene);

    if (q->views().size() > 0) {
        QPointF windowPos(q->views().at(0)->pos());
        QSize resolution = MDeviceProfile::instance()->resolution();
        QPointF centerPoint(resolution.width() / 2, resolution.height() / 2);

        QPointF mirrorPoint = centerPoint + (centerPoint - event->screenPos() + windowPos);

        point.setPos(mirrorPoint);
        point.setScenePos(mirrorPoint);
        point.setScreenPos(mirrorPoint + windowPos);
    }
}

void MScenePrivate::touchPointMirrorMousePosToPointStartPos(QTouchEvent::TouchPoint &point, const QGraphicsSceneMouseEvent *event)
{
    Q_Q(MScene);

    if (q->views().size() > 0) {
        QPointF windowPos(q->views().at(0)->pos());
        QSize resolution = MDeviceProfile::instance()->resolution();
        QPointF centerPoint(resolution.width() / 2, resolution.height() / 2);

        QPointF mirrorPoint = centerPoint + (centerPoint - event->screenPos() + windowPos);

        point.setStartPos(mirrorPoint);
        point.setStartScenePos(mirrorPoint);
        point.setStartScreenPos(mirrorPoint + windowPos);
    }
}

bool MScenePrivate::eventEmulateTwoFingerGestures(QEvent *event)
{
    if (MApplication::emulateTwoFingerGestures() &&
        eventEmulatePinch(event)) {
        return true;
    }

    return false;
}

bool MScenePrivate::eventEmulatePinch(QEvent *event)
{
    Q_Q(MScene);

    bool sendTouchEvent = false;
    QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);

    QEvent::Type touchEventType;
    Qt::TouchPointState touchPointState;

    if (QEvent::GraphicsSceneMousePress == event->type()) {
        if (Qt::LeftButton == e->button() && e->modifiers().testFlag(Qt::ControlModifier)) {
            pinchEmulationEnabled = true;

            touchPointMirrorMousePosToPointPos(emuPoint1,e);
            touchPointMirrorMousePosToPointStartPos(emuPoint1,e);
            emuPoint1.setState(Qt::TouchPointPressed);

            touchPointCopyMousePosToPointPos(emuPoint2,e);
            touchPointCopyMousePosToPointStartPos(emuPoint2,e);
            emuPoint2.setState(Qt::TouchPointPressed);

            touchEventType = QEvent::TouchBegin;
            touchPointState = Qt::TouchPointPressed;
            sendTouchEvent = true;
        }
    }

    if (pinchEmulationEnabled && QEvent::GraphicsSceneMouseMove == event->type()) {

        touchPointCopyPosToLastPos(emuPoint1);
        touchPointMirrorMousePosToPointPos(emuPoint1,e);
        emuPoint1.setState(Qt::TouchPointMoved);

        touchPointCopyPosToLastPos(emuPoint2);
        touchPointCopyMousePosToPointPos(emuPoint2,e);
        emuPoint2.setState(Qt::TouchPointMoved);

        touchEventType = QEvent::TouchUpdate;
        touchPointState = Qt::TouchPointMoved;
        sendTouchEvent = true;
    }

    if (pinchEmulationEnabled && QEvent::GraphicsSceneMouseRelease == event->type()) {
        if (Qt::LeftButton == e->button()) {

            touchPointCopyPosToLastPos(emuPoint1);
            emuPoint1.setState(Qt::TouchPointReleased);

            touchPointCopyPosToLastPos(emuPoint2);
            touchPointCopyMousePosToPointPos(emuPoint2,e);
            emuPoint2.setState(Qt::TouchPointReleased);

            touchEventType = QEvent::TouchEnd;
            touchPointState = Qt::TouchPointReleased;
            pinchEmulationEnabled = false;
            sendTouchEvent = true;
        }
    }

    if (sendTouchEvent) {
        QList<QTouchEvent::TouchPoint> touchList;
        touchList.append(emuPoint1);
        touchList.append(emuPoint2);

        QTouchEvent touchEvent(touchEventType, QTouchEvent::TouchPad, Qt::NoModifier, touchPointState, touchList);
        if (q->views().size()>0)
            QApplication::sendEvent(q->views().at(0), &touchEvent);
        q->update();
        return true;
    }

    return false;
}


void MScenePrivate::showFpsCounter(QPainter *painter, float fps)
{
    Q_Q(MScene);

    QString display = QString("FPS: %1").arg((int)(fps + 0.5f));
    /* Draw a simple FPS counter in the lower right corner */
    static QRectF fpsRect(0, 0, FpsBoxSize.width(), FpsBoxSize.height());
    if (!q->views().isEmpty()) {
        MApplicationWindow *window = qobject_cast<MApplicationWindow *>(q->views().at(0));
        if (window) {
            if (manager && manager->orientation() == M::Portrait)
                fpsRect.moveTo(QPointF(window->visibleSceneSize().height() - FpsBoxSize.width(),
                                       window->visibleSceneSize().width() - FpsBoxSize.height()));
            else
                fpsRect.moveTo(QPointF(window->visibleSceneSize().width() - FpsBoxSize.width(),
                                       window->visibleSceneSize().height() - FpsBoxSize.height()));
        }
    }

    painter->fillRect(fpsRect, fpsBackgroundBrush);

    painter->setPen(FpsTextColor);
    painter->setFont(FpsFont);
    painter->drawText(fpsRect,
                      Qt::AlignCenter,
                      display);
}

void MScenePrivate::logFpsCounter(const QTime *timeStamp, float fps)
{
    /* Open fps log-file, if needed */
    if (!fpsLog.output.isOpen()) {
        QFileInfo fi(qApp->arguments().at(0));
        QString appName = fi.baseName();
        QString pid("0");
#ifdef Q_OS_UNIX
        pid = QString().setNum(getpid());
#endif //Q_OS_UNIX

        QString fileName = QString("%1-%2.fps").arg(appName, pid);
        fpsLog.output.setFileName(fileName);
        if (!fpsLog.output.open(QIODevice::WriteOnly | QIODevice::Text)) {
            mWarning("MScene::logFpsCounter") << "Could not open fps log file " << fileName;
            MApplication::setLogFps(false);
            return;
        } else {
            fpsLog.stream.setDevice(&fpsLog.output);
        }
    }
    fpsLog.stream << timeStamp->toString();
    fpsLog.stream << " " << QString("%1").arg(fps, 0, 'f', 1) << endl;
}

void MScenePrivate::fillMarginRectWithPattern(QPainter *painter, const QRectF& rect, int thickness)
{
    if(thickness == 0)
        return;

    QColor fillColor;

    switch(thickness)
    {
    case 24:
        fillColor = QColor(122, 24, 127);
        break;
    case 16:
        fillColor = QColor(250, 25, 0);
        break;
    case 12:
        fillColor = QColor(100, 190, 69);
        break;
    case 8:
        fillColor = QColor(0, 51, 250);
        break;
    case 6:
        fillColor = QColor(138, 93, 59);
        break;
    case 4:
        fillColor = QColor(255, 171, 0);
        break;
    case 2:
        fillColor = QColor(250, 250, 2);
        break;

    default:
        fillColor = QColor(2, 254, 255);
    }

    painter->fillRect(rect, QBrush(fillColor));
    painter->setOpacity(0.6);
    painter->fillRect(rect, QBrush(Qt::black, Qt::BDiagPattern));
}

void MScenePrivate::notifyChildRequestedMouseCancel()
{
    if (!cancelSent)
        sendCancelEvent();
}

void MScenePrivate::sendEventToAllSubLayouts(QGraphicsLayout *layout, QEvent *event)
{
    for (int i = layout->count()-1; i >= 0; --i) {
        // QGraphicsLayout::itemAt allows the subclasses to assume that item index is valid (within the count boundaries).
        // The items might be removed from layout when it'll process the orientation change event.
        if (i >= layout->count())
            i = layout->count() - 1;
        // The layout may end up empty, thus we should early out if it happens
        if (i < 0)
            return;
        QGraphicsLayoutItem * layoutItem = layout->itemAt(i);
        if (layoutItem->isLayout()) {
            QGraphicsLayout *layout = static_cast<QGraphicsLayout *>(layoutItem);
            layout->widgetEvent(event);
            MScenePrivate::sendEventToAllSubLayouts(layout, event);
        }
    }
}

void MScenePrivate::notifySceneAboutOrientationChange(QGraphicsScene *scene, M::Orientation orientation)
{
    if (!scene)
        return;

    MOrientationChangeEvent event(orientation);

    QList<QGraphicsItem *> sceneItems = scene->items();
    for (int i = sceneItems.count() - 1; i >= 0; i--) {
        QGraphicsItem *item = sceneItems.at(i);
        // event handlers might remove items from the scene
        // so we must check if item it's still there
        if (scene->items().contains(item)) {
            scene->sendEvent(item, &event);
            // Sending an event to a widget means only its direct layout gets the orientation
            // change event.
            // To ensure that the event it sent to all layouts, we have to manually send it to
            // the sub-layouts (layout inside a layout)
            if (item->isWidget()) {
                QGraphicsLayout *layout = static_cast<QGraphicsWidget *>(item)->layout();
                if (layout)
                    MScenePrivate::sendEventToAllSubLayouts(layout, &event);
            }
        }
    }
}


void MScenePrivate::resetMouseGrabber()
{
    if (cancelSent) {
        eventEater->ungrabMouse();
        cancelSent = false;
     }
}

void MScenePrivate::sendCancelEvent()
{
    Q_Q(MScene);

    if (q->mouseGrabberItem()) {
        MCancelEvent cancelEvent;
        q->sendEvent(q->mouseGrabberItem(),&cancelEvent);
    }

    if (q->mouseGrabberItem() != eventEater)
        eventEater->grabMouse();

    cancelSent = true;
}

void MScenePrivate::drawObjectNames(QPainter *painter, QList<QGraphicsItem *>::iterator item)
{
    Q_Q(MScene);

    QRectF br = (*item)->boundingRect();
    QPolygonF bp = (*item)->mapToScene(br);

    MWidgetController *widget = 0;
    if ((*item)->isWidget()) {
        widget = qobject_cast<MWidgetController *>(static_cast<QGraphicsWidget*>(*item));
    }

    if (widget) {
        fpsBackgroundBrush.setColor(Qt::transparent);
        painter->setOpacity(1.0);
        QString name = widget->styleName();
        QRectF boundingRect = bp.boundingRect();
        QRect textBoundingRect = metrics.boundingRect(name);
        QPointF center;
        painter->setPen(objectsTextColor);
        if (q->sceneManager()->orientationAngle()== M::Angle0) {
            center = boundingRect.topLeft();
            center += QPointF(0.0 , boundingRect.height() / 1.5);
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->drawText(center, name);
        } else if (q->sceneManager()->orientationAngle() == M::Angle90) {
            center = boundingRect.topLeft();
            center += QPointF(boundingRect.width() / 1, 0.0);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(90);
            painter->drawText(0, 0, name);
            painter->restore();
        } else if (q->sceneManager()->orientationAngle() == M::Angle180) {
            center = boundingRect.topLeft();
            center += QPointF(0.0 , boundingRect.height() / 1.5);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(180);
            painter->drawText(0, 0, name);
            painter->restore();
        } else if (q->sceneManager()->orientationAngle() == M::Angle270) {
            center = boundingRect.bottomLeft();
            center += QPointF(boundingRect.width() / 1, 0.0);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(270);
            painter->drawText(0, 0, name);
            painter->restore();
        }
        fpsBackgroundBrush.setColor(FpsBackgroundColor);
    }
}

void MScenePrivate::drawStyleNames(QPainter *painter, QList<QGraphicsItem *>::iterator item)
{
    Q_Q(MScene);

    QRectF br = (*item)->boundingRect();
    QPolygonF bp = (*item)->mapToScene(br);

    MWidgetController *widget = 0;
    if ((*item)->isWidget()) {
        widget = qobject_cast<MWidgetController *>(static_cast<QGraphicsWidget*>(*item));
    }

    if (widget) {
        fpsBackgroundBrush.setColor(Qt::transparent);
        painter->setOpacity(1.0);
        QString name = widget->styleName();
        QRectF boundingRect = bp.boundingRect();
        QRect textBoundingRect = metrics.boundingRect(name);
        QPointF center;
        painter->setPen(stylesTextColor);
        if (q->sceneManager()->orientationAngle()== M::Angle0) {
            center = boundingRect.topLeft();
            center += QPointF(0.0 , boundingRect.height() / 4);
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->drawText(center, name);
        } else if (q->sceneManager()->orientationAngle() == M::Angle90) {
            center = boundingRect.topLeft();
            center += QPointF(boundingRect.width() / 3, 0.0);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(90);
            painter->drawText(0, 0, name);
            painter->restore();
        } else if (q->sceneManager()->orientationAngle() == M::Angle180) {
            center = boundingRect.topLeft();
            center += QPointF(0.0 , boundingRect.height() / 4);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(180);
            painter->drawText(0, 0, name);
            painter->restore();
        } else if (q->sceneManager()->orientationAngle() == M::Angle270) {
            center = boundingRect.bottomLeft();
            center += QPointF(boundingRect.width() / 3, 0.0);
            painter->save();
            painter->fillRect(textBoundingRect.translated(center.toPoint()), fpsBackgroundBrush);
            painter->translate(center);
            painter->rotate(270);
            painter->drawText(0, 0, name);
            painter->restore();
        }
    }
    fpsBackgroundBrush.setColor(FpsBackgroundColor);
}



MScene::MScene(QObject *parent)
    : QGraphicsScene(parent),
      d_ptr(new MScenePrivate)
{
    Q_D(MScene);

    d->q_ptr = this;
    d->manager = 0;
    QColor fpsBackgroundColor(FpsBackgroundColor);
    fpsBackgroundColor.setAlphaF(FpsBackgroundOpacity);
    d->fpsBackgroundBrush = QBrush(fpsBackgroundColor);

    QColor boundingRectLineColor(BoundingRectLineColor);
    d->boundingRectLinePen = QPen(boundingRectLineColor);

    QColor boundingRectFillColor(BoundingRectFillColor);
    d->boundingRectFillBrush = QBrush(boundingRectFillColor);

    setItemIndexMethod(QGraphicsScene::NoIndex);

    d->eventEater->setParent(this);
    addItem(d->eventEater);
}

MScene::~MScene()
{
    delete d_ptr;
}

MSceneManager *MScene::sceneManager()
{
    Q_D(MScene);

    return d->manager;
}

bool MScene::event(QEvent *event)
{
    Q_D(MScene);

    if (d->eventEmulateTwoFingerGestures(event))
        return true;

    bool retValue = false;
    retValue = QGraphicsScene::event(event);

    if (event->type() == QEvent::GraphicsSceneMouseRelease)
        d->resetMouseGrabber();

    return retValue;
}

void MScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_D(MScene);

    /* Overlay information on the widgets in development mode */
    if (MApplication::showBoundingRect() || MApplication::showSize() || MApplication::showPosition() || MApplication::showMargins() || MApplication::showObjectNames() || MApplication::showStyleNames()) {
        QList<QGraphicsItem *> itemList = items(rect);
        QList<QGraphicsItem *>::iterator item;

        painter->setFont(TextFont);
        int fontHeight = d->metrics.height();
        QTransform rotationMatrix;
        painter->setTransform(rotationMatrix);

        QList<QGraphicsItem *>::iterator end = itemList.end();
        for (item = itemList.begin(); item != end; ++item) {
            QRectF br = (*item)->boundingRect();
            QPolygonF bp = (*item)->mapToScene(br);

            if (MApplication::showBoundingRect()) {
                if (!dynamic_cast<MApplicationPage *>(*item)) { // filter out page for clarity
                    painter->setOpacity(BoundingRectOpacity);
                    painter->setPen(d->boundingRectLinePen);
                    painter->setBrush(d->boundingRectFillBrush);
                    painter->drawPolygon(bp);
                }
            }

            if (MApplication::showMargins()) {
                // Draw content margins
                QGraphicsLayoutItem *layoutItem = dynamic_cast<QGraphicsLayoutItem *>(*item);
                if (layoutItem) {
                    qreal left, top, right, bottom;
                    layoutItem->getContentsMargins(&left, &top, &right, &bottom);

                    QRectF outerRect = (*item)->mapRectToScene(br);
                    QRectF innerRect = (*item)->mapRectToScene(br.adjusted(left, top, -right, -bottom));

                    QRectF leftRect(outerRect.x(), outerRect.y(), innerRect.x() - outerRect.x(), outerRect.height());
                    QRectF topRect(innerRect.x(), outerRect.y(), innerRect.width(), innerRect.y() - outerRect.y());
                    QRectF rightRect(innerRect.bottomRight().x(), outerRect.y(), outerRect.bottomRight().x() - innerRect.bottomRight().x(), outerRect.height());
                    QRectF bottomRect(innerRect.x(), innerRect.bottomRight().y(), innerRect.width(), outerRect.bottomRight().y() - innerRect.bottomRight().y());

                    painter->setOpacity(0.5);

                    d->fillMarginRectWithPattern(painter, leftRect, leftRect.width());
                    d->fillMarginRectWithPattern(painter, topRect, topRect.height());
                    d->fillMarginRectWithPattern(painter, rightRect, rightRect.width());
                    d->fillMarginRectWithPattern(painter, bottomRect, bottomRect.height());

                }

                painter->setOpacity(1.0);
            }

            if (MApplication::showPosition()) {
                QPointF topLeft = ((*item)->mapToScene(br.topLeft()));
                QString posStr = QString("(%1, %2)").arg(topLeft.x()).arg(topLeft.y());
                painter->setPen(Qt::black);
                painter->drawText(topLeft += QPointF(5, fontHeight), posStr);
                painter->setPen(Qt::white);
                painter->drawText(topLeft -= QPointF(1, 1),  posStr);
            }

            if (MApplication::showSize()) {
                QPointF bottomRight = ((*item)->mapToScene(br.bottomRight()));
                QPointF pos = (*item)->mapToScene(br.topLeft());
                QString sizeStr = QString("%1x%2 (%3,%4)").arg(br.width()).arg(br.height()).arg(pos.x()).arg(pos.y());
                painter->setPen(Qt::black);
                painter->drawText(bottomRight -= QPointF(d->metrics.width(sizeStr), 2), sizeStr);
                painter->setPen(Qt::white);
                painter->drawText(bottomRight -= QPointF(1, 1), sizeStr);
            }

            if (MApplication::showObjectNames()) {
                d->drawObjectNames(painter, item);
            }

            if (MApplication::showStyleNames()) {
                d->drawStyleNames(painter, item);
            }
        }
    }

    bool countingFps = MApplication::logFps() || MApplication::showFps();
    if (countingFps) {
        if (d->fps.frameCount < 0) {
            d->fps.time.restart();
            d->fps.frameCount = 0;
        }
        ++d->fps.frameCount;

        int ms = d->fps.time.elapsed();
        if (ms > FpsRefreshInterval) {
            float fps = d->fps.frameCount * 1000.0f / ms;
            d->fps.time.restart();
            d->fps.frameCount = 0;
            if (MApplication::logFps()) {
                QTime time = d->fps.time.currentTime();
                d->logFpsCounter(&time, fps);
            }
            d->fps.fps = fps;
        }

        if (MApplication::showFps()) {
            d->showFpsCounter(painter, d->fps.fps);
        }

        // this update call makes repainting work as fast
        // as possible, and by that prints useful fps numbers
        QTimer::singleShot(0, this, SLOT(update()));
    } else {
        d->fps.frameCount = -1;
    }

    if (MApplication::emulateTwoFingerGestures() &&
        d->pinchEmulationEnabled)
    {
        static const QPixmap *tapPixmap = MTheme::pixmap("meegotouch-tap");
        QPointF pixmapCenterDelta(tapPixmap->width() / 2, tapPixmap->height() / 2);

        painter->drawPixmap(d->emuPoint1.scenePos() - pixmapCenterDelta, *tapPixmap);
        painter->drawPixmap(d->emuPoint2.scenePos() - pixmapCenterDelta, *tapPixmap);
    }
}

#include "moc_mscene.cpp"
