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

#ifndef QTMAEMO6TESTSTYLE_H
#define QTMAEMO6TESTSTYLE_H

#include <QPlastiqueStyle>

class QtMaemo6WindowDecoration;
class QtMaemo6TestStyle;
class QtMaemo6TestStylePrivate;

class QtMaemo6TestStyleEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit QtMaemo6TestStyleEventFilter(QtMaemo6TestStyle *parent);
    virtual ~QtMaemo6TestStyleEventFilter() {};
protected:
    bool eventFilter(QObject *obj, QEvent *event);
protected:
    QtMaemo6TestStyle *m_style;
};

/*!
 * this class is a basic QStyle. It is currently only used for testing purposes
 * and may be deprecated in future
 */
class QtMaemo6TestStyle : public QPlastiqueStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QtMaemo6TestStyle)
    Q_DISABLE_COPY(QtMaemo6TestStyle)

    friend class QtMaemo6TestStyleEventFilter;
    //FIXME: bad dependency
    friend class QtMaemo6StyleEventFilter;
public:
    QtMaemo6TestStyle();
    virtual ~QtMaemo6TestStyle();

    /*! \reimp */
    virtual void polish(QApplication *app);
    virtual void polish(QPalette &palette);
    virtual void polish(QWidget *widget);
    /*! \reimp_end */

    /*!
     * initializes the style
     */
    virtual void init();
protected:
    QtMaemo6TestStylePrivate *const d_ptr;
    QtMaemo6TestStyle(QtMaemo6TestStylePrivate &dd);

protected:

    QtMaemo6TestStyleEventFilter *m_windowEventFilter;
    QtMaemo6WindowDecoration *m_windowDecoration;

private:
    void setTexture(QPalette &palette,
                    QPalette::ColorRole role,
                    const QPixmap &pixmap);
};

#endif
