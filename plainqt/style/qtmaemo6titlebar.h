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

#ifndef QTMAEMO6TITLEBAR_H
#define QTMAEMO6TITLEBAR_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>

#include <mnamespace.h>

class QtMaemo6ClickLabel;

/*!
 * this class emulates a m titlebar
 */
class QtMaemo6TitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
public:
    explicit QtMaemo6TitleBar(QWidget *parent);
    virtual ~QtMaemo6TitleBar();

    /*!
     * returns the title of titlebar
     */
    QString title() const;

    /*!
     * returns the title of titlebar
     */
    QColor titleColor() const;

    /*!
     * returns the margin
     */
    int margin() const;

    /*!
     * returns the spacing between the items
     */
    int itemSpacing() const;

    /*!
     * hides or shows the Menu Button
     */
    void setMenuButtonVisible(bool visible);
public Q_SLOTS:
    /*!
     * sets the title of the titlebar
     */
    void setTitle(const QString &title);

    /*!
     * sets the color of the title
     */
    void setTitleColor(const QColor &color);

    /*!
     * sets the margin
     */
    void setMargin(int spacing);

    /*!
     * sets the spacing between the items
     */
    void setItemSpacing(int spacing);

    /*!
     * adds a QAction to the titlebar
     * /see addButton()
     */
    void addAction(QAction *action);

    /*!
     * adds a button to the titlebar
     */
    void addButton(QToolButton *button);

    /*!
     * sets the orientation of the titlebar
     */
    void setOrientation(M::OrientationAngle);

    M::OrientationAngle orientation() const { return m_orientationAngle; };
Q_SIGNALS:
    /*!
     * this signal is emitted if the home button was pressed
     */
    void closeButtonClicked();

    /*!
     * this signal is emitted if the close button was pressed
     */
    void minimizeButtonClicked();

    /*!
     * this signal is emitted if the title label was pressed
     */
    void menuLabelClicked();

protected:
    /*! \reimp */
    void paintEvent(QPaintEvent *event);
    /*! \reimp_end */

private:
    static const int titleBarHeight;
    QtMaemo6ClickLabel *m_titleLabel;
    QtMaemo6ClickLabel *m_titleLabelMenuButton;
    QtMaemo6ClickLabel *m_minimizeButton;
    QtMaemo6ClickLabel *m_closeButton;
    QBoxLayout *m_buttonsLayout;
    QBoxLayout *m_titleBarLayout;
    M::OrientationAngle m_orientationAngle;
};

#endif
