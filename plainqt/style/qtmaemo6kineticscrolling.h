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

#ifndef QTMAEMO6KINETICSCROLLING_H
#define QTMAEMO6KINETICSCROLLING_H

#include <QObject>
#include <QBasicTimer>
#include <QHash>
#include <QPoint>

class QWidget;
class QAbstractScrollArea;

class QtMaemo6KineticScrolling: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int scrollStartDelay READ scrollStartDelay WRITE setScrollStartDelay);
    Q_PROPERTY(int scrollStartOffset READ scrollStartOffset WRITE setScrollStartOffset);
    Q_PROPERTY(int deaccelerationInterval READ deaccelerationInterval WRITE setDeaccelerationInterval);
    Q_PROPERTY(int deaccelerationStrength READ deaccelerationStrength WRITE setDeaccelerationStrength);
    Q_PROPERTY(int maxKineticScrollSpeed READ maxKineticScrollSpeed WRITE setMaxKineticScrollSpeed);
public:
    QtMaemo6KineticScrolling(QObject *parent = 0);
    ~QtMaemo6KineticScrolling();

    /*!
     * Enables the kinetic scrolling on the given scrollarea.
     */
    void enableOn(QAbstractScrollArea *scrollArea);

    /*! \reimp */
    bool eventFilter(QObject *object, QEvent *event);
    /*! \reimp_end */

    /*!
     * \brief Returns the current scroll delay in milliseconds.
     * After a press event a move must been made within this time
     * before the scrolling starts. If there is no move within this time
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 50ms.
     * \see scrollStartOffset
     */
    int scrollStartDelay() const {
        return m_scrollStartDelay;
    };

    /*!
     * \brief Sets the scroll delay in milliseconds.
     * \see scrollStartDelay()
     */
    void setScrollStartDelay(int delay) {
        m_scrollStartDelay = delay;
    };

    /*!
     * \brief Rturns the current scroll offset in milliseconds.
     * After a press event a move must been made with at least this manhattan
     * length before the scrolling starts. If there is no move within this length
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 5.
     * \see scrollStartDelay()
     */
    int scrollStartOffset() const {
        return m_scrollStartOffset;
    };

    /*!
     * \brief Sets the scroll offset in milliseconds.
     * \see scrollStartOffset()
     */
    void setScrollStartOffset(int offset) {
        m_scrollStartDelay = offset;
    };

    /*!
     * \brief Returns the current intervall in milliseconds the deacceleration methode is called.
     * After a press event a move must been made with at least this manhattan
     * length before the scrolling starts. If there is no move within this length
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 20ms.
     */
    int deaccelerationInterval() const {
        return m_deaccelerationInterval;
    };

    /*!
     * \brief Sets the deaccelaration interval in milliseconds.
     * \see deaccelerationInterval()
     */
    void setDeaccelerationInterval(int interval) {
        m_deaccelerationInterval = interval;
    };

    /*!
     * \brief Returns the current deaccelaration strength.
     * this is the value of speed in pixels the kinetic scrolling will be decreased
     * every deaccelaration interval.
     * Default is 1.
     */
    int deaccelerationStrength() const {
        return m_deaccelerationStrength;
    };

    /*!
     * \brief Sets the deaccelaration strength.
     * \see deaccelerationStrength()
     */
    void setDeaccelerationStrength(int strength) {
        m_deaccelerationStrength = strength;
    };

    /*!
     * \brief Returns the max value of the kinetic scrolling speed in pixels per interval.
     * Default is 64.
     */
    int maxKineticScrollSpeed() const {
        return m_maxKineticScrollSpeed;
    };
    /*!
     * \brief Sets the maximum speed of kinetic scrolling in pixels per interval.
     * \see maxKineticScrollSpeed()
     */
    void setMaxKineticScrollSpeed(int speed) {
        m_maxKineticScrollSpeed = speed;
    };

    /*!
     * \brief If application has direction right to left, set this.
     */
    void setRightToLeft(bool rightToLeft) {
        m_rightToLeft = rightToLeft;
    }

protected:
    void timerEvent(QTimerEvent *event);
    void installEventFilter(QObjectList list);

private:
    struct KineticData {
        typedef enum { Waiting, Pressed, Panning, KineticScroll, Stop } State;
        State state;
        QWidget *widget;
        QWidget* pressedWidget;
        QPoint pressPos;
        QPoint offset;
        QPoint dragPos;
        QPoint speed;
        QList<QEvent*> ignored;
    };

    QHash<QWidget*, KineticData*> m_kineticData;
    QBasicTimer m_ticker;

    int m_scrollStartDelay;
    int m_scrollStartOffset;
    int m_deaccelerationInterval;
    int m_deaccelerationStrength;
    int m_maxKineticScrollSpeed;

    bool m_rightToLeft;
};

#endif // QTMAEMO6KINETICSCROLLING_H
