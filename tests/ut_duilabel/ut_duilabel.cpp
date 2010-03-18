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

#include "ut_duilabel.h"

#include <duiwidget.h>
#include <duiwidgetcontroller.h>
#include <duilabelview.h>
#include <duitheme.h>
#include <duiwindow.h>
#include <duiapplication.h>
#include <duilabelhighlighter.h>

#include <QSignalSpy>
#include <QTestEventList>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>

#define SAVE_IMAGE(fileName, image) \
    do{ \
        QFile f(fileName); f.open(QIODevice::WriteOnly); image.save(&f, "PNG"); f.close(); \
    } while(0)

// Subclass of DuiLabel in order to simulate clicks
TestDuiLabel::TestDuiLabel(): DuiLabel()
{
}

TestDuiLabel::~TestDuiLabel()
{
}

void TestDuiLabel::simulateClick(const QPointF &pos)
{
    QGraphicsSceneMouseEvent *pressevent = new QGraphicsSceneMouseEvent();
    QGraphicsSceneMouseEvent *releaseevent = new QGraphicsSceneMouseEvent();
    pressevent->setPos(pos);
    releaseevent->setPos(pos);
    mousePressEvent(pressevent);
    mouseReleaseEvent(releaseevent);
    delete pressevent;
    delete releaseevent;
}

QImage TestDuiLabel::getLabelImage()
{
    QPixmap pixmap(size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    DuiWidgetController::paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

DuiApplication *app;

void Ut_DuiLabel::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duilabel" };
    app = new DuiApplication(argc, app_name);

    DuiTheme::loadCSS(qApp->applicationDirPath() + "/ut_duilabel.css");
}

void Ut_DuiLabel::init()
{
    label = new TestDuiLabel;
    label->setGeometry(QRectF(0, 0, 400, 20));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_DuiLabel::cleanup()
{
    delete label;
}

void Ut_DuiLabel::cleanupTestCase()
{
    delete app;
}

void Ut_DuiLabel::testTestConstruction()
{
    DuiLabel *label = new DuiLabel();
    delete label;

    label = new DuiLabel("this is plain text!");
    delete label;

    label = new DuiLabel("this is <b>rich</b> text!");
    delete label;

    label = new DuiLabel();
    label->setText("this is <b>rich</b> text!");
    DuiLabelModel *model = new DuiLabelModel();
    model->setText("this is plain text!");
    label->setModel(model);
    delete label;
}


void Ut_DuiLabel::testTextAlignment_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is plain text!";
    QTest::newRow("rich") << "<b>this is rich text!</b>";
}
void Ut_DuiLabel::testTextAlignment()
{
    QVector<Qt::Alignment> alignments;
    alignments.push_back(Qt::AlignLeft | Qt::AlignTop);
    alignments.push_back(Qt::AlignRight | Qt::AlignTop);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignTop);
    alignments.push_back(Qt::AlignLeft | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignRight | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignVCenter);
    alignments.push_back(Qt::AlignLeft | Qt::AlignBottom);
    alignments.push_back(Qt::AlignRight | Qt::AlignBottom);
    alignments.push_back(Qt::AlignHCenter | Qt::AlignBottom);

    label->setMinimumSize(500, 500);
    label->setMaximumSize(500, 500);
    label->setPreferredSize(500, 500);
    label->setGeometry(QRectF(0, 0, 500, 500));

    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    QVector<QImage> images;

    for (int i = 0; i < alignments.count(); i++) {
        label->setAlignment(alignments[i]);
        QVERIFY(label->alignment() == alignments[i]);

        QImage image = captureImage(label);
        images.push_back(image);
        //image.save(QString("0x") + QString::number(alignments[i], 16), "PNG");
    }

    for (int i = 0; i < alignments.count(); i++) {
        for (int j = i + 1; j < alignments.count(); j++) {
            if (images[i] == images[j]) {
                qDebug() << "Images are similar with alignments:" <<
                         QString("0x") + QString::number(alignments[i], 16) <<
                         "and" <<
                         QString("0x") + QString::number(alignments[j], 16);
            }

            QVERIFY(images[i] != images[j]);
        }
    }
}

void Ut_DuiLabel::testLayoutDirection_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is plain text!";
    QTest::newRow("rich") << "<b>this is rich text!</b>";
}

void Ut_DuiLabel::testLayoutDirection()
{
    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setLayoutDirection(Qt::LeftToRight);
    QVERIFY(label->layoutDirection() == Qt::LeftToRight);
    QImage ltr = captureImage(label);

    label->setLayoutDirection(Qt::RightToLeft);
    QVERIFY(label->layoutDirection() == Qt::RightToLeft);
    QImage rtl = captureImage(label);

    //ltr.save("ltr", "PNG");
    //rtl.save("rtl", "PNG");

    QVERIFY(ltr != rtl);
}

void Ut_DuiLabel::testTextWordWrap_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("equal");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006." << true;
    QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>" << false;
}

void Ut_DuiLabel::testTextWordWrap()
{
    QFETCH(QString, text);
    QFETCH(bool, equal);

    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(true);
    QVERIFY(label->wordWrap() == true);
    QImage wrapped = captureImage(label);

    label->setWordWrap(false);
    QVERIFY(label->wordWrap() == false);
    QImage unwrapped = captureImage(label);

    //wrapped.save("wrapped", "PNG");
    //unwrapped.save("unwrapped", "PNG");

    //
    QVERIFY(((wrapped == unwrapped) == equal));
}

void Ut_DuiLabel::testTextElide_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.";
    // For some reason this doesn't work in CC -> solve why
    //QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>";
}

void Ut_DuiLabel::testTextElide()
{
    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(false);
    label->setTextElide(true);
    QVERIFY(label->textElide() == true);
    QImage elided = captureImage(label);

    label->setTextElide(false);
    QVERIFY(label->textElide() == false);
    QImage unelided = captureImage(label);

    label->setLayoutDirection(Qt::RightToLeft);
    QImage elidedRtl = captureImage(label);

    //elided.save("elided", "PNG");
    //unelided.save("unelided", "PNG");
    //elidedRtl.save("elidedrtl", "PNG");

    QVERIFY(elided != unelided);
    QVERIFY(elided != elidedRtl);
}

void Ut_DuiLabel::testHighlighting()
{
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    DuiCommonLabelHighlighter *h = new DuiCommonLabelHighlighter(QRegExp("Label"));
    label->addHighlighter(h);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    QSignalSpy spy(h, SIGNAL(clicked(QString)));
    //QSignalSpy spy(h, SIGNAL(longPress(QString)));
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spy.count(), 1);

    label->removeHighlighter(NULL);
    label->removeHighlighter(h);
    delete h;
}

void Ut_DuiLabel::testAnchor()
{
    label->setText("<a href=\"http://www.nokia.com\">www.nokia.com</a>");
    anchorHitTest(QPointF(15, 5), 1);
    anchorHitTest(QPointF(200, 0), 0);
}

void Ut_DuiLabel::anchorHitTest(const QPointF &point, int expectedHits)
{
    QSignalSpy spy(label, SIGNAL(linkActivated(QString)));
    label->simulateClick(point);
    QCOMPARE(spy.count(), expectedHits);
}



QImage Ut_DuiLabel::captureImage(DuiLabel *label)
{
    QPixmap pixmap(label->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    label->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

void Ut_DuiLabel::testClear_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_DuiLabel::testClear()
{
    QFETCH(QString, text);

    QImage emptyLabel = captureImage(label);
    label->setText(text);
    QImage xxxxLabel = captureImage(label);
    QVERIFY(emptyLabel != xxxxLabel);

    label->setText("");
    QImage clearedLabel = captureImage(label);

    QVERIFY(label->text().isEmpty());
    QVERIFY(xxxxLabel != clearedLabel);
}

void Ut_DuiLabel::testPlainTextColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_DuiLabel::testPlainTextColor()
{
    QFETCH(QString, text);

    TestDuiLabel *label = new TestDuiLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));
    QImage imageLabel = captureImage(label);

    TestDuiLabel *anotherLabel = new TestDuiLabel();
    anotherLabel->setText(text);
    anotherLabel->setGeometry(QRectF(0, 0, 400, 20));
    QImage anotherLabelImage = captureImage(anotherLabel);
    QVERIFY2(imageLabel == anotherLabelImage, "Images should be equal, until we set name for second label.");
    anotherLabel->setObjectName("greyLabel");
    anotherLabel->setText(text);
    anotherLabelImage = captureImage(anotherLabel);
    QVERIFY2(imageLabel != anotherLabelImage, "Images shoudn't be equal, because in css colors are different");
    delete label;
    delete anotherLabel;
}
void Ut_DuiLabel::testSizeHint_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_DuiLabel::testSizeHint()
{
    QFETCH(QString, text);

    TestDuiLabel *label = new TestDuiLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));

    label->setWordWrap(true);
    {
        QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize);
        QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize);
        QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize);
    }

    {
        qreal width = 100.0;
        QSizeF constraint(width, -1);
        QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize, constraint);
        QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize, constraint);
        QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize, constraint);

        QVERIFY(minSizeHint.width() <= width);
        QVERIFY(prefSizeHint.width() <= width);
    }

    {
        qreal width = 80.0;
        QSizeF constraint(width, -1);
        QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize, constraint);
        QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize, constraint);
        QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize, constraint);

        QVERIFY(minSizeHint.width() <= width);
        QVERIFY(prefSizeHint.width() <= width);
    }

    {
        qreal width = 50.0;
        QSizeF constraint(width, -1);
        QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize, constraint);
        QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize, constraint);
        QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize, constraint);

        QVERIFY(minSizeHint.width() <= width);
        QVERIFY(prefSizeHint.width() <= width);
    }

    {
        QSizeF hint = label->sizeHint((Qt::SizeHint)12345);
    }
}

void Ut_DuiLabel::testFont_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_DuiLabel::testFont()
{
    QFETCH(QString, text);

    DuiLabel *label = new TestDuiLabel();
    label->setText(text);

    // this will create the view and load CSS font
    label->preferredSize();

    QFont newFont("Times", 30, QFont::Bold);
    label->setFont(newFont);

    QVERIFY(label->font() == newFont);

    // make style reload
    label->setActive(true);
    label->setActive(false);

    QVERIFY(label->font() == newFont);
}

void Ut_DuiLabel::testColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_DuiLabel::testColor()
{
    QFETCH(QString, text);

    DuiLabel *label = new TestDuiLabel();
    label->setText(text);

    // this will create the view and load CSS font
    label->preferredSize();

    QColor color("#FF00FF");
    label->setColor(color);

    QVERIFY(label->color() == color);

    // make style reload
    label->setActive(true);
    label->setActive(false);

    QVERIFY(label->color() == color);
}

void Ut_DuiLabel::linefeedBeforeFirstTag()
{
    QString htmlData;
    QChar wierd(10);
    htmlData = "Hey,";
    htmlData.append(wierd); 
    htmlData.append(" <br />");
    htmlData.append(wierd);
    htmlData.append("<br /> I have some ");
    htmlData.append(wierd);
    htmlData.append("character in between <br /> B import A.");

    DuiLabel label;
    label.setText(htmlData);
    QSizeF prefSizeHint = label.sizeHint(Qt::PreferredSize);

    //This is true if label interprets <br> tags correctly 
    QVERIFY(prefSizeHint.height() > prefSizeHint.width());


}

QTEST_APPLESS_MAIN(Ut_DuiLabel);
