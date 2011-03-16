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

#include "screenshotpage.h"
#include <QGraphicsGridLayout>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QGraphicsLayout>
#include <MButton>
#include <MDebug>
#include <QDir>
#include <QTimer>
#include <QPropertyAnimation>
#include <MBanner>

const QString ImagesPath(QDir::homePath() + "/MyDocs/.images");

ScreenShotPage::ScreenShotPage()
    : TemplatePage(TemplatePage::ApplicationView),
    buttonScreenshot(0),
    label1(0),
    labelDescription(0),
    entrySeconds(0)
{
}

ScreenShotPage::~ScreenShotPage()
{
}

void ScreenShotPage::createContent()
{
    TemplatePage::createContent();

    QGraphicsWidget *panel = centralWidget();
    panel->setContentsMargins(0,0,0,0);

    setContentsMargins(0,0,0,0);

    layout = new MLayout(panel);
    layout->setContentsMargins(0,0,0,0);

    layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setSpacing(0);
    layoutPolicy->setContentsMargins(0,0,0,0);

    layout->setPortraitPolicy(layoutPolicy);
    layout->setLandscapePolicy(layoutPolicy);

    MLayout * elementsLayout = new MLayout;

    MLinearLayoutPolicy* elementsPolicyL = new MLinearLayoutPolicy(elementsLayout,Qt::Vertical);
    elementsLayout->setLandscapePolicy(elementsPolicyL);
    elementsPolicyL->setSpacing(0);

    MLinearLayoutPolicy* elementsPolicyP = new MLinearLayoutPolicy(elementsLayout,Qt::Vertical);
    elementsLayout->setLandscapePolicy(elementsPolicyP);
    elementsPolicyP->setSpacing(0);

    label1 = new MLabel ("Take a screenshot");
    label1->setStyleName("CommonApplicationHeader");

    elementsPolicyL->addItem(label1);
    elementsPolicyP->addItem(label1);

    labelDescription = new MLabel ();
    labelDescription->setWordWrap(true);
    labelDescription->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    labelDescription->setStyleName("CommonBodyText");

    elementsPolicyL->addItem(labelDescription);
    elementsPolicyP->addItem(labelDescription);

    QGraphicsGridLayout * gridSecs = new QGraphicsGridLayout();

    entrySeconds = new MTextEdit(MTextEditModel::SingleLine,"");
    entrySeconds->setMaxLength(2);
    entrySeconds->setStyleName("CommonSingleInputField");
    entrySeconds->setContentType(M::NumberContentType);
    entrySeconds->setText("8");
    entrySeconds->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    MLabel * secsLabel = new MLabel();
    secsLabel->setText("seconds");
    secsLabel->setStyleName("CommonSingleTitle");

    gridSecs->addItem(entrySeconds, 0 , 0 , Qt::AlignVCenter);
    gridSecs->addItem(secsLabel, 0 , 1, Qt::AlignVCenter);

    elementsPolicyL->addItem(gridSecs);
    elementsPolicyP->addItem(gridSecs);

    buttonScreenshot = new MButton("Take it!");
    buttonScreenshot->setStyleName("CommonSingleButton");

    elementsPolicyL->addItem(buttonScreenshot);
    elementsPolicyP->addItem(buttonScreenshot);

    connect(buttonScreenshot, SIGNAL(clicked()), this, SLOT(screenshotDelay()));
    elementsPolicyL->addStretch();
    elementsPolicyP->addStretch();

    layoutPolicy->addItem(elementsLayout);

    retranslateUi();
}

QString ScreenShotPage::timedemoTitle()
{
    return "Take a screenshot";
}

void ScreenShotPage::screenshotDelay()
{
    QTimer::singleShot((entrySeconds->text().toInt() * 1000), this, SLOT(takeScreenshot()));
}

void ScreenShotPage::retranslateUi()
{
    /*% "You can set the number of seconds before to take the screenshot. Set the timer and press the button after that a screenshot will be taken.*/
    labelDescription->setText(qtTrId("xx_label_description_screenshot"));
}

class ScreenshotEffect : public QGraphicsWidget
{
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->fillRect(boundingRect(), Qt::white);
    }
};

void ScreenShotPage::playScreenshotEffect()
{

    ScreenshotEffect *flash = new ScreenshotEffect();
    flash->setGeometry(0, 0, scene()->views().at(0)->width(), scene()->views().at(0)->height());

    QPropertyAnimation *animation = new QPropertyAnimation(flash, "opacity", scene()->views().at(0));
    animation->setDuration(400);
    animation->setEndValue(0.0f);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    scene()->addItem(flash);

    QObject::connect(animation, SIGNAL(finished()), flash, SLOT(deleteLater()));
}

void ScreenShotPage::takeScreenshot()
{
    QPixmap screenshot;

    screenshot = QPixmap::grabWindow(scene()->views().at(0)->effectiveWinId());

    QString path;

    if (QDir(ImagesPath).exists())
        path = ImagesPath;
    else
        path = QDir::homePath();

    QString fileFormat(QString("%1/%2-%3.png").arg(path).arg(QDate::currentDate().toString("yyyyMMdd")).arg(QTime::currentTime().toString("hhmmss")));

    if (!screenshot.save(fileFormat))
        mWarning("MWindow") << "Could not save screenshot to" << path;

    playScreenshotEffect();

    MBanner * bannerPath = new MBanner();
    bannerPath->setStyleName("InformationBanner");
    bannerPath->setTitle("Screenshot has been saved in: " + fileFormat);
    bannerPath->appear(scene(), MSceneWindow::DestroyWhenDone);
}

