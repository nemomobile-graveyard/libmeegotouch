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

#ifndef QTMAEMO6WINDOW
#define QTMAEMO6WINDOW

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QPen>

class QGridLayout;

/*!
 * this is an abstract base class for emulating m windows, such as the
 * application window, dialogs or menus.
 * It may put some decoration around the window and puts the original Widget
 * into a scrollarea, if it is not a scrollarea by itself.
 * The window uses a layout like
 *     +------------------------------------------+
 *     |              topSpacer                   |
 *     +------------+---------------+-------------+
 *     | leftSpacer | centralWidget | rightSpacer |
 *     +------------+---------------+-------------+
 *     |              bottomSpacer                |
 *     +------------------------------------------+
 * the centralWidget is automatically added to the layout's center
 * if you wan't to add spacing you must add the spacers by yourself
 */
class QtMaemo6Window : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Consturct a new window
     * \param originalWidget the widget that should be embedded in this window
     * \param parent the parent widget
     */
    explicit QtMaemo6Window(QWidget *originalWidget, QWidget *parent = NULL);

    virtual ~QtMaemo6Window();

    /*!
     * returns the maximum size the viewport of the scrollarea
     * may take
    */
    QSize maxViewportSize() const;

    /*!
     * shows the window full screen
     * use this instead of showMaximized()
     */
    void showFastMaximized();

    /*!
     * \brief sets the widget that is shown inside the window
     * the given widget will be used as the central widget within this window.
     * The widget is not added to any layout, this must be done by the user.
     * If the widget inherits QAbstractScrollArea, it will be added directly. If
     * it does not, a scrollArea will be created and the widget is set as the
     * scrollarea's viewport. Note: in this case, the centralWidget of the window
     * will be the scrollArea, not the given widget!
     */
    void setCentralWidget(QWidget *widget);

    /*!
     * \brief returns the central widget of this window
     * If no widget is set, this returns NULL. The returned widget may not be
     * the widget, which was set with setCentralWidget().
     * \see setCentralWidget()
     */
    QWidget* centralWidget() const;

    /*!
     * \brief returns the widget set with central widget
     * in opposite to centralWidget() this always returns the added widget.
     */
    QWidget* widget() const { return m_window; };

protected:
    QtMaemo6Window() {};

    /*! \reimp */
    virtual void closeEvent(QCloseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void paintEvent(QPaintEvent* e);
    /*! \reimp_end */

protected:
    QGridLayout *m_windowLayout;
private:
    QPointer<QWidget> m_window;
    QAbstractScrollArea *m_centralWidget;
    QScrollArea *m_scrollArea;

    Qt::WindowFlags m_originalFlags;
    bool m_closeFromChild;
    bool m_hideFromChild;
};

#endif //QTMAEMO6WINDOW
