/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mgridpage.h"

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
#include <MSheet>
#include <MBasicSheetHeader>

#include "utils.h"
#include "gridmodel.h"

#include "gridimagewidget.h"
#include "itemdetailpage.h"

class ContentItemCreator : public MAbstractCellCreator<MWidgetController>
{
public:
    ContentItemCreator(MGridPage* gridPage, MList *parent)
        : MAbstractCellCreator<MWidgetController>(),
          m_gridPage(gridPage),
          m_preferredSize(),
          parent(parent)
    {
    }

    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
    {
        Q_UNUSED(recycler);

        MWidget *cell = NULL;

        MediaType m = index.data(Qt::DisplayRole).value<MediaType>();

        if (m.type == MediaType::Image) {
            cell = new GridImageWidget(parent);
            updateCell(index, cell);
        }
        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const;
    QSizeF cellSize() const;

private:
    MGridPage* m_gridPage;
    QSizeF m_preferredSize;
    MList *parent;
};

void ContentItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!cell)
        return;

    MediaType m = index.data(Qt::DisplayRole).value<MediaType>();

    if (m.type == MediaType::Image) {
        GridImageWidget *imageWidget = qobject_cast<GridImageWidget*>(cell);

        imageWidget->setStyleName("gridItem");
        imageWidget->setImage(m.image);
        imageWidget->setId(m.path);
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
      model(0),
      actionConfiguration(0),
      m_itemSize(10,10),
      m_columnsPortrait(4),
      m_columnsLandscape(6),
      m_columnsLandscapeSlider(0),
      m_columnsPortraitSlider(0)
{
    setStyleName("gridPage");
}

MGridPage::~MGridPage()
{
}

void MGridPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

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
    list->setObjectName("list");
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

    ContentItemCreator *cellCreator = new ContentItemCreator(this, list);
    list->setCellCreator(cellCreator);

    QStringList mediaDirs;
    mediaDirs << Utils::picturesDir();
    mediaDirs << Utils::mediaArtDir();

    model = new GridModel(this, m_itemSize.toSize(), mediaDirs);
    model->setObjectName("gridModel");
    list->setItemModel(model);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    //% "Configuration"
    actionConfiguration = new MAction(this);
    actionConfiguration->setObjectName("actionConfiguration");
    actionConfiguration->setLocation(MAction::ApplicationMenuLocation);
    connect(actionConfiguration, SIGNAL(triggered()), this, SLOT(showGridConfigurationSheet()));
    addAction(actionConfiguration);

    connect(this, SIGNAL(disappearing()), this, SLOT(pauseLoaders()));
    connect(this, SIGNAL(appeared()), this, SLOT(resumeLoaders()));

    connect(list, SIGNAL(panningStarted()), this, SLOT(pauseLoaders()));
    connect(list, SIGNAL(panningStopped()), this, SLOT(resumeLoaders()));

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

void MGridPage::itemClicked(const QModelIndex &index)
{
    //image clicked
    MediaType::Type type = (MediaType::Type)index.data(GridModel::Type).toInt();

    if (type == MediaType::Image) {
        QString imageId = index.data(GridModel::ImageId).toString();

        ItemDetailPage* page = new ItemDetailPage();
        page->setObjectName("itemDetailPage");
        page->setImageId(imageId);
        page->appear(scene(), DestroyWhenDismissed);
    }
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

void MGridPage::configurationUpdated()
{
    m_columnsLandscape = m_columnsLandscapeSlider->value();
    m_columnsPortrait = m_columnsPortraitSlider->value();
    configureGrid();
    configurationSheet->dismiss();
}

void MGridPage::configurationCanceled()
{
    configurationSheet->dismiss();
}

void MGridPage::modifyColumnsSliderHandle(int newValue)
{
    m_columnsLandscapeSlider->setHandleLabel(QString::number(newValue));
}

void MGridPage::modifyRowsSliderHandle(int newValue)
{
    m_columnsPortraitSlider->setHandleLabel(QString::number(newValue));
}

void MGridPage::showGridConfigurationSheet()
{
    configurationSheet = new MSheet;
    configurationSheet->setObjectName("configurationSheet");
    MBasicSheetHeader *header = new MBasicSheetHeader;
    header->setObjectName("sheetHeader");

    header->setPositiveAction(new QAction("Done", header));
    connect(header->positiveAction(), SIGNAL(triggered()), SLOT(configurationUpdated()));

    header->setNegativeAction(new QAction("Cancel", header));
    connect(header->negativeAction(), SIGNAL(triggered()), SLOT(configurationCanceled()));

    configurationSheet->setHeaderWidget(header);


    MLayout *layout = new MLayout(configurationSheet->centralWidget());
    layout->setContentsMargins(0, 0, 0, 0);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPolicy(policy);

    configurationSheet->centralWidget()->setLayout(layout);

    MLabel *titleLabel = new MLabel(qtTrId("xx_gridpage_set_columns"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setStyleName("CommonTitle");
    policy->addItem(titleLabel);

    m_columnsLandscapeSlider = new MSlider;
    m_columnsLandscapeSlider->setObjectName("m_columnsLandscapeSlider");
    m_columnsLandscapeSlider->setStyleName("CommonSlider");
    m_columnsLandscapeSlider->setRange(2,8);
    m_columnsLandscapeSlider->setValue(m_columnsLandscape);
    m_columnsLandscapeSlider->setHandleLabelVisible(true);
    m_columnsLandscapeSlider->setMinLabelVisible(false);
    m_columnsLandscapeSlider->setMaxLabelVisible(false);
    m_columnsLandscapeSlider->setMinLabel("2");
    m_columnsLandscapeSlider->setMaxLabel("8");

    m_columnsPortraitSlider = new MSlider;
    m_columnsPortraitSlider->setObjectName("m_columnsPortraitSlider");
    m_columnsPortraitSlider->setStyleName("CommonSlider");
    m_columnsPortraitSlider->setRange(2,5);
    m_columnsPortraitSlider->setValue(m_columnsPortrait);
    m_columnsPortraitSlider->setHandleLabelVisible(true);
    m_columnsPortraitSlider->setMinLabelVisible(false);
    m_columnsPortraitSlider->setMaxLabelVisible(false);
    m_columnsPortraitSlider->setMinLabel("2");
    m_columnsPortraitSlider->setMaxLabel("5");

    connect( m_columnsLandscapeSlider,SIGNAL(valueChanged(int)), this,SLOT(modifyColumnsSliderHandle(int)) );
    connect( m_columnsPortraitSlider,SIGNAL(valueChanged(int)), this,SLOT(modifyRowsSliderHandle(int)) );

    //% "Landscape"
    MLabel *landscapeLabel = new MLabel(qtTrId("xx_gridpage_landscape"));
    landscapeLabel->setObjectName("landscapeLabel");
    landscapeLabel->setStyleName("CommonFieldLabel");

    //% "Portrait"
    MLabel *portraitLabel = new MLabel(qtTrId("xx_gridpage_portrait"));
    portraitLabel->setObjectName("portraitLabel");
    portraitLabel->setStyleName("CommonFieldLabel");

    policy->addItem(landscapeLabel);
    policy->addItem(m_columnsLandscapeSlider);

    policy->addItem(portraitLabel);
    policy->addItem(m_columnsPortraitSlider);

    policy->addStretch();

    configurationSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void MGridPage::pauseLoaders()
{
    model->pauseLoaders();
}

void MGridPage::resumeLoaders()
{
    if (MApplication::instance()->activeApplicationWindow()->currentPage() == this)
        model->resumeLoaders(list->firstVisibleItem().row());
}
