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

#ifndef VIDEOCONTAINERPAGE_H
#define VIDEOCONTAINERPAGE_H

#include <DuiContainer>
#include <DuiImageWidget>
#include <DuiVideoWidget>

#include "templatepage.h"

class DuiGridLayoutPolicy;
class DuiLinearLayoutPolicy;
class DuiDialog;
class DuiSlider;

//image widget which emits clicked signal
class MyImageWidget : public DuiImageWidget
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
class MyVideoWidget : public DuiVideoWidget
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
class MyImageVideoContainer : public DuiContainer
{
public:
    MyImageVideoContainer(QGraphicsItem *parent = 0);
    virtual ~MyImageVideoContainer();

    DuiGridLayoutPolicy  *landscapePolicy;
    DuiGridLayoutPolicy  *portraitPolicy;

    void addItem(QGraphicsLayoutItem *button);
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
    
    void updatePosition();

protected:
    virtual void retranslateUi();

private:
    DuiLayout* layout;
    DuiLinearLayoutPolicy* policy;

    DuiVideoWidget* video;
    DuiSlider* slider;
    DuiImageWidget* image;
    
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
