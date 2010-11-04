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

#ifndef PHONEBOOKCELLCONTENT_H
#define PHONEBOOKCELLCONTENT_H

#include <QGraphicsWidget>
#include <QImage>

#include <MLayout>
#include <MListItem>

class MListItem;
class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MImageWidget;
class MLabel;
class MProgressIndicator;

class PhoneBookCell : public MListItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    Q_PROPERTY(QImage image READ image WRITE setImage)

public:
    PhoneBookCell();
    virtual ~PhoneBookCell();

    QString title();
    void setTitle(const QString &title);

    QString subtitle();
    void setSubtitle(const QString &title);

    QImage image();
    void setImage(const QImage &iconImage);

private:
    MLayout *createLayout();

    MLabel *landscapeTitleLabelWidget();
    MLabel *portraitTitleLabelWidget();
    MLabel *subtitleLabelWidget();
    MImageWidget *imageWidget();

    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    MLayout *layout;
    MGridLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;

    MLabel *landscapeTitleLabel;
    MLabel *portraitTitleLabel;
    MLabel *subtitleLabel;
    MImageWidget *icon;
};

#endif // PHONEBOOKCONTENTITEM_H
