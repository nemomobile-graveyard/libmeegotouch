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

protected Q_SLOTS:
    QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,
                                     const QWidget *widget = 0) const;

    virtual int styleHint(StyleHint hint, const QStyleOption *option = 0,
                          const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const;
    /*! \reimp_end */
};

#endif
