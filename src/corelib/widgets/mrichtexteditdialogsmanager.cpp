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

#include <QFontDatabase>
#include <QStringListModel>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <MApplication>
#include <MApplicationWindow>
#include <MBasicListItem>
#include <MComboBox>
#include <MDialog>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSceneManager>
#include "mrichtexteditdialogsmanager_p.h"

MRichTextEditDialogsManager *MRichTextEditDialogsManager::dialogsManager = 0;

MRichTextEditFontStyleWidget::MRichTextEditFontStyleWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
}


MRichTextEditFontStyleWidget::~MRichTextEditFontStyleWidget()
{
}

void MRichTextEditFontStyleWidget::updateSize(int numRows, int numColumns)
{
    QSizeF boundingRect = maximumSize();
    // TODO: Yet to get the layout guide and need to align accordingly
    static const int translateX = 10;
    static const int translateY = 10;
    static const int rectWidth = 70;
    static const int rectHeight = 50;

    qreal width = numColumns * rectWidth;
    qreal height = numRows * rectHeight;

    // Using 75% width and height of the parent (MDialog)
    qreal boundingRectWidth = 0.75 * boundingRect.width();
    qreal boundingRectHeight = 0.75 * boundingRect.height();

    qreal horizontalGapSize = (boundingRectWidth - width - translateX) / (numColumns + 1);
    qreal verticalGapSize = (boundingRectHeight - height - translateY) / (numRows + 1);

    items.clear();
    int size = numRows * numColumns;
    for (int index = 0; index < size; index++) {
        QRectF rect;
        rect.setX(translateX + ((index % numColumns) * horizontalGapSize) +
                  ((index % numColumns) * rectWidth));
        rect.setY(translateY + ((index / numColumns) * verticalGapSize) +
                  ((index / numColumns) * rectHeight));
        rect.setWidth(rectWidth);
        rect.setHeight(rectHeight);

        items << rect;
    }
}


void MRichTextEditFontStyleWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF curPos = event->pos();
    event->accept();

    int size = items.size();
    for (int index = 0; index < size; index++) {
        if (items[index].contains(curPos)) {
            selectItem(index);
            update();
            break;
        }
    }
}


void MRichTextEditFontStyleWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}


MRichTextEditFontSizeWidget::MRichTextEditFontSizeWidget(const QList<int> &fontSizeValues,
                                                         QGraphicsItem *parent)
    : MRichTextEditFontStyleWidget(parent),
      sizeValues(fontSizeValues),
      activeIndex(-1)
{
}


MRichTextEditFontSizeWidget::~MRichTextEditFontSizeWidget()
{
}

void MRichTextEditFontSizeWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush brush(Qt::gray, Qt::SolidPattern);
    painter->setBrush(brush);

    int size = items.size();
    for (int index = 0; index < size; index++) {
        if (activeIndex == index) {
            painter->drawRect(items[index]);
        } else {
            painter->fillRect(items[index], brush);
        }
        // Draw the font size value as text
        painter->drawText(items[index], Qt::AlignCenter, QString::number(sizeValues[index]));
    }
}


void MRichTextEditFontSizeWidget::selectItem(int index)
{
    if (index >= 0 && index < sizeValues.size()) {
        activeIndex = index;
        emit fontSizeSelected(sizeValues[index]);
    }
}


void MRichTextEditFontSizeWidget::updateOrientation(M::Orientation orientation)
{
    Q_UNUSED(orientation);

    // Update the size of the items in the ui when the orientation changes
    updateSize(sizeValues.size() / 4, 4);
}


void MRichTextEditFontSizeWidget::setActiveSize(int size)
{
    activeIndex = sizeValues.indexOf(size);
}


MRichTextEditDialogsManager::MRichTextEditDialogsManager()
    : fontFamilyCombo(0),
      fontSizeListItem(0),
      fontSizeWidget(0)
{
}


MRichTextEditDialogsManager::~MRichTextEditDialogsManager()
{
    delete dialogs.textStyles.first;
    delete dialogs.fontSize.first;
}


MRichTextEditDialogsManager *MRichTextEditDialogsManager::instance()
{
    if (!dialogsManager) {
        dialogsManager = new MRichTextEditDialogsManager();
    }

    return dialogsManager;
}


void MRichTextEditDialogsManager::initTextStylingDialog()
{
    if (dialogs.textStyles.first) {
        return;
    }

    dialogs.textStyles.first = new MDialog();
    //% "Text styles"
    dialogs.textStyles.first->setTitle(qtTrId("qtn_comm_text_styles"));
    dialogs.textStyles.second = false;

    QGraphicsWidget *centralWidget = dialogs.textStyles.first->centralWidget();
    MLayout *layout = new MLayout(centralWidget);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    QFontDatabase fontDatabase;
    QStringList fontFamilyValues = fontDatabase.families();
    fontFamilyCombo = new MComboBox(centralWidget);
    //% "Font"
    fontFamilyCombo->setTitle(qtTrId("qtn_comm_font"));
    QStringListModel *fontModel = new QStringListModel(centralWidget);
    fontModel->setStringList(fontFamilyValues);
    fontFamilyCombo->setItemModel(fontModel);
    policy->addItem(fontFamilyCombo);

    connect(fontFamilyCombo, SIGNAL(activated(QString)),
            this, SIGNAL(fontFamilySelected(QString)));

    fontSizeListItem = new MBasicListItem(MBasicListItem::TitleWithSubtitle, centralWidget);
    //% "Font size"
    fontSizeData.titleName = qtTrId("qtn_comm_font_size");
    fontSizeListItem->setTitle(fontSizeData.titleName);

    fontSizeData.sizeValues << 8 << 9 << 10 << 11
                            << 12 << 14 << 16 << 18
                            << 20 << 22 << 24 << 26
                            << 28 << 32 << 48 << 72;

    fontSizeListItem->setSubtitle(QString::number(fontSizeData.sizeValues[0]));

    connect(fontSizeListItem, SIGNAL(clicked()),
            this, SLOT(showFontSizeDialog()));
    policy->addItem(fontSizeListItem);
}


void MRichTextEditDialogsManager::initFontSizeDialog()
{
    if (dialogs.fontSize.first) {
        return;
    }

    dialogs.fontSize.first = new MDialog();
    dialogs.fontSize.first->setTitle(fontSizeData.titleName);
    dialogs.fontSize.second = false;

    QGraphicsWidget *centralWidget = dialogs.fontSize.first->centralWidget();
    MLayout *layout = new MLayout(centralWidget);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    fontSizeWidget = new MRichTextEditFontSizeWidget(fontSizeData.sizeValues, centralWidget);
    policy->addItem(fontSizeWidget);

    connect(fontSizeWidget, SIGNAL(fontSizeSelected(int)),
            SLOT(setFontSize(int)));
}


void MRichTextEditDialogsManager::showFontSizeDialog()
{
    initFontSizeDialog();

    Q_ASSERT(MApplication::activeApplicationWindow());

    const MSceneManager *sceneManager = MApplication::activeApplicationWindow()->sceneManager();

    // Orientation could be changing when the font size widget is not active, so
    // update the orientation change before showing the widget
    updateFontSizeWidgetOrientation(sceneManager->orientation());

    Q_ASSERT(fontSizeWidget);

    int activeSizeIndex = fontSizeData.activeSizeIndex;
    if (activeSizeIndex >= 0 && activeSizeIndex < fontSizeData.sizeValues.size()) {
        fontSizeWidget->setActiveSize(fontSizeData.sizeValues[activeSizeIndex]);
    } else {
        fontSizeWidget->setActiveSize(-1);
    }

    // For getting the orientation change when the font size widget is active
    connect(sceneManager, SIGNAL(orientationChanged(M::Orientation)),
            SLOT(updateFontSizeWidgetOrientation(M::Orientation)));

    execDialog(&dialogs.fontSize);

    disconnect(sceneManager, SIGNAL(orientationChanged(M::Orientation)),
               this, SLOT(updateFontSizeWidgetOrientation(M::Orientation)));
}


void MRichTextEditDialogsManager::updateFontSizeWidgetOrientation(M::Orientation orientation)
{
    Q_ASSERT(fontSizeWidget);
    if (dialogs.fontSize.first) {
        // Update the font widget size when the orientation changes
        fontSizeWidget->setMinimumSize(dialogs.fontSize.first->maximumSize());
        fontSizeWidget->setMaximumSize(dialogs.fontSize.first->maximumSize());
        fontSizeWidget->updateOrientation(orientation);
    }
}


void MRichTextEditDialogsManager::setFontSize(int fontSize)
{
    Q_ASSERT(fontSizeListItem);

    fontSizeData.activeSizeIndex = fontSizeData.sizeValues.indexOf(fontSize);

    if (fontSizeData.activeSizeIndex >= 0 &&
        fontSizeData.activeSizeIndex < fontSizeData.sizeValues.size()) {
        int fontSize = fontSizeData.sizeValues[fontSizeData.activeSizeIndex];
        fontSizeListItem->setSubtitle(QString::number(fontSize));
    } else {
        fontSizeListItem->setSubtitle(QString());
    }

    if (dialogs.fontSize.first) {
        dialogs.fontSize.first->done(MDialog::Accepted);
    }

    emit fontSizeSelected(fontSize);
}


void MRichTextEditDialogsManager::setTextStyleValues(const QString &fontfamily, int fontPointSize)
{
    Q_ASSERT(fontFamilyCombo);

    int familyIndex = -1;
    QStringListModel *fontModel = dynamic_cast<QStringListModel *>(fontFamilyCombo->itemModel());
    if (fontModel) {
        QStringList fontFamilies = fontModel->stringList();
        familyIndex = fontFamilies.indexOf(fontfamily);
    }

    fontFamilyCombo->setCurrentIndex(familyIndex);

    Q_ASSERT(fontSizeListItem);

    fontSizeData.activeSizeIndex = fontSizeData.sizeValues.indexOf(fontPointSize);
    if (fontSizeData.activeSizeIndex >= 0 &&
        fontSizeData.activeSizeIndex < fontSizeData.sizeValues.size()) {
        int fontSize = fontSizeData.sizeValues[fontSizeData.activeSizeIndex];
        fontSizeListItem->setSubtitle(QString::number(fontSize));
    } else {
        fontSizeListItem->setSubtitle(QString());
    }
}


void MRichTextEditDialogsManager::execDialog(ActiveDialog *activeDialog)
{
    if (activeDialog &&
        !activeDialog->second &&
        activeDialog->first) {
        activeDialog->second = true;
        activeDialog->first->exec();
        activeDialog->second = false;
    }
}

void MRichTextEditDialogsManager::showTextStylingDialog(const QString &fontfamily, int fontPointSize)
{
    initTextStylingDialog();
    setTextStyleValues(fontfamily, fontPointSize);
    execDialog(&dialogs.textStyles);
}

