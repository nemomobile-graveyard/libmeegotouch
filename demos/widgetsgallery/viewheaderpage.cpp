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

#include "viewheaderpage.h"

#include <MApplication>
#include <MPannableViewport>
#include <MList>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MBasicListItem>
#include <MAction>
#include <MAbstractCellCreator>

#include <QGraphicsLinearLayout>
#include <QStringListModel>

namespace {
    class BasicListItemCreator : public MAbstractCellCreator<MBasicListItem>
    {
    public:
        MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
        {
            MWidget *cell = recycler.take(MBasicListItem::staticMetaObject.className());
            if (!cell) {
                cell = new MBasicListItem;
            }
            updateCell(index, cell);
            return cell;
        }

        void updateCell(const QModelIndex &index, MWidget *cell) const
        {
            MBasicListItem *item = qobject_cast<MBasicListItem*>(cell);
            Q_ASSERT(item);
            item->setTitle(index.data().toString());
        }
    };

    class ViewHeader : public MWidgetController
    {
    public:
        ViewHeader(QGraphicsItem *parent = 0) :
            MWidgetController(parent),
            linearLayout(0),
            titleWidget(0)
        {
            setStyleName(inv("CommonHeaderPanel"));
            setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

            linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
            titleWidget = new MLabel(this);
            titleWidget->setTextElide(true);
            titleWidget->setStyleName("CommonHeader");

            linearLayout->addItem(titleWidget);
        }

        void setTitle(const QString &title)
        {
            titleWidget->setText(title);
        }

    private:
        QString inv(QString stylename)
        {
            if (MApplication::instance()->objectName() == "widgetsgallery") {
                return stylename;
            } else {
                return stylename.append("Inverted");
            }
        }

    private:
        QGraphicsLinearLayout *linearLayout;
        MLabel *titleWidget;
    };
}

ViewHeaderPage::ViewHeaderPage()
    : TemplatePage(TemplatePage::ApplicationView),
      actionNormal(0),
      actionFixed(0),
      normalPolicy(0),
      fixedPolicy(0)
{
}

ViewHeaderPage::~ViewHeaderPage()
{
}

QString ViewHeaderPage::timedemoTitle()
{
    return "ViewHeader";
}

void ViewHeaderPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);
    pannableViewport()->setVerticalPanningPolicy(MPannableViewport::PanningAsNeeded);
    setStyleName(inv("CommonApplicationPage"));

    MLayout *layout = new MLayout(centralWidget());
    layout->setContentsMargins(0,0,0,0);

    normalPolicy = createNormalPolicy(layout);
    fixedPolicy = createFixedPolicy(layout);

    normalPolicy->activate();

    /* Set up the toolbar */
    actionNormal = new MAction(this);
    actionNormal->setObjectName("normalAction");
    actionNormal->setLocation(MAction::ToolBarLocation);
    actionNormal->setEnabled(false);
    addAction(actionNormal);
    connect(actionNormal, SIGNAL(triggered()), this, SLOT(normal()));

    actionFixed = new MAction(this);
    actionFixed->setObjectName("fixedAction");
    actionFixed->setLocation(MAction::ToolBarLocation);
    actionFixed->setEnabled(true);
    addAction(actionFixed);
    connect(actionFixed, SIGNAL(triggered()), this, SLOT(fixed()));

    retranslateUi();
}

MLinearLayoutPolicy *ViewHeaderPage::createNormalPolicy(MLayout *layout)
{
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    MList *list = new MList;
    list->setCellCreator(new BasicListItemCreator);
    list->setItemModel(createListModel());

    ViewHeader *header = new ViewHeader;
    //% "View Header"
    header->setTitle(qtTrId("xx_viewheader_page_view_header"));

    policy->addItem(header);
    policy->addItem(list);

    return policy;
}

MLinearLayoutPolicy *ViewHeaderPage::createFixedPolicy(MLayout *layout)
{
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0,0,0,0);
    policy->setSpacing(0);

    MPannableViewport *viewport = new MPannableViewport;
    MList *list = new MList(viewport);
    list->setCellCreator(new BasicListItemCreator);
    list->setItemModel(createListModel());
    viewport->setWidget(list);

    ViewHeader *header = new ViewHeader;
    //% "Fixed View Header"
    header->setTitle(qtTrId("xx_viewheader_page_fixed_view_header"));

    policy->addItem(header);
    policy->addItem(viewport);

    return policy;
}

QStringListModel *ViewHeaderPage::createListModel()
{
    QStringList items;
    for (int i = 0; i < 30; i++)
        items << QString("Item #%1").arg(QString::number(i));

    return new QStringListModel(items);
}

void ViewHeaderPage::normal()
{
    normalPolicy->activate();
    actionNormal->setEnabled(false);
    actionFixed->setEnabled(true);
}

void ViewHeaderPage::fixed()
{
    fixedPolicy->activate();
    actionNormal->setEnabled(true);
    actionFixed->setEnabled(false);
}

void ViewHeaderPage::retranslateUi()
{
    //% "View Header"
    setTitle(qtTrId("xx_viewheader_page_title"));
    if (!isContentCreated())
        return;

    //% "Normal"
    actionNormal->setText(qtTrId("xx_view_header_page_normal"));

    //% "Fixed"
    actionFixed->setText(qtTrId("xx_view_header_page_fixed"));
}
