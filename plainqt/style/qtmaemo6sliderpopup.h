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

#ifndef QTMAEMO6SLIDERPOPUP_H
#define QTMAEMO6SLIDERPOPUP_H

#include <QLabel>

class QSlider;

/*!
 * This class adds a popup to a QSlider, that shows the value of the slider while
 * the slider handle is pressed or moved.
 * The popup is added to toplevel widget the slider belongs to. It's positioned
 * within this top level widget freely depending on the slider's position.
 */
class QtMaemo6SliderPopUp : public QLabel {
    Q_OBJECT
public:
    QtMaemo6SliderPopUp();

    /*!
     * \brief append the popup to a QSlider
     * \param slider the QSlider the popup should be shown on
     */
    void enableOn(QSlider* slider);
public Q_SLOTS:
    /*!
     * \brief updates the popups position if the slider is moved
     */
    void updatePosition();
protected:
    /*! \reimp */
    virtual void showEvent(QShowEvent *);
    virtual void paintEvent(QPaintEvent *);
    /*! \reimp_end */

    /*!
     * \brief returns the size of the popup including the borders
     */
    QSize styledSizeHint();
protected:
    QSlider* m_slider;
};

#endif //QTMAEMO6SLIDERPOPUP_H
