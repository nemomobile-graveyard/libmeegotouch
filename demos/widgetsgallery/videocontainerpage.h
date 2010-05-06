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

#ifndef VIDEOCONTAINERPAGE_H
#define VIDEOCONTAINERPAGE_H

#include <MContainer>
#include <MImageWidget>
#include <MVideoWidget>

#include "templatepage.h"

class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MDialog;
class MSlider;

//image widget which emits clicked signal
class MyImageWidget : public MImageWidget
{   
    Q_OBJECT
    
    public:
        MyImageWidget(QGraphicsItem *parent = 0);
        void setId(const QString& id);
        QString id();
Q_SIGNALS:
        void clicked();
        
    protected:
    
         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
         
    private:
    
        QString m_id;
};

//video widget which emits clicked signal
class MyVideoWidget : public MVideoWidget
{   
    Q_OBJECT
    
    public:
        MyVideoWidget(QGraphicsItem *parent = 0);
        void setId(const QString& id);
        QString id();
Q_SIGNALS:
        void clicked();
        void longPressed();
        
    protected:
    
         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
         virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
         
    private:
    
        QString m_id;
};

//container for image and video thumbnails
class MyImageVideoContainer : public MContainer
{
public:
    MyImageVideoContainer(QGraphicsItem *parent = 0);
    virtual ~MyImageVideoContainer();

    MGridLayoutPolicy  *landscapePolicy;
    MGridLayoutPolicy  *portraitPolicy;

    void addItem(QGraphicsLayoutItem *button);
    void pauseAll();
    void playAll();
};

//page for showing video or image in it's native size
class ItemDetailPage  : public TimedemoPage
{
    Q_OBJECT

public:

    ItemDetailPage();
    virtual ~ItemDetailPage();

    virtual QString timedemoTitle();

    void setVideoId(const QString& id) {videoId = id; imageId = "";}
    void setImageId(const QString& id) {imageId = id; videoId = "";};

    virtual void createContent();
public slots:

    void videoReady();
    
    void sliderPressed();
    void sliderReleased();
    void videoSliderValueChanged(int newValue);
    
    void buttonClicked();
    
    void updatePosition();

protected:
    virtual void retranslateUi();

private:
    MLayout* layout;
    MLinearLayoutPolicy* policy;

    MVideoWidget* video;
    MSlider* slider;
    MButton* button;
    MImageWidget* image;
    
    QString imageId;
    QString videoId;
};

class VideoContainerPage : public TemplatePage
{
    Q_OBJECT

public:
    VideoContainerPage();
    virtual ~VideoContainerPage();

    virtual QString timedemoTitle();

    virtual void createContent();

public slots:
    void itemClicked();
    void itemLongPressed();
    
    void videoReady();
    
    void visibilityChanged(bool visible);

protected:
    virtual void retranslateUi();

private:

    void openImageDialog(const QString& itemid);
    void openVideoDialog(const QString& itemid);
    
    void openVideoDetailPage(const QString& itemid);
    void openImageDetailPage(const QString& itemid);
private:

    MyImageVideoContainer* container;

    ItemDetailPage* detailPage;
};

#endif
