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

#include "qtmaemo6sliderpopup.h"
#include "qtmaemo6style_p.h"
#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QSizePolicy>
#include <QPainter>
#include <QStyleOption>
#include <QPaintEvent>

#include <MScalableImage>

#include <mlabelstyle.h>

QtMaemo6SliderPopUp::QtMaemo6SliderPopUp()
    : m_slider(0)
{
    setObjectName("Qt_Maemo6_SliderPopUp");
}

void QtMaemo6SliderPopUp::enableOn(QSlider* slider) {

    QWidget* topLevelWidget = slider;

    while(topLevelWidget->parentWidget())
        topLevelWidget = topLevelWidget->parentWidget();

    setParent(topLevelWidget);
    setVisible(false);
    setAlignment(Qt::AlignCenter);

    disconnect();
    m_slider = slider;
    connect(slider, SIGNAL(sliderPressed()), this, SLOT(show()));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(hide()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(updatePosition()));
}

void QtMaemo6SliderPopUp::showEvent(QShowEvent *e) {
    updatePosition();
    QLabel::showEvent(e);
}

void QtMaemo6SliderPopUp::resizeEvent(QResizeEvent *e) {
    QLabel::resizeEvent(e);
    if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(style())) {
        if(s->setPaletteBackground(this, "MSliderStyle", "MSliderHandleLabel"))
            qCritical() << "SliderPopUp Resize:" << this;
    }

}

void QtMaemo6SliderPopUp::paintEvent(QPaintEvent *e) {
    QLabel::paintEvent(e);
}

void QtMaemo6SliderPopUp::updatePosition() {
    if(m_slider) {
        //have to use the style option to determine groove length, because
        // groove length may differ from widget size
        QStyleOptionSlider opt;
        opt.initFrom(m_slider);

        int maxSliderLength = style()->pixelMetric(QStyle::PM_SliderLength, &opt, m_slider);
        int span = (m_slider->orientation() == Qt::Horizontal)
                   ? opt.rect.width() - maxSliderLength
                   : opt.rect.height() - maxSliderLength;

        int sliderPos = QStyle::sliderPositionFromValue(m_slider->minimum(), m_slider->maximum(), m_slider->value(), span);
        setText(QString("%1").arg(m_slider->value()));
        resize(styledSizeHint());

        QPoint global;
        if(qApp->isLeftToRight())
            global = m_slider->mapToGlobal(m_slider->rect().topLeft());
        else
            global = m_slider->mapToGlobal(m_slider->rect().topRight());
        QPoint offset = parentWidget()->mapFromGlobal(global);

        //add the difference between widget width and groove width and the label
        // size to the offset and move the label
        if(m_slider->orientation() == Qt::Horizontal) {
            if(qApp->isLeftToRight()) {
                offset.setX(offset.x() + ((m_slider->width() - span) / 2) - styledSizeHint().width() / 2);
                move(offset.x() + sliderPos, offset.y() - height());
            } else {
                offset.setX(offset.x() - ((m_slider->width() - span) / 2) - styledSizeHint().width() / 2);
                move(offset.x() - sliderPos, offset.y() - height());
            }
        } else {
            offset.setY(offset.y() + ((m_slider->height() - span) / 2)  - styledSizeHint().height() / 2);
            move(offset.x() + m_slider->width(), offset.y() + span - sliderPos);
        }
    }
}

QSize QtMaemo6SliderPopUp::styledSizeHint() {
    QStyleOption opt;
    opt.initFrom(this);

    const MLabelStyle *style =
        static_cast<const MLabelStyle *>(QtMaemo6StylePrivate::mStyle(opt.state,
                                            "MSliderStyle",
                                            "MSliderHandleLabel"));
    //int left, top, right, bottom;
    //style->backgroundImage()->borders(&left, &right, &top, &bottom);
    //return sizeHint() + QSize(left + right, top + bottom);
    return sizeHint() + QSize(style->paddingLeft() + style->paddingRight(),
                              style->paddingTop() + style->paddingBottom());
}
