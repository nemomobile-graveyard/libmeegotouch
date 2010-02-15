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

#include <QList>
#include <QGraphicsItem>
#include <QPainter>
#include <QTime>
#include <QTimer>

#include <duiondisplaychangeevent.h>
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiscene.h"
#include "duiscenemanager.h"
#include "duiwidget.h"
#include "duiapplicationpage.h"
#include "duiscene_p.h"

const QFont     TextFont                = QFont("Sans", 10);
const int       MillisecsInSec          = 1000;
const int       FpsRefreshInterval      = 1000;
const QSize     FpsBoxSize              = QSize(100, 40);
const QColor    FpsTextColor            = QColor(0xFFFF00);
const QFont     FpsFont                 = QFont("Sans", 15);
const QString   FpsBackgroundColor      = "#000000";
const qreal     FpsBackgroundOpacity    = 0.35;
const QString   BoundingRectLineColor   = "#00F000";
const QString   BoundingRectFillColor   = "#00F000";
const qreal     BoundingRectOpacity     = 0.1;
const qreal     MarginBackgroundOpacity = 0.4;
const QColor    MarginColor             = QColor(Qt::red);
const int       MarginBorderWidth       = 2;



// FIXME: Probably we should have a DuiScenePrivate class as a matter of policy
static QTime lastUpdate;
static int _frameCount = 0;
static int fps = 0;


DuiScenePrivate::~DuiScenePrivate()
{
}

void DuiScenePrivate::setSceneManager(DuiSceneManager *sceneManager)
{
    manager = sceneManager;
}

void DuiScenePrivate::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    Q_Q(DuiScene);

    if (event->state() == DuiOnDisplayChangeEvent::FullyOnDisplay ||
            event->state() == DuiOnDisplayChangeEvent::FullyOffDisplay) {
        // Simple cases. Just forward the event as it is.
        sendEventToDuiWidgets(filterDuiWidgets(q->items()), event);
        return;
    }

    QList<DuiWidget *> fullyOnWidgets;
    QList<DuiWidget *> partiallyOnWidgets;
    QList<DuiWidget *> fullyOffWidgets;

    QList<DuiWidget *> intersectingWidgets = filterDuiWidgets(q->items(event->viewRect(),
            Qt::IntersectsItemBoundingRect));

    QList<DuiWidget *> allWidgets = filterDuiWidgets(q->items());

    // Find who is fully on, partially on and fully off

    QSet<DuiWidget *> fullyOffWidgetsSet = allWidgets.toSet().subtract(intersectingWidgets.toSet());
    fullyOffWidgets = fullyOffWidgetsSet.toList();

    int intersectingWidgetsCount = intersectingWidgets.count();
    DuiWidget *widget;
    for (int i = 0; i < intersectingWidgetsCount; i++) {
        widget = intersectingWidgets.at(i);
        if (event->viewRect().contains(widget->sceneBoundingRect())) {
            fullyOnWidgets << widget;
        } else {
            partiallyOnWidgets << widget;
        }
    }

    // Send the events to the corresponding DuiWidgets

    if (fullyOnWidgets.count() > 0) {
        DuiOnDisplayChangeEvent fullyOnEvent(DuiOnDisplayChangeEvent::FullyOnDisplay,
                                             event->viewRect());

        sendEventToDuiWidgets(fullyOnWidgets, &fullyOnEvent);
    }

    if (fullyOffWidgets.count() > 0) {
        DuiOnDisplayChangeEvent fullyOffEvent(DuiOnDisplayChangeEvent::FullyOffDisplay,
                                              event->viewRect());

        sendEventToDuiWidgets(fullyOffWidgets, &fullyOffEvent);
    }

    if (partiallyOnWidgets.count() > 0) {
        DuiOnDisplayChangeEvent partiallyOnEvent(DuiOnDisplayChangeEvent::PartiallyOnDisplay,
                event->viewRect());

        sendEventToDuiWidgets(partiallyOnWidgets, &partiallyOnEvent);
    }

}

QList<DuiWidget *> DuiScenePrivate::filterDuiWidgets(QList<QGraphicsItem *> itemsList)
{
    QGraphicsItem *item;
    DuiWidget *duiWidget;
    QList<DuiWidget *> widgetsList;

    int itemsCount = itemsList.count();
    for (int i = 0; i < itemsCount; i++) {
        item = itemsList.at(i);
        if (item->isWidget()) {
            duiWidget = qobject_cast<DuiWidget *>(static_cast<QGraphicsWidget *>(item));
            if (duiWidget) {
                widgetsList << duiWidget;
            }
        }
    }

    return widgetsList;
}

void DuiScenePrivate::sendEventToDuiWidgets(QList<DuiWidget *> widgetsList, QEvent *event)
{
    Q_Q(DuiScene);
    DuiWidget *widget;

    int widgetsCount = widgetsList.count();
    for (int i = 0; i < widgetsCount; i++) {
        widget = widgetsList.at(i);
        q->sendEvent(widget, event);
    }
}

DuiScene::DuiScene(QObject *parent)
    : QGraphicsScene(parent),
      d_ptr(new DuiScenePrivate)
{
    Q_D(DuiScene);

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
}

DuiScene::~DuiScene()
{
    delete d_ptr;
}

DuiSceneManager *DuiScene::sceneManager()
{
    Q_D(DuiScene);

    return d->manager;
}

void DuiScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_D(DuiScene);

    /* Overlay information on the widgets in development mode */
    if (DuiApplication::showBoundingRect() || DuiApplication::showSize() || DuiApplication::showPosition() || DuiApplication::showMargins() || DuiApplication::showObjectNames()) {
        QList<QGraphicsItem *> itemList = items(rect);
        QList<QGraphicsItem *>::iterator item;

        painter->setFont(TextFont);
        QFontMetrics metrics(TextFont);
        int fontHeight = metrics.height();

        QTransform rotationMatrix;
        painter->setTransform(rotationMatrix);

        QList<QGraphicsItem *>::iterator end = itemList.end();
        for (item = itemList.begin(); item != end; ++item) {
            QRectF br = (*item)->boundingRect();
            QPolygonF bp = (*item)->mapToScene(br);

            if (DuiApplication::showBoundingRect()) {
                if (!dynamic_cast<DuiApplicationPage *>(*item)) { // filter out page for clarity
                    painter->setOpacity(BoundingRectOpacity);
                    painter->setPen(d->boundingRectLinePen);
                    painter->setBrush(d->boundingRectFillBrush);
                    painter->drawPolygon(bp);
                }
            }

            if (DuiApplication::showMargins()) {
                // Draw content margins
                QGraphicsLayoutItem *layoutItem = dynamic_cast<QGraphicsLayoutItem *>(*item);
                if (layoutItem) {
                    qreal left, top, right, bottom;
                    layoutItem->getContentsMargins(&left, &top, &right, &bottom);
                    if (left != 0 || top != 0 || right != 0 || bottom != 0) {
                        QPainterPath path;
                        path.addPolygon(bp);
                        path.addPolygon((*item)->mapToScene(br.adjusted(left, top, -right, -bottom)));

                        painter->setOpacity(MarginBackgroundOpacity);
                        painter->fillPath(path, QBrush(MarginColor));
                        painter->setOpacity(1.0);
                        painter->fillPath(path, QBrush(MarginColor, Qt::BDiagPattern));
                        QPen pen(Qt::DashLine);
                        pen.setWidth(MarginBorderWidth);
                        pen.setColor(MarginColor);
                        painter->strokePath(path, pen);
                    }
                }

                painter->setOpacity(1.0);
            }

            if (DuiApplication::showPosition()) {
                QPointF topLeft = ((*item)->mapToScene(br.topLeft()));
                QString posStr = QString("(%1, %2)").arg(topLeft.x()).arg(topLeft.y());
                painter->setPen(Qt::black);
                painter->drawText(topLeft += QPointF(5, fontHeight), posStr);
                painter->setPen(Qt::white);
                painter->drawText(topLeft -= QPointF(1, 1),  posStr);
            }

            if (DuiApplication::showSize()) {
                QPointF bottomRight = ((*item)->mapToScene(br.bottomRight()));
                QString sizeStr = QString("%1x%2").arg(br.width()).arg(br.height());
                painter->setPen(Qt::black);
                painter->drawText(bottomRight -= QPointF(metrics.width(sizeStr), 2), sizeStr);
                painter->setPen(Qt::white);
                painter->drawText(bottomRight -= QPointF(1, 1), sizeStr);
            }

            if (DuiApplication::showObjectNames()) {
                QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(*item);
                if (widget) {
                    QRectF boundingRect = bp.boundingRect();
                    QString name = widget->objectName();
                    QRect textBoundingRect = metrics.boundingRect(name);
                    QPointF center = boundingRect.topLeft();
                    center += QPointF((boundingRect.width() - textBoundingRect.width()) / 2, (boundingRect.height() - fontHeight) / 2);
                    painter->fillRect(textBoundingRect.translated(center.toPoint()), d->fpsBackgroundBrush);
                    painter->setPen(FpsTextColor);
                    painter->drawText(center, name);
                }
            }
        }
    }

    /* Draw a simple FPS counter in the lower right corner */
    if (DuiApplication::showFps()) {
        static QRectF fpsRect(0, 0, FpsBoxSize.width(), FpsBoxSize.height());
        if (!views().isEmpty()) {
            DuiApplicationWindow *window = qobject_cast<DuiApplicationWindow *>(views().at(0));
            if (window) {
                if (d->manager && d->manager->orientation() == Dui::Portrait)
                    fpsRect.moveTo(QPointF(window->visibleSceneSize().height() - FpsBoxSize.width(),
                                           window->visibleSceneSize().width() - FpsBoxSize.height()));
                else
                    fpsRect.moveTo(QPointF(window->visibleSceneSize().width() - FpsBoxSize.width(),
                                           window->visibleSceneSize().height() - FpsBoxSize.height()));
            }
        }

        painter->fillRect(fpsRect, d->fpsBackgroundBrush);
        QTime now = QTime::currentTime();
        ++_frameCount;

        if (lastUpdate.msecsTo(now) > FpsRefreshInterval) {
            fps = (MillisecsInSec * _frameCount) / (lastUpdate.msecsTo(now));
            _frameCount = 0;
            lastUpdate = now;
        }

        QString stringToDraw = QString("FPS: %1").arg(fps);
        painter->setPen(FpsTextColor);
        painter->setFont(FpsFont);
        painter->drawText(fpsRect,
                          Qt::AlignCenter,
                          stringToDraw);

        // this update call makes repainting work as fast
        // as possible, and by that prints useful fps numbers
        QTimer::singleShot(0, this, SLOT(update()));
    }
}
