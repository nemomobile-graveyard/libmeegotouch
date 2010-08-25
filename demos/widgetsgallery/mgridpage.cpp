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

#include <QDir>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QGraphicsSceneContextMenuEvent>
#include <QPointer>

#include <MApplication>
#include <MApplicationWindow>
#include <MAbstractCellCreator>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <MLayout>
#include <MSceneManager>
#include <MOrientationChangeEvent>
#include <MAction>
#include <MSlider>
#include <MDialog>
#include <MPannableViewport>
#include <MGridLayoutPolicy>

#include "utils.h"
#include "mgridpage.h"
#include "gridmodel.h"

#include "gridimagewidget.h"
#include "gridvideowidget.h"
#include "itemdetailpage.h"

class ContentItemCreator : public MAbstractCellCreator<MWidgetController>
{
public:
    ContentItemCreator(MGridPage* gridPage)
        : MAbstractCellCreator<MWidgetController>(),
          m_gridPage(0),
          m_preferredSize()
    {
        m_gridPage = gridPage;
    }

    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
    {
        MWidget *cell;
        const QVariant data = index.data(Qt::DecorationRole);

        MediaType m;
        if( data.canConvert<MediaType>() )
           m = data.value<MediaType>();

        if( m.type == MediaType::Video )
        {
#ifdef HAVE_GSTREAMER
            cell = MListCellCreatorHelper<GridVideoWidget>::createCell(recycler, "", "");

            GridVideoWidget *video = qobject_cast<GridVideoWidget*>(cell);
            video->open( m.path );
            video->setMuted(true);
            QObject::connect(video, SIGNAL(videoReady()), m_gridPage, SLOT(videoReady()));
            updateCell(index, cell);
#endif
        } else {
            cell = MListCellCreatorHelper<GridImageWidget>::createCell(recycler, "", "");
            updateCell(index, cell);
        }
        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const;
    QSizeF cellSize() const;

private:
    MGridPage* m_gridPage;
    QSizeF m_preferredSize;
};

void ContentItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if(!cell)
        return;

    const QVariant data = index.data(Qt::DecorationRole);
    MediaType m;

    if( data.canConvert<MediaType>() )
        m = data.value<MediaType>();

#ifdef HAVE_GSTREAMER
    if( m.type == MediaType::Video ) {
        GridVideoWidget *video = qobject_cast<GridVideoWidget*>(cell);

        QFileInfo info(m.path);
        video->setId(info.absolutePath() + QDir::separator() + info.fileName()/*.remove("thumb-")*/);

        QObject::connect(video, SIGNAL(clicked()), m_gridPage, SLOT(itemClicked()));

        return;
    }
#endif

    if( m.type == MediaType::Image ) {
        GridImageWidget *imageWidget = qobject_cast<GridImageWidget*>(cell);

        imageWidget->setObjectName("gridItem");

        imageWidget->setImage(m.image);
        imageWidget->setId(m.path);

        QObject::connect(imageWidget, SIGNAL(clicked()), m_gridPage, SLOT(itemClicked()));
        return;
    }
}

QSizeF ContentItemCreator::cellSize() const
{
    return m_gridPage->itemSize();
}

MGridPage::MGridPage()
    : TemplatePage(TemplatePage::ListsGridsAndPopups),
      list(0),
      actionConfiguration(0),
      pageShown(false),
      m_itemSize(10,10),
      m_columnsPortrait(2),
      m_columnsLandscape(4),
      m_columnsLandscapeSlider(0),
      m_columnsPortraitSlider(0),
      m_columnsLandscapeLabel(0),
      m_columnsPortraitLabel(0)
{
    setObjectName("gridPage");
}

MGridPage::~MGridPage()
{
}

void MGridPage::createContent()
{
    MApplicationPage::createContent();

    MTheme::addPixmapDirectory(QDir(CONTACTS_DIR).canonicalPath());
    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0,0,0,0);
    panel->setLayout(layout);

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    list = new MList(panel);
    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    /* get initial page width from the application window */
    int initialWidth = MApplication::activeWindow()->visibleSceneSize().width();

    if (MApplication::activeApplicationWindow()->orientation() == M::Landscape) {
        list->setColumns(m_columnsLandscape);
        m_itemSize.setWidth(initialWidth / m_columnsLandscape);
    } else {
        list->setColumns(m_columnsPortrait);
        m_itemSize.setWidth(initialWidth / m_columnsPortrait);
    }
    m_itemSize.setHeight(m_itemSize.width());

    ContentItemCreator *cellCreator = new ContentItemCreator(this);
    list->setCellCreator(cellCreator);

    GridModel *model = new GridModel(m_itemSize.toSize(), Utils::mediaArtDir());
    list->setItemModel(model);
    connect(this, SIGNAL(rate(MediaType::Rating,QString)), model, SLOT(rateImage(MediaType::Rating,QString)));

    //% "Configuration"
    actionConfiguration = new MAction(this);
    actionConfiguration->setLocation(MAction::ApplicationMenuLocation);
    connect(actionConfiguration, SIGNAL(triggered()), this, SLOT(showGridConfigurationDialog()));
    addAction(actionConfiguration);

    retranslateUi();
}

void MGridPage::retranslateUi()
{
    //% "Grid page"
    setTitle(qtTrId("xx_gridpage_title"));
    if (!isContentCreated())
        return;

    //% "Configuration"
    actionConfiguration->setText(qtTrId("xx_gridpage_configuration"));
}

QString MGridPage::timedemoTitle()
{
    return "Grid";
}

void MGridPage::videoReady()
{
#ifdef HAVE_GSTREAMER
    GridVideoWidget* video = qobject_cast<GridVideoWidget*>(sender());
    if( video )
        video->play();
#endif
}

void MGridPage::itemClicked()
{
    if( !pageShown ) {
        //image clicked
        GridImageWidget* image = qobject_cast<GridImageWidget*>(sender());
        if( image ) {
            pageShown = true;

            ItemDetailPage* page = new ItemDetailPage();
            page->setImageId(image->id());
            page->setParent(this);
            connect( page, SIGNAL(backButtonClicked()), this, SLOT(backButtonClicked()) );
            connect( page, SIGNAL(rate(MediaType::Rating,QString)), this, SIGNAL(rate(MediaType::Rating,QString)) );

            page->appear(DestroyWhenDismissed);

            return;
        }

#ifdef HAVE_GSTREAMER
        //video clicked
        GridVideoWidget* video = qobject_cast<GridVideoWidget*>(sender());
        if( video ) {
            pageShown = true;

            ItemDetailPage* page = new ItemDetailPage();
            QString videoID(video->id());
            videoID.remove("thumb-");
            page->setVideoId(videoID);
            page->setParent(this);
            connect( page, SIGNAL(backButtonClicked()), this, SLOT(backButtonClicked()) );

            page->appear(DestroyWhenDismissed);
        }
#endif
    }
}

void MGridPage::backButtonClicked()
{
    pageShown = false;
}

void MGridPage::orientationChangeEvent(MOrientationChangeEvent *event)
{
    MApplicationPage::orientationChangeEvent(event);
    configureGrid(event->orientation());
}

void MGridPage::configureGrid()
{
    configureGrid(sceneManager()->orientation());
}

void MGridPage::configureGrid(M::Orientation orientation)
{
    if (list) {
        if (orientation == M::Portrait) {
            m_itemSize.setHeight(list->geometry().width() / m_columnsPortrait);
            list->setColumns(m_columnsPortrait);
        } else {
            m_itemSize.setHeight(list->geometry().width() / m_columnsLandscape);
            list->setColumns(m_columnsLandscape);
        }
    }
    m_itemSize.setWidth(m_itemSize.height());

    updateGeometry();
    update();
}

void MGridPage::showGridConfigurationDialog()
{
    //% "Set columns"
    QPointer<MDialog> dialog = new MDialog(qtTrId("xx_gridpage_set_columns"), M::OkButton | M::CancelButton);

    MLayout *layout = new MLayout(dialog->centralWidget());
    MGridLayoutPolicy *landscapePolicy = new MGridLayoutPolicy(layout);
    layout->setLandscapePolicy(landscapePolicy);

    MLinearLayoutPolicy *potraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(potraitPolicy);

    dialog->centralWidget()->setLayout(layout);

    m_columnsLandscapeSlider = new MSlider;
    m_columnsLandscapeSlider->setRange(2,8);
    m_columnsLandscapeSlider->setValue(m_columnsLandscape);
    m_columnsLandscapeSlider->setHandleLabelVisible(true);
    m_columnsLandscapeSlider->setMinLabelVisible(true);
    m_columnsLandscapeSlider->setMaxLabelVisible(true);

    m_columnsPortraitSlider = new MSlider;
    m_columnsPortraitSlider->setRange(2,5);
    m_columnsPortraitSlider->setValue(m_columnsPortrait);
    m_columnsPortraitSlider->setHandleLabelVisible(true);
    m_columnsPortraitSlider->setMinLabelVisible(true);
    m_columnsPortraitSlider->setMaxLabelVisible(true);

    m_columnsLandscapeLabel = new MLabel(QString::number( m_columnsLandscapeSlider->value() ));
    m_columnsPortraitLabel = new MLabel(QString::number(m_columnsPortraitSlider->value()));

    connect( m_columnsLandscapeSlider,SIGNAL(valueChanged(int)), this,SLOT(modifyColumnsSliderHandle(int)) );
    connect( m_columnsPortraitSlider,SIGNAL(valueChanged(int)), this,SLOT(modifyRowsSliderHandle(int)) );

    //% "Landscape"
    landscapePolicy->addItem(new MLabel(qtTrId("xx_gridpage_landscape")), 0,0);
    landscapePolicy->addItem(m_columnsLandscapeLabel,  1,0);
    landscapePolicy->addItem(m_columnsLandscapeSlider, 2,0);

    //% "Portrait"
    landscapePolicy->addItem(new MLabel(qtTrId("xx_gridpage_portrait")), 0,1);
    landscapePolicy->addItem(m_columnsPortraitLabel,  1,1);
    landscapePolicy->addItem(m_columnsPortraitSlider, 2,1);

    //% "Landscape"
    potraitPolicy->addItem(new MLabel(qtTrId("xx_gridpage_landscape")));
    potraitPolicy->addItem(m_columnsLandscapeLabel);
    potraitPolicy->addItem(m_columnsLandscapeSlider);

    //% "Portrait"
    potraitPolicy->addItem(new MLabel(qtTrId("xx_gridpage_portrait")));
    potraitPolicy->addItem(m_columnsPortraitLabel);
    potraitPolicy->addItem(m_columnsPortraitSlider);

    if (dialog->exec() == MDialog::Accepted) {
            m_columnsLandscape = m_columnsLandscapeSlider->value();
            m_columnsPortrait = m_columnsPortraitSlider->value();
            configureGrid();
    }

    delete dialog;
}

void MGridPage::modifyColumnsSliderHandle(int newValue)
{
    m_columnsLandscapeSlider->setHandleLabel(QString::number(newValue));
    m_columnsLandscapeLabel->setText(QString::number(newValue));
}
void MGridPage::modifyRowsSliderHandle(int newValue)
{
    m_columnsPortraitSlider->setHandleLabel(QString::number(newValue));
    m_columnsPortraitLabel->setText(QString::number(newValue));
}
