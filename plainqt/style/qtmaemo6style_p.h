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

#ifndef QTMAEMO6STYLE_P_H
#define QTMAEMO6STYLE_P_H

#include <QPointer>

#include "qtmaemo6style.h"
#include "qtmaemo6teststyle_p.h"
#include "qtmaemo6styleeventfilter.h"
#include "qtmaemo6scrollbareventfilter.h"

class MComponentData;
class MWidgetController;
class QToolButton;
class QAction;
class QStatusBar;
class QMenuBar;
class QtMaemo6KineticScrolling;
class QStyleOptionButton;

class QtMaemo6StylePrivate : public QtMaemo6TestStylePrivate
{
public:
    Q_DECLARE_PUBLIC(QtMaemo6Style)

    QtMaemo6StylePrivate();
    virtual ~QtMaemo6StylePrivate();

    /*!
     * Initialization of the M framework.
     * init() gets called by the style constructor.
     * At that point we don't know yet whether the application object is a M
     * application or not. As we only need to fully initialize the M framework
     * for non-M application we need to defer the initialization of the M
     * framework to a later point of the application life cycle.
     */
    void initM();

    /*!
     * returns a MStyle corresponding the given parameters
     */
    static const MStyle *mStyle(QStyle::State state,
                                    const QString &styleClass,
                                    const QString &styleObject = QString(),
                                    const QString &type = QString(),
                                    const MWidgetController *parent = NULL);

    /*!
     * returns a string generated from the QStyle::state, m can use
     */
    static QString modeFromState(QStyle::State state) ;

    /*!
     * draws the background of the widget
     */
    static void drawWindowBackground(QWidget *);
    static void drawWidgetBackground(QPainter *p,
                                     const QStyleOption *option,
                                     const QRect &rect,
                                     const MWidgetStyle *style);

    /*!
     * draws a scalable image
     */
    static void drawScalableImage(QPainter *p,
                                  const QStyleOption *option,
                                  const QRect &rect,
                                  const MScalableImage *scalableImage,
                                  const MWidgetStyle *style,
                                  const QString &purpose = "bg",
                                  bool enableCache = true);

    /*!
     * draws the background of a slider
     */
    static void drawSliderBaseBackground(QPainter *p,
                                         const QStyleOption *option,
                                         const QRect &rect,
                                         const MSliderStyle *style,
                                         int maxSliderLength);

    /*!
     * Draws the text on the button.
     * \param style the m style which is used to draw the button
     * \param painter
     * \param textRect the rect within the button in which the text (and icon) is drawn
     * \param text the text
     * \param align the alignment the text uses within the textRect!!
     * \param font font used instead of font from the style
     */
    void drawButtonText(const MButtonStyle *style,
                        QPainter *painter,
                        const QRectF &textRect,
                        const QString &text,
                        Qt::Alignment align,
                        const QFont &font) const;
    /*!
     * Draws the text on the button.
     *  overload: uses the font from style to draw the text
     * \param style the m style which is used to draw the button
     * \param painter
     * \param textRect the rect within the button in which the text (and icon) is drawn
     * \param text the text
     * \param align the alignment the text uses within the textRect!!
     */
    void drawButtonText(const MButtonStyle *style,
                        QPainter *painter,
                        const QRectF &textRect,
                        const QString &text,
                        Qt::Alignment align) const;

    /*!
     * Draws the icon on the button.
     * \param style the m style which is used to draw the button
     * \param painter
     * \param contentsRect the rect within the button in which the text (and icon) is drawn
     * \param icon the icon
     * \param iconSize optional overrides the icon size from style
     */
    void drawButtonIcon(const MButtonStyle *style,
                        QPainter *painter,
                        const QRect &contentsRect,
                        const QIcon &icon,
                        const QSize &iconSize = QSize()) const;

    /*!
     * Draws the button.
     * \param painter
     * \param text the text on the button
     * \param icon the icon on the button
     * \param rect the rect in which the button is drawn
     * \param option the QStyleOption used to draw the button
     * \param style the m style used to draw the button
     */
    void drawBasicButton(QPainter *painter,
                         const QString &text,
                         const QIcon &icon,
                         const QRect &rect,
                         const QStyleOption *option,
                         const MButtonStyle *style) const;

    /*!
     * Draws the button (overloaded)
     * \param painter
     * \param text the text on the button
     * \param icon the icon on the button
     * \param rect the rect in which the button is drawn
     * \param option the QStyleOption used for the button
     * \param styleClass style class that should be used
     * \param styleObject style object that should be used
     */
    void drawBasicButton(QPainter *painter,
                         const QString &text,
                         const QIcon &icon,
                         const QRect &rect,
                         const QStyleOption *option,
                         const QString &styleClass,
                         const QString &styleObject = QString()) const;

    /*!
     * Draws the button.
     * \param painter
     * \param text the text on the button
     * \param icon the icon on the button
     * \param rect the rect in which the button is drawn
     * \param option the QStyleOption used to draw the button
     * \param style the m style used to draw the button
     * \param font use this font instead of the font from the style
     * \param font use this icon size instead of icon size from the style
     */
    void drawBasicButton(QPainter *painter,
                         const QString &text,
                         const QIcon &icon,
                         const QRect &rect,
                         const QStyleOption *option,
                         const MButtonStyle *style,
                         const QFont &font,
                         const QSize &iconSize) const;

    /*!
     * Draws the toggle button
     * \param painter
     * \param text the text on the button
     * \param rect the rect in which the button is drawn
     * \param icon the icon on the button
     * \param option the QStyleOption used to draw the button
     * \param styleClass style class that should be used
     * \param styleObject style object that should be used
     */
    void drawToggleButton(QPainter *painter,
                          const QString &text,
                          const QIcon &icon,
                          const QRect &rect,
                          const QStyleOption *option,
                          const QString &styleClass,
                          const QString &styleObject = QString()) const;

    /*!
     * Draws the checkbox
     * \param painter
     * \param text the text besides the checkBox
     * \param rect the rect in which the checkBox is drawn
     * \param icon the icon on the button
     * \param option the QStyleOption used to draw the checkBox
     */
    void drawCheckBox(QPainter *painter,
                      const QString &text,
                      const QIcon &icon,
                      const QRect &rect,
                      const QStyleOption *option) const;

    /*!
     * calculates a rect the text and icon earn
     * returns a rect within the button's rect used by the text and icon
     * the rect already includes the margins
     * \param style the m style used to draw the button
     * \param text the text on the button
     * \param icon optional the icon on the button
     * \param font optional used instead of the font from the style
     * \param iconSize optional used instead of iconSize from the style if valid
     */
    QRect getTextAndIconRect(const MButtonStyle *style,
                             const QString &text,
                             const QIcon &icon = QIcon(),
                             const QFont &font = QFont(),
                             const QSize &iconSize = QSize()) const;
    /*!
     * returns the rect of the text with font font
     * includes no margins
     * \param text the text
     * \param font font used to calculate the text size
     */
    QRect textBoundingRect(const QString &text, const QFont &font) const;

    /*!
     * returns the effective padding used for the style
     * uses the borders of the MScalableImage, but if the padding specified in the style is
     * greater than the border of the MScalableImage, the borders from style are used
     * \param style the used style
     * \param left border in the left side
     * \param top border in the top side
     * \param right border in the right side
     * \param bottom border in the bottom side
     */
    void paddingFromStyle(const MWidgetStyle *style,
                          int *left,
                          int *top,
                          int *right,
                          int *bottom) const;

    /*!
     * returns the rect a slider uses
     */
    QRect scrollBarSliderRect(const QStyleOptionComplex *option, const QWidget *widget = 0) const;

    /*!
     * \brief returns a Pixmap with the given borders cropped away
     * The given borders of the scalable image are cut away, the resulting pixmap will
     * have size as size. If drawLines is true, lines are drawn on the cut away borders
     * to close the pixmap. The color of these lines is taken from one of the pixmaps
     * outermost pixels.
     * \param image the image used as base
     * \param size target size of the pixmap
     * \param borders combined Borders flags that should be cut away
     * \param borderLines optional draw closing lines on the given border sides. If not
     *        given border lines are drawn for all cut borders
     */
    enum Borders {
        leftBorder = 1,
        topBorder = 2,
        rightBorder = 4,
        bottomBorder = 8
    };
    QPixmap borderCroppedPixmap(const MScalableImage* image, QSize size, int borders, int borderLines) const;
    QPixmap borderCroppedPixmap(const MScalableImage* image, QSize size, int borders) const {
        return borderCroppedPixmap(image, size, borders, borders);
    }

    /*!
     * returns an inverted Alignment, so align right will become align left, top
     * will become bottom and so on
     */
    Qt::Alignment invertAlignment(Qt::Alignment align) const;

    bool hasVerticalAlignment(Qt::Alignment align) const {
        return (align & 0xF0) > 0;
    };
    bool hasHorizontalAlignment(Qt::Alignment align) const {
        return (align & 0xF) > 0;
    };
    Qt::Alignment verticalAlignment(Qt::Alignment align) const {
        return align & 0xF0;
    };
    Qt::Alignment horizontalAlignment(Qt::Alignment align) const {
        return align & 0xF;
    };

    void ensureWidgetVisible(QWidget* widget, QRect visibleArea);

    struct WidgetPos {
        WidgetPos() : widget(NULL) {};
        WidgetPos(QWidget* w, QPoint p) : widget(w), position(p) {};
        WidgetPos(const WidgetPos& other) { widget = other.widget; position = other.position; }
        QWidget* widget;
        QPoint position;
    };

    WidgetPos m_originalWidgetPos;

public:
    QList<QToolButton *> m_toolButtonsInTitleBar;
    QList<QAction *> m_toolBarActions;
    QPointer<QStatusBar> m_statusBar;

    const int m_actionsInTitleBarCount;

    MComponentData *m_componentData;
    bool m_isMInitialized;
    bool m_isMApplication;

    QtMaemo6ScrollBarEventFilter *m_scrollBarEventFilter;
    QtMaemo6StyleEventFilter *m_windowEventFilter;

    QMenuBar *m_menuBar;

    QtMaemo6KineticScrolling* m_kinetic;

    QList<QString> m_excludeClasses;
};

#endif
