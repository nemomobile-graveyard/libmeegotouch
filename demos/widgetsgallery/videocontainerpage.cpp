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

#include "videocontainerpage.h"

#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MDialog>
#include <MLabel>
#include <MTextEdit>
#include <MAction>
#include <MSlider>
#include <MButton>

#include <QDir>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

#include "utils.h"
#include "mdebug.h"

MyImageWidget::MyImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent)
{
}

void MyImageWidget::setId(const QString& id)
{
    m_id = id;
}

QString MyImageWidget::id()
{
    return m_id;
}

void MyImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event);
    event->accept();
}

void MyImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}

MyVideoWidget::MyVideoWidget(QGraphicsItem *parent)
    : MVideoWidget(parent)
{
}

void MyVideoWidget::setId(const QString& id)
{
    m_id = id;
}

QString MyVideoWidget::id()
{
    return m_id;
}

void MyVideoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mousePressEvent(event);
    event->accept();
}

void MyVideoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mouseReleaseEvent(event);
    emit clicked();
}

void MyVideoWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    MVideoWidget::contextMenuEvent(event);
    //emit longPressed();
}

MyImageVideoContainer::MyImageVideoContainer(QGraphicsItem *parent)
    : MContainer(parent)
{
    MLayout *layout = new MLayout(centralWidget());

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);
    //To make sure that both columns have the same width, give them the same preferred width.
    landscapePolicy->setColumnPreferredWidth(0, 800);
    landscapePolicy->setColumnPreferredWidth(1, 800);
    landscapePolicy->setColumnPreferredWidth(2, 800);
    
    portraitPolicy = new MGridLayoutPolicy(layout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
}

MyImageVideoContainer::~MyImageVideoContainer()
{
}

void MyImageVideoContainer::addItem(QGraphicsLayoutItem* button)
{
    int count = landscapePolicy->count();

    int row = count / 3;
    int column = count % 3;

    landscapePolicy->addItem(button, row, column);
    portraitPolicy->addItem(button, row, column);
}

void MyImageVideoContainer::pauseAll()
{
    MLayout *layout = dynamic_cast<MLayout*>(centralWidget()->layout());
    if( layout ) {
        for( int i = 0; i < layout->count(); ++i ) {
            MyVideoWidget* video = dynamic_cast<MyVideoWidget*>(layout->itemAt(i));
            if( video ) 
                if( video->state() != MVideo::NotReady )
                    video->pause();
        }
    }
}

void MyImageVideoContainer::playAll()
{
    MLayout *layout = dynamic_cast<MLayout*>(centralWidget()->layout());
    if( layout ) {
        for( int i = 0; i < layout->count(); ++i ) {
            MyVideoWidget* video = dynamic_cast<MyVideoWidget*>(layout->itemAt(i));
            if( video )
                if( video->state() != MVideo::NotReady )
                    video->play();
        }
    }
}


ItemDetailPage::ItemDetailPage()
{
    video = NULL;
    slider = NULL;
    image = NULL;
}

ItemDetailPage::~ItemDetailPage()
{
    delete video;
    delete slider;
    delete image;
}

QString ItemDetailPage::timedemoTitle()
{
    return "ItemDetailPage";
}


void ItemDetailPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);

    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setSpacing(0.0);
    layout->setLandscapePolicy(policy);
    layout->setPortraitPolicy(policy);
    
    if( !videoId.isEmpty() ) {
        video = new MVideoWidget(panel);
        connect(video, SIGNAL(videoReady()), this, SLOT(videoReady()));
        video->setFullscreen(true);
        video->open(videoId);
        policy->addItem(video);

        QGraphicsLinearLayout* controlLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        controlLayout->setContentsMargins(0,0,0,0);
        button = new MButton(panel);
        //button->setViewType(MButton::iconType);
        button->setObjectName("video-player-button");
        button->setIconID("icon-m-common-pause");
        button->setIconVisible(true);
        button->setTextVisible(false);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        controlLayout->addItem(button);
        controlLayout->setAlignment(button, Qt::AlignCenter);
        
        slider = new MSlider(panel);
        slider->setObjectName("video-player-slider");
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(videoSliderValueChanged(int)));
        connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
        connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
        controlLayout->addItem(slider);
        
        policy->addItem(controlLayout);

        QFileInfo info(videoId);        
        setTitle(info.fileName());    
    }
    else if( !imageId.isEmpty() ) {
        image = new MImageWidget(panel);
        image->setImage(QImage(imageId));
        policy->addItem(image);
        
        QFileInfo info(imageId);        
        setTitle(info.fileName());
    }
    
    retranslateUi();
}

void ItemDetailPage::retranslateUi()
{
}

void ItemDetailPage::videoReady()
{
    video->play();
    video->setFullscreen(true);
    slider->setMinimum(0);
    slider->setMaximum(video->length());
    QTimer::singleShot(100, this, SLOT(updatePosition()));
    
    setAutoMarginsForComponentsEnabled(false);
    setComponentsDisplayMode(MApplicationPage::NavigationBar/*MApplicationPage::AllComponents*/, MApplicationPageModel::Hide);
    
}

void ItemDetailPage::sliderPressed()
{
    //if( video )
    //    video->pause();
}

void ItemDetailPage::sliderReleased()
{
    //if( video )
    //    video->play();
}

void ItemDetailPage::videoSliderValueChanged(int newValue)
{
    if( video ) {
        video->seek(newValue);
    }
}

void ItemDetailPage::buttonClicked()
{
    if( video->state() == MVideo::Playing ) {
        video->pause();
        button->setIconID("icon-m-common-play");

        //button->setText("PLAY");
    } else {
        video->play();
        button->setIconID("icon-m-common-pause");
        //button->setText("PAUSE");
    }
        
}


void ItemDetailPage::updatePosition()
{
    if( video ) {
        slider->setValue(video->position());
        QTimer::singleShot(100, this, SLOT(updatePosition()));
    }
}

VideoContainerPage::VideoContainerPage() :
    TemplatePage(), container(NULL), detailPage(NULL)
{
    gid = TemplatePage::ListsGridsAndMenus;
}

VideoContainerPage::~VideoContainerPage()
{

}

QString VideoContainerPage::timedemoTitle()
{
    return "VideoContainer";
}

void VideoContainerPage::createContent()
{
    TemplatePage::createContent();

    container = new MyImageVideoContainer(centralWidget());
    containerPolicy->addItem(container);

    QString contactsDir = "/usr/share/widgetsgallery/media/";//Utils::contactsDir();
    QDir imagesDir(contactsDir);
    QStringList imageContacts = imagesDir.entryList(QDir::Files);

    for( int i = 0; i < imageContacts.size(); ++i ) {
        QString filename = contactsDir + imageContacts[i];
        QFileInfo info(filename);
        if( info.suffix() == "png" ) {
            MyImageWidget *image = new MyImageWidget(container->centralWidget());
            connect(image, SIGNAL(clicked()), this, SLOT(itemClicked()));
            image->setImage(QImage(filename));
            image->setId(filename);
            
            MAction *action = new MAction("Open", image);
            action->setLocation(MAction::ObjectMenuLocation);
            image->addAction(action);

            image->setPreferredSize(92, 92);
            //image->setMinimumSize(92, 92);
            //image->setMaximumSize(92, 92);            
            container->addItem(image);
        } 
        else if( info.suffix() == "mp4" || info.suffix() == "mov" ) {
            if( info.fileName().startsWith("thumb-") ) {
                MyVideoWidget *video = new MyVideoWidget(container->centralWidget());
                MAction *action = new MAction("Open", video);
                action->setLocation(MAction::ObjectMenuLocation);
                video->addAction(action);
                
                //connect(video, SIGNAL(clicked()), this, SLOT(itemClicked()));
                //connect(video, SIGNAL(longPressed()), this, SLOT(itemLongPressed()));
                connect(video, SIGNAL(clicked()), this, SLOT(itemLongPressed()));
                //connect(video, SIGNAL(longPressed()), this, SLOT(itemLongPressed()));
                connect(video, SIGNAL(videoReady()), this, SLOT(videoReady()));
                video->open(filename);
                video->setId(info.absolutePath() + QDir::separator() + info.fileName().remove("thumb-"));
                video->setPreferredSize(92, 92);
                //video->setMinimumSize(92, 92);
                //video->setMaximumSize(92, 92);
                container->addItem(video);
            }
        }
    }   

    retranslateUi();
    
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChanged(bool)));
}

void VideoContainerPage::retranslateUi()
{
    //% "Video container"
    setTitle(qtTrId("xx_video_container_page_title"));
    if (!isContentCreated())
        return;
        
    //% "Tap video thumbnail to play/pause it. Tap and hold thumbnail to open video in it's native size."
    infoLabel->setText("<a></a>" + qtTrId("xx_video_container_page_info_label"));
 
    //% "Gallery"
    container->setTitle(qtTrId("xx_video_container_header_title"));
}

void VideoContainerPage::openImageDialog(const QString& itemid)
{
    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget->setLayout(layout);

    MImageWidget *widget = new MImageWidget(centralWidget);
    widget->setImage(QImage(itemid));
    layout->addItem(widget);

    QFileInfo info(itemid);
    MDialog* dialog = new MDialog(info.fileName(),
                           M::OkButton);
    dialog->setCentralWidget(centralWidget);
    dialog->exec();
}

void VideoContainerPage::openVideoDetailPage(const QString& itemid)
{
    ItemDetailPage* page = new ItemDetailPage();
    page->setVideoId(itemid);
    page->setParent(this);
    page->appear(DestroyWhenDismissed);    
}

void VideoContainerPage::openImageDetailPage(const QString& itemid)
{
    ItemDetailPage* page = new ItemDetailPage();
    page->setImageId(itemid);
    page->setParent(this);
    page->appear(DestroyWhenDismissed); 
}

void VideoContainerPage::itemClicked()
{
    //image clicked
    MyImageWidget* image = qobject_cast<MyImageWidget*>(sender());
    if( image ) {
        openImageDetailPage(image->id());
        return;
    }
    
    //video clicked
    MyVideoWidget* video = qobject_cast<MyVideoWidget*>(sender());
    if( video ) {
        if( video->state() == MVideo::Paused || video->state() == MVideo::Stopped )
            video->play();
        else if( video->state() == MVideo::Playing )
            video->pause();
    }
}

void VideoContainerPage::itemLongPressed()
{
    //video long pressed
    MyVideoWidget* video = qobject_cast<MyVideoWidget*>(sender());
    if( video ) {
        openVideoDetailPage(video->id());
    }
}

void VideoContainerPage::videoReady()
{
    MyVideoWidget* video = qobject_cast<MyVideoWidget*>(sender());
    if( video )
        video->play();
}

void VideoContainerPage::visibilityChanged(bool visible)
{   
    mDebug("VideoContainerPage::visibilityChanged()") << isContentCreated();
    if( isContentCreated() ) {
        if( visible ) {
            container->playAll();
        } else {
            container->pauseAll();
        }
    }
}



