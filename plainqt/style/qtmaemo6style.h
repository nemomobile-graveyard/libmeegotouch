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

#ifndef QTMAEMO6STYLE_H
#define QTMAEMO6STYLE_H

#include <QFont>
#include <QMap>
#include <QLabel>
#include "qtmaemo6teststyle.h"

#include <duiexport.h>

class DuiComponentData;
class DuiStyle;
class DuiButtonStyle;
class DuiWidgetStyle;
class DuiScalableImage;
class DuiSliderStyle;
class QScrollBar;
class QTimer;
class QSequentialAnimationGroup;
class QAbstractScrollArea;

class QtMaemo6StylePrivate;


/*!
 * This global function adds opacity to a pixmap
 */
QPixmap setPixmapOpacity(const QPixmap &pixmap, double opacity);

/*!
 * This class emulates the DUI windowdecoration, it provides a emulation
 * of the DUI titlebar, adds scrolling functionallity and places a statusbar
 * on the bottom of the screen, if the application has one.
 * This class inherits indirectly from QPlastiqueStyle and reimplements
 * necessarry methods for doing the correct dui-like styling.
 * Currently it inherits from QtMaemo6TestStyle (inherits QPlastiqueStyle),
 * which is currently only for testing purposes and may be deprecated in future.
 */
class DUI_EXPORT QtMaemo6Style : public QtMaemo6TestStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QtMaemo6Style)
    friend class QtMaemo6StyleEventFilter;
    Q_PROPERTY(int kineticScrollStartDelay READ kineticScrollStartDelay WRITE setKineticScrollStartDelay);
    Q_PROPERTY(int kineticScrollStartOffset READ kineticScrollStartOffset WRITE setKineticScrollStartOffset);
    Q_PROPERTY(int kineticDeaccelerationInterval READ kineticDeaccelerationInterval WRITE setKineticDeaccelerationInterval);
    Q_PROPERTY(int kineticDeaccelerationStrength READ kineticDeaccelerationStrength WRITE setKineticDeaccelerationStrength);
    Q_PROPERTY(int kineticMaxKineticScrollSpeed READ kineticMaxKineticScrollSpeed WRITE setKineticMaxKineticScrollSpeed);
public:
    QtMaemo6Style();
    virtual ~QtMaemo6Style();

    /*! \reimp */
    virtual void init();

    virtual void polish(QApplication *app);
    virtual void polish(QWidget *w);

    virtual void drawPrimitive(PrimitiveElement element,
                               const QStyleOption *option,
                               QPainter *painter,
                               const QWidget *widget = 0) const;

    virtual void drawControl(ControlElement element,
                             const QStyleOption *option,
                             QPainter *painter,
                             const QWidget *widget = 0) const;

    virtual void drawComplexControl(ComplexControl control,
                                    const QStyleOptionComplex *option,
                                    QPainter *painter,
                                    const QWidget *widget = 0) const;

    QSize sizeFromContents(ContentsType type,
                           const QStyleOption *option,
                           const QSize &contentsSize,
                           const QWidget *widget = 0) const;

    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option = 0,
                    const QWidget *widget = 0) const;

    virtual QRect subControlRect(ComplexControl control,
                                 const QStyleOptionComplex *option,
                                 SubControl subControl,
                                 const QWidget *widget = 0) const;
    /*! \reimp_end */

    /*!
     * \brief Returns the current scroll delay in milliseconds.
     *
     * After a press event a move must been made within this time
     * before the scrolling starts. If there is no move within this time
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 50ms.
     * \see kineticScrollStartOffset
     */
    int kineticScrollStartDelay() const;

    /*!
     * \brief Sets the scroll delay in milliseconds.
     *
     * /see kineticScrollStartDelay()
     */
    void setKineticScrollStartDelay(int delay);

    /*!
     * \brief Returns the current scroll offset in milliseconds.
     *
     * After a press event a move must been made with at least this manhattan
     * length before the scrolling starts. If there is no move within this length
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 5.
     * \see kineticScrollStartDelay()
     */
    int kineticScrollStartOffset() const;

    /*!
     * \brief Sets the scroll offset in milliseconds.
     *
     * \see kineticScrollStartOffset()
     */
    void setKineticScrollStartOffset(int offset);

    /*!
     * \brief Returns the current intervall in milliseconds the deacceleration methode
     * is called.
     *
     * After a press event a move must been made with at least this manhattan
     * length before the scrolling starts. If there is no move within this length
     * the mouse press event is forwarded to the widget under the mouse.
     * Default is 20ms.
     */
    int kineticDeaccelerationInterval() const;

    /*!
     * \brief sets the deaccelaration interval in milliseconds
     *
     * /see kineticDeaccelerationInterval()
     */
    void setKineticDeaccelerationInterval(int interval);

    /*!
     * \brief Returns the current deaccelaration strength.
     *
     * This is the value of speed in pixels the kinetic scrolling will be decreased
     * every deaccelaration interval.
     * Default is 1.
     */
    int kineticDeaccelerationStrength() const;

    /*!
     * \brief Sets the deaccelaration strength.
     *
     * \see kineticDeaccelerationStrength()
     */
    void setKineticDeaccelerationStrength(int strength);

    /*!
     * \brief Returns the max value of the kinetic scrolling speed in pixels per interval.
     *
     * Default is 64.
     */
    int kineticMaxKineticScrollSpeed() const;

    /*!
     * \brief Sets the maximum speed of kinetic scrolling in pixels per interval.
     *
     * \see kineticMaxKineticScrollSpeed()
     */
    void setKineticMaxKineticScrollSpeed(int speed);
protected Q_SLOTS:
    /*! \reimp */
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,
                                     const QWidget *widget = 0) const;

    virtual int styleHint(StyleHint hint, const QStyleOption *option = 0,
                          const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const;
    /*! \reimp_end */
};

#endif
