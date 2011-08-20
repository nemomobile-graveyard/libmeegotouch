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

#include "ut_mlabel.h"

#include <mapplicationpage.h>
#include <mapplicationwindow.h>
#include <mwidget.h>
#include <mwidgetcontroller.h>
#include <mlabelview.h>
#include <mtheme.h>
#include <mwindow.h>
#include <mapplication.h>
#include <mlabelhighlighter.h>
#include <mlabelview.h>
#include <mpannableviewport.h>
#include <mlocale.h>

#include <QSignalSpy>
#include <QTestEventList>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGesture>
#include <QTextDocument>
#include <QTextLayout>

#include <QLabel>

#define SHOW_IMAGE(image) \
    do { \
        QLabel label; label.setPixmap(QPixmap::fromImage(image)); label.show(); QTest::qWait(1000); \
    } while(0)

#define SAVE_IMAGE(fileName, image) \
    do{ \
        QFile f(fileName); f.open(QIODevice::WriteOnly); image.save(&f, "PNG"); f.close(); \
    } while(0)

// Subclass of MLabel in order to simulate clicks
TestMLabel::TestMLabel(): MLabel()
{
}

TestMLabel::~TestMLabel()
{
}

void TestMLabel::simulateClick(const QPointF &pos)
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

QImage TestMLabel::getLabelImage()
{
    QPixmap pixmap(size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    MWidgetController::paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

MApplication *app;

void Ut_MLabel::initTestCase()
{
#ifndef __arm__
    QApplication::setGraphicsSystem("raster");
#endif

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlabel" };
    app = new MApplication(argc, app_name);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mlabel.css");
}

void Ut_MLabel::init()
{
    label = new TestMLabel;
    label->setGeometry(QRectF(0, 0, 400, 20));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_MLabel::cleanup()
{
    delete label;
}

void Ut_MLabel::cleanupTestCase()
{
    delete app;
}

void Ut_MLabel::testTestConstruction()
{
    MLabel *label = new MLabel();
    delete label;

    label = new MLabel("this is plain text!");
    delete label;

    label = new MLabel("this is <b>rich</b> text!");
    delete label;

    label = new MLabel();
    label->setText("this is <b>rich</b> text!");
    MLabelModel *model = new MLabelModel();
    model->setText("this is plain text!");
    label->setModel(model);
    delete label;
}

void Ut_MLabel::testAutomaticTextAlignment_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("wordWrap");
    QTest::addColumn<Qt::Alignment>("alignment");
    QTest::addColumn<Qt::Alignment>("finalAlignmentShouldBe");

    //----------------------------------------------------------------------
    // “ar” locale non-mirorred:
    QTest::newRow("locale ar, left aligned english single line, wordwrap off")
        << "ar"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned english single line, wordwrap on")
        << "ar"
        <<  "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned english multiline, wordwrap off")
        << "ar"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned english multiline, wordwrap on")
        << "ar"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);

    QTest::newRow("locale ar, left aligned arabic single line, wordwrap off")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned arabic single line, wordwrap on")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned arabic multiline, wordwrap off")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar, left aligned arabic multiline, wordwrap on")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale ar, right aligned english single line, wordwrap off")
        << "ar"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned english single line, wordwrap on")
        << "ar"
        << "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned english multiline, wordwrap off")
        << "ar"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned english multiline, wordwrap on")
        << "ar"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale ar, right aligned arabic single line, wordwrap off")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned arabic single line, wordwrap on")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned arabic multiline, wordwrap off")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar, right aligned arabic multiline, wordwrap on")
        << "ar"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    //----------------------------------------------------------------------
    // “en” locale, same as “ar” non-mirorred:
    QTest::newRow("locale en, left aligned english single line, wordwrap off")
        << "en"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned english single line, wordwrap on")
        << "en"
        <<  "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned english multiline, wordwrap off")
        << "en"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned english multiline, wordwrap on")
        << "en"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);

    QTest::newRow("locale en, left aligned arabic single line, wordwrap off")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned arabic single line, wordwrap on")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned arabic multiline, wordwrap off")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en, left aligned arabic multiline, wordwrap on")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale en, right aligned english single line, wordwrap off")
        << "en"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned english single line, wordwrap on")
        << "en"
        << "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned english multiline, wordwrap off")
        << "en"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned english multiline, wordwrap on")
        << "en"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale en, right aligned arabic single line, wordwrap off")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned arabic single line, wordwrap on")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned arabic multiline, wordwrap off")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en, right aligned arabic multiline, wordwrap on")
        << "en"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    //----------------------------------------------------------------------
    // “ar@layout-direction=auto” locale, i.e. Arabic with mirrored UI:
    QTest::newRow("locale ar@layout-direction=auto, left aligned english single line, wordwrap off")
        << "ar@layout-direction=auto"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, left aligned english single line, wordwrap on")
        << "ar@layout-direction=auto"
        <<  "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, left aligned english multiline, wordwrap off")
        << "ar@layout-direction=auto"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, left aligned english multiline, wordwrap on")
        << "ar@layout-direction=auto"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);

    QTest::newRow("locale ar@layout-direction=auto, left aligned arabic single line, wordwrap off")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, left aligned arabic single line, wordwrap on")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, left aligned arabic multiline, wordwrap off")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, left aligned arabic multiline, wordwrap on")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale ar@layout-direction=auto, right aligned english single line, wordwrap off")
        << "ar@layout-direction=auto"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, right aligned english single line, wordwrap on")
        << "ar@layout-direction=auto"
        << "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, right aligned english multiline, wordwrap off")
        << "ar@layout-direction=auto"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale ar@layout-direction=auto, right aligned english multiline, wordwrap on")
        << "ar@layout-direction=auto"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale ar@layout-direction=auto, right aligned arabic single line, wordwrap off")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, right aligned arabic single line, wordwrap on")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, right aligned arabic multiline, wordwrap off")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale ar@layout-direction=auto, right aligned arabic multiline, wordwrap on")
        << "ar@layout-direction=auto"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    //----------------------------------------------------------------------
    // “en@layout-direction=rtl” locale, i.e. English with UI forced to RTL direction:
    QTest::newRow("locale en@layout-direction=rtl, left aligned english single line, wordwrap off")
        << "en@layout-direction=rtl"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned english single line, wordwrap on")
        << "en@layout-direction=rtl"
        <<  "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned english multiline, wordwrap off")
        << "en@layout-direction=rtl"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned english multiline, wordwrap on")
        << "en@layout-direction=rtl"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignLeft);

    QTest::newRow("locale en@layout-direction=rtl, left aligned arabic single line, wordwrap off")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned arabic single line, wordwrap on")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned arabic multiline, wordwrap off")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);
    QTest::newRow("locale en@layout-direction=rtl, left aligned arabic multiline, wordwrap on")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignLeft)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale en@layout-direction=rtl, right aligned english single line, wordwrap off")
        << "en@layout-direction=rtl"
        << "This is plain text!"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned english single line, wordwrap on")
        << "en@layout-direction=rtl"
        << "This is plain text!"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned english multiline, wordwrap off")
        << "en@layout-direction=rtl"
        << "This is plain text!\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned english multiline, wordwrap on")
        << "en@layout-direction=rtl"
        << "This is plain text!\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("locale en@layout-direction=rtl, right aligned arabic single line, wordwrap off")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned arabic single line, wordwrap on")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned arabic multiline, wordwrap off")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << false
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("locale en@layout-direction=rtl, right aligned arabic multiline, wordwrap on")
        << "en@layout-direction=rtl"
        << "ﺎﻠﻋﺮﺒﻳﺓ\nSecond line"
        << true
        << Qt::Alignment(Qt::AlignRight)
        << Qt::Alignment(Qt::AlignLeft);
}

void Ut_MLabel::testAutomaticTextAlignment()
{
    QFETCH(QString, localeName);
    QFETCH(QString, text);
    QFETCH(bool, wordWrap);
    QFETCH(Qt::Alignment, alignment);
    QFETCH(Qt::Alignment, finalAlignmentShouldBe);

    MLocale currentLocale;
    MLocale locale(localeName);
    MLocale::setDefault(locale);

    label->setWordWrap(wordWrap);

    label->setMinimumSize(500, 500);
    label->setMaximumSize(500, 500);
    label->setPreferredSize(500, 500);
    label->setGeometry(QRectF(0, 0, 500, 500));
    label->setAlignment(alignment | (label->alignment() & Qt::AlignVertical_Mask));

    label->setText(text);
    QVERIFY(text == label->text());

    QImage image = captureImage(label);

    /* Now verify that the alignment is correct by forcing it (with the absolute flag) to the alignment it should be, and
     * checking that nothing changed*/
    label->setAlignment(finalAlignmentShouldBe | Qt::AlignAbsolute | (label->alignment() & Qt::AlignVertical_Mask));
    QImage shouldBeImage = captureImage(label);

    QVERIFY(shouldBeImage == image);
    MLocale::setDefault(currentLocale);
}

void Ut_MLabel::testTextAlignment_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is plain text!";
    QTest::newRow("rich") << "<b>this is rich text!</b>";
}
void Ut_MLabel::testTextAlignment()
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

void Ut_MLabel::testTextAlignmentFromCSS_data()
{
    QTest::addColumn<QString>("styleName");
    QTest::addColumn<Qt::Alignment>("alignment");

    QTest::newRow("aligned left") << "leftAligned" << Qt::Alignment(Qt::AlignLeft);
    QTest::newRow("aligned right") << "rightAligned" << Qt::Alignment(Qt::AlignRight);

    QTest::newRow("aligned absolute left1") << "leftAbsoluteAligned1" << Qt::Alignment(Qt::AlignLeft | Qt::AlignAbsolute);
    QTest::newRow("aligned absolute right1") << "rightAbsoluteAligned1" << Qt::Alignment(Qt::AlignRight | Qt::AlignAbsolute);

    QTest::newRow("aligned absolute left2") << "leftAbsoluteAligned2" << Qt::Alignment(Qt::AlignLeft | Qt::AlignAbsolute);
    QTest::newRow("aligned absolute right2") << "rightAbsoluteAligned2" << Qt::Alignment(Qt::AlignRight | Qt::AlignAbsolute);
}

void Ut_MLabel::testTextAlignmentFromCSS()
{
    QFETCH(QString, styleName);
    QFETCH(Qt::Alignment, alignment);

    label->setStyleName(styleName);
    QCOMPARE(label->alignment() & Qt::AlignHorizontal_Mask, alignment);
}
void Ut_MLabel::testLayoutDirection_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("english-plain") << "this is plain English text!";
    QTest::newRow("english-rich") << "<b>this is rich English text!</b>";
    QTest::newRow("arabic-plain") << QString::fromUtf8("العربية");
    QTest::newRow("arabic-rich") << QString::fromUtf8("<b>العربية</b>");
}

void Ut_MLabel::testLayoutDirection()
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

#if 0
    ltr.save("layoutdirection-" + QString(QTest::currentDataTag()) + "-ltr.png");
    rtl.save("layoutdirection-" + QString(QTest::currentDataTag()) + "-rtl.png");
#endif

    QVERIFY(ltr == rtl);
}

void Ut_MLabel::testTextDirection_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("english-arabic-english-plain")
        << QString::fromUtf8("Englishالعربيةlanguage");
    QTest::newRow("english-arabic-english-rich")
        << QString::fromUtf8("<b>Englishالعربيةlanguage</b>");
    QTest::newRow("arabic-english-arabic-plain")
        << QString::fromUtf8("العرEnglishبية");
    QTest::newRow("arabic-english-arabic-rich")
        << QString::fromUtf8("<b>العرEnglishبية</b>");
}

void Ut_MLabel::testTextDirection()
{
    QFETCH(QString, text);
    label->setText(text);
    QVERIFY(text == label->text());

    label->model()->setTextDirection(Qt::LeftToRight);
    QVERIFY(label->model()->textDirection() == Qt::LeftToRight);
    QImage ltr = captureImage(label);

    label->model()->setTextDirection(Qt::RightToLeft);
    QVERIFY(label->model()->textDirection() == Qt::RightToLeft);
    QImage rtl = captureImage(label);

#if 0
    ltr.save("textdirection-" + QString(QTest::currentDataTag()) + "-ltr.png");
    rtl.save("textdirection-" + QString(QTest::currentDataTag()) + "-rtl.png");
#endif

    QVERIFY(ltr != rtl);
}

void Ut_MLabel::testTextWordWrap_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("equal");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006." << true;
    QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>" << false;
}

void Ut_MLabel::testTextWordWrap()
{
    QFETCH(QString, text);

    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(true);
    label->resize(400,400);
    QVERIFY(label->wordWrap() == true);
    QImage wrapped = captureImage(label);

    label->setWordWrap(false);
    QVERIFY(label->wordWrap() == false);
    QImage unwrapped = captureImage(label);

    //wrapped.save("wrapped", "PNG");
    //unwrapped.save("unwrapped", "PNG");

    //
    QVERIFY(wrapped != unwrapped);
}

void Ut_MLabel::testTextElide_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("elidedEndWithoutWordWrap");
    QTest::addColumn<QString>("elidedEndWithWordWrap");

    QTest::newRow("plain") << "David Michael Hasselhoff (born July 17, 1952) is an American actor and singer.He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006."
                           << "American actor and singer...." << "in Wall was brought down in...";
    QTest::newRow("nospaces") << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                              << "xxxx..." << "xxxx...";
    // For some reason this doesn't work in CC -> solve why
    //QTest::newRow("rich") << "<b>David Michael Hasselhoff (born July 17, 1952) is an American actor and singer. He is best known for his lead roles as Michael Knight in the popular 1980s U.S. series Knight Rider and as L.A. County Lifeguard Mitch Buchannon in the series Baywatch. Hasselhoff also produced Baywatch for a number of series in the 1990s up until 2001 when the series ended with Baywatch Hawaii. Hasselhoff also crossed over to a music career during the end of the 1980s and the early 1990s and was noted for his performance when the Berlin Wall was brought down in 1989. He enjoyed a short lived success as a singer primarily in German-speaking Europe, particularly in Germany and Austria. More recently Hasselhoff has been involved with talent shows such as NBC's America's Got Talent in 2006. Hasselhoff's autobiography, Making Waves, was released in the United Kingdom in September 2006.</b>";
}

void Ut_MLabel::testTextElide()
{
    QFETCH(QString, text);
    QFETCH(QString, elidedEndWithoutWordWrap);
    QFETCH(QString, elidedEndWithWordWrap);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setWordWrap(false);
    label->setTextElide(true);
    label->resize(400, 100);
    QVERIFY(label->textElide() == true);
    QImage elided = captureImage(label);

#ifdef __arm__
    // The scratchbox and device environment use a different font and theme per default.
    // Only test the wrapping for the device to keep testTextElide_data() maintainable:
    const MLabelView* view = qobject_cast<const MLabelView*>(label->view());
    QString labelTextEnd = view->renderedText().right(elidedEndWithoutWordWrap.length());
    QCOMPARE(labelTextEnd, elidedEndWithoutWordWrap);
#endif

    label->setWordWrap(true);
    label->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    label->setTextElide(true);
    label->resize(400, 100);

#ifdef __arm__
    // The scratchbox and device environment use a different font and theme per default.
    // Only test the wrapping for the device to keep testTextElide_data() maintainable:
    labelTextEnd = view->renderedText().right(elidedEndWithWordWrap.length());
    QCOMPARE(labelTextEnd, elidedEndWithWordWrap);
#endif

    label->setWordWrap(false);
    label->setTextElide(false);
    label->resize(400, 100);
    QVERIFY(label->textElide() == false);
    QImage unelided = captureImage(label);

    label->setLayoutDirection(Qt::RightToLeft);
    QImage elidedRtl = captureImage(label);

    QVERIFY(elided != unelided);
    QVERIFY(elided != elidedRtl);
}

void Ut_MLabel::testRichTextElide_data()
{
    QTest::addColumn<QString>("text");
    QTest::newRow("rich") << "<b>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</b>";
}

void Ut_MLabel::testRichTextElide()
{
    QFETCH(QString, text);
    label->setLayoutDirection(Qt::LeftToRight);
    label->setWordWrap(false);
    label->setText(text);
    QVERIFY(text == label->text());

    label->setTextElide(true);
    label->resize(400,400);
    QVERIFY(label->textElide() == true);
    const QImage elided = captureImage(label);
    const QRect elidedRect = contentRect(elided);

    label->setTextElide(false);
    label->resize(400,400);
    QVERIFY(label->textElide() == false);
    const QImage unelided = captureImage(label);
    const QRect unelidedRect = contentRect(unelided);

    int cropWidth = 200;
    QImage leftUnelided = unelided.copy(0, 0, cropWidth, unelided.height());
    QImage rightUnelided = unelided.copy(cropWidth, 0, unelided.width() - cropWidth, unelided.height());

    QImage leftElided = elided.copy(0, 0, cropWidth, elided.height());
    QImage rightElided = elided.copy(cropWidth, 0, elided.width() - cropWidth, elided.height());

    QCOMPARE(elided.isNull(), false);
    QCOMPARE(unelided.isNull(), false);

    QVERIFY(elided != unelided);
    QVERIFY(elidedRect != unelidedRect);

    //'...' is added to the rightmost part of the label in LTR mode
    QVERIFY(leftUnelided == leftElided);
    QVERIFY(rightUnelided != rightElided);

}

void Ut_MLabel::testHighlighting()
{
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter *h = new MCommonLabelHighlighter(QRegExp("Label"));
    label->addHighlighter(h);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    //click and long press
    QSignalSpy spyClick(h, SIGNAL(clicked(QString)));
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spyClick.count(), 1);
    QSignalSpy spyLongPress(h, SIGNAL(longPressed(QString)));
    QList<QGesture *> list;
    QTapAndHoldGesture gesture;
    gesture.setPosition(QPointF(15, 5));
    list.append( &gesture );
    QGestureEvent gestureEvent(list);
    label->tapAndHoldGestureEvent (&gestureEvent, &gesture );
    QCOMPARE(spyLongPress.count(), 1);

    //click and long press when events are ignored
    h->setIgnoreClickAndLongPressEvents(true);
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spyClick.count(), 1);
    label->tapAndHoldGestureEvent (&gestureEvent, &gesture );
    QCOMPARE(spyLongPress.count(), 1);

    label->removeHighlighter(NULL);
    label->removeHighlighter(h);
    delete h;
}

void Ut_MLabel::testHighlightingModel()
{
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter *h = new MCommonLabelHighlighter(QRegExp("Label"));
    label->model()->addHighlighter(h);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    QSignalSpy spyClick(h, SIGNAL(clicked(QString)));
    label->simulateClick(QPoint(15, 5));
    QCOMPARE(spyClick.count(), 1);

    QSignalSpy spyLongPress(h, SIGNAL(longPressed(QString)));

    QList<QGesture *> list;
    QTapAndHoldGesture gesture;
    gesture.setPosition(QPointF(15, 5));
    list.append( &gesture );
    QGestureEvent gestureEvent(list);
    label->tapAndHoldGestureEvent (&gestureEvent, &gesture );

    QCOMPARE(spyLongPress.count(), 1);

    label->model()->removeHighlighter(NULL);
    label->model()->removeHighlighter(h);
    delete h;
}

Qt::GestureState QGesture::state() const {
    return Qt::GestureFinished;
}

void Ut_MLabel::testAnchor()
{
    label->setText("<a href=\"http://www.nokia.com\">www.nokia.com</a>");
    anchorHitTest(QPointF(15, 5), 1);
    anchorHitTest(QPointF(200, 0), 0);
}

void Ut_MLabel::anchorHitTest(const QPointF &point, int expectedHits)
{
    QSignalSpy spy(label, SIGNAL(linkActivated(QString)));
    label->simulateClick(point);
    QCOMPARE(spy.count(), expectedHits);
}

QImage Ut_MLabel::captureImage(MLabel *label)
{
    QPixmap pixmap(label->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    label->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

void Ut_MLabel::testClear_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testClear()
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

void Ut_MLabel::testPlainTextColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testPlainTextColor()
{
    QFETCH(QString, text);

    TestMLabel *label = new TestMLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));
    QImage imageLabel = captureImage(label);

    TestMLabel *anotherLabel = new TestMLabel();
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
void Ut_MLabel::testSizeHint_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QTextOption::WrapMode>("wrapMode");
    QTest::addColumn<qreal>("widthConstraint");

    QStringList texts;
    texts.append("Short plain text");
    texts.append("Long plain text that gets wrapped several times within the label boundaries.");
    texts.append("UnwrappablePlainTextWithoutSpaces");
    texts.append("Partly wrappable text containing VeryLongUnwrappableWordsThatExceedTheWidth of the label.");
    texts.append("<b>Some short rich text</b>");
    texts.append("Long rich text <b>that</b> gets wrapped <i>several times</i> within the label boundaries.");

    typedef QPair<QByteArray, QTextOption::WrapMode> WrapInfo;
    QList<WrapInfo> wrapModes;
    wrapModes.append(WrapInfo("NoWrap", QTextOption::NoWrap));
    wrapModes.append(WrapInfo("WordWrap", QTextOption::WordWrap));
    wrapModes.append(WrapInfo("ManualWrap", QTextOption::ManualWrap));
    wrapModes.append(WrapInfo("WrapAnywhere", QTextOption::WrapAnywhere));
    wrapModes.append(WrapInfo("WrapAtWordBoundaryOrAnywhere", QTextOption::WrapAtWordBoundaryOrAnywhere));

    static QList<QByteArray> descriptions;
    for (qreal widthConstraint = -1.0; widthConstraint < 90.0; widthConstraint += 30.0) {
        foreach (const WrapInfo &wrapInfo, wrapModes) {
            foreach (const QString &text, texts) {
                const QString descr = text.left(20) + ".../" + wrapInfo.first + "/" + QString::number(widthConstraint);
                descriptions.append(descr.toAscii());
                QTest::newRow(descriptions.last().constData()) << text << wrapInfo.second << widthConstraint;
            }
        }
    }
}

void Ut_MLabel::testSizeHint()
{
    QFETCH(QString, text);
    QFETCH(QTextOption::WrapMode, wrapMode);
    QFETCH(qreal, widthConstraint);

    TestMLabel *label = new TestMLabel();
    label->setText(text);
    label->setGeometry(QRectF(0, 0, 400, 20));
    QVERIFY(label->sizePolicy().hasHeightForWidth());

    label->setWordWrap(true);
    label->setWrapMode(wrapMode);

    QSizeF constraint(widthConstraint, -1);
    QSizeF minSizeHint = label->sizeHint(Qt::MinimumSize, constraint);
    QSizeF maxSizeHint = label->sizeHint(Qt::MaximumSize, constraint);
    QSizeF prefSizeHint = label->sizeHint(Qt::PreferredSize, constraint);

    // Test minimum size-hint
    QFontMetricsF fm(label->font());
    if (constraint.width() > 0.0) {
        QCOMPARE(minSizeHint.width(), constraint.width());
    } else if (wrapMode == QTextOption::NoWrap || wrapMode == QTextOption::ManualWrap ) {
        QCOMPARE(minSizeHint.width(), prefSizeHint.width());
    } else {
        QCOMPARE((int)(minSizeHint.width()+0.5), (int)(fm.width(QLatin1Char('x'))+0.5)); //round up the width for the x character
    }
    if (constraint.height() > 0.0) {
        QCOMPARE(minSizeHint.width(), constraint.height());
    } else if (wrapMode == QTextOption::NoWrap || wrapMode == QTextOption::ManualWrap ) {
        QCOMPARE(minSizeHint.height(), prefSizeHint.height());
    } else {
        QCOMPARE(minSizeHint.height(), fm.height());
    }

    // Test maximum size-hint
    if (constraint.width() > 0.0) {
        QCOMPARE(maxSizeHint.width(), constraint.width());
    } else {
        QCOMPARE(maxSizeHint.width(), qreal(QWIDGETSIZE_MAX));
    }
    if (constraint.height() > 0.0) {
        QCOMPARE(maxSizeHint.height(), constraint.height());
    } else {
        QCOMPARE(maxSizeHint.height(), qreal(QWIDGETSIZE_MAX));
    }

    // Test preferred size-hint
    QVERIFY(prefSizeHint.width() <= maxSizeHint.width());
    QVERIFY(prefSizeHint.height() <= maxSizeHint.height());

    QSizeF targetSize = requiredTextSize(text, label->font(), wrapMode, prefSizeHint.width());
    if (constraint.width() > 0.0) {
        QVERIFY(prefSizeHint.width() <= constraint.width());
        targetSize.rwidth() = constraint.width();
    }
    if (constraint.height() > 0.0) {
        QVERIFY(prefSizeHint.height() <= constraint.height());
        targetSize.rheight() = constraint.height();
    }
    QCOMPARE(prefSizeHint, targetSize);

    // Check that resizing does not change the sizehint
    QSizeF prefSizeHintWithoutConstraint = label->preferredSize();
    label->resize(prefSizeHint);
    QCOMPARE(label->sizeHint(Qt::MinimumSize, constraint), minSizeHint);
    QCOMPARE(label->sizeHint(Qt::PreferredSize, constraint), prefSizeHint);
    QCOMPARE(label->preferredSize(), prefSizeHintWithoutConstraint);
    QCOMPARE(label->sizeHint(Qt::MaximumSize, constraint), maxSizeHint);

    {
        //Check setting constraints via css
        label->setObjectName("width40");
        qreal width = 40.0;
        QSizeF minSizeHint = label->minimumSize();
        QSizeF prefSizeHint = label->preferredSize();
        QSizeF maxSizeHint = label->maximumSize();

        if (wrapMode == QTextOption::NoWrap || wrapMode == QTextOption::ManualWrap )
            width = qMax(width, minSizeHint.width());
        QCOMPARE(prefSizeHint.width(), width);

        label->resize(prefSizeHint);
        QCOMPARE(minSizeHint, label->minimumSize());
        QCOMPARE(prefSizeHint, label->preferredSize());
        QCOMPARE(maxSizeHint, label->maximumSize());
        label->setObjectName("");
    }
}

void Ut_MLabel::testWordWrappingSizeHint_data()
{
    QTest::addColumn<QTextOption::WrapMode>("wrapMode");
    QTest::addColumn<qreal>("widthConstraint");

    typedef QPair<QString, QTextOption::WrapMode> WrapInfo;
    QList<WrapInfo> wrapModes;
    wrapModes.append(WrapInfo("NoWrap", QTextOption::NoWrap));
    wrapModes.append(WrapInfo("WordWrap", QTextOption::WordWrap));
    wrapModes.append(WrapInfo("ManualWrap", QTextOption::ManualWrap));
    wrapModes.append(WrapInfo("WrapAnywhere", QTextOption::WrapAnywhere));
    wrapModes.append(WrapInfo("WrapAtWordBoundaryOrAnywhere", QTextOption::WrapAtWordBoundaryOrAnywhere));

    foreach (const WrapInfo &wrapInfo, wrapModes) {
        QTest::newRow(wrapInfo.first.toLatin1()) << wrapInfo.second;;
    }

}

void Ut_MLabel::testWordWrappingSizeHint()
{
    QFETCH(QTextOption::WrapMode, wrapMode);
    TestMLabel *label = new TestMLabel();
    label->setText("This is a long string with lots of words, lots of words, lots of words.");

    label->setWordWrap(true);
    label->setWrapMode(wrapMode);
    QSizeF prefSize = label->preferredSize();

    //Check that setting the constraint to the preferred width doesn't change the preferred height
    QCOMPARE(prefSize, label->sizeHint(Qt::PreferredSize, QSizeF(prefSize.width(), -1)));

    //Now check that if we add a constraint to force word wrapping, the height increases.
    QSizeF prefSizeWithConstraint = label->sizeHint(Qt::PreferredSize, QSizeF(prefSize.width() * 0.8, -1));

    //Check that the preferred height has not changed if we word wrapping is disabled, and has increased if word wrap is enabled
    if (wrapMode == QTextOption::NoWrap || wrapMode == QTextOption::ManualWrap)
        QCOMPARE(prefSizeWithConstraint.height(), prefSize.height());
    else
        QVERIFY(prefSizeWithConstraint.height() > prefSize.height());

    //Check preferred size a second time to make sure it's cached..
    QCOMPARE(prefSize, label->sizeHint(Qt::PreferredSize, QSizeF(prefSize.width(), -1)));

    //Add some text and make sure it now wraps to the next line
    label->setText( label->text() + "Goodbye");
    prefSizeWithConstraint = label->sizeHint(Qt::PreferredSize, QSizeF(prefSize.width(), -1));

    //Check again that the preferred height has not changed if we word wrapping is disabled, and has increased if word wrap is enabled
    if (wrapMode == QTextOption::NoWrap || wrapMode == QTextOption::ManualWrap)
        QCOMPARE(prefSizeWithConstraint.height(), prefSize.height());
    else
        QVERIFY(prefSizeWithConstraint.height() > prefSize.height());
}

void Ut_MLabel::testUnknownSizeHint()
{
    QSizeF hint = label->sizeHint((Qt::SizeHint)12345);
    Q_UNUSED(hint);
}

void Ut_MLabel::testFont_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testFont()
{
    QFETCH(QString, text);

    MLabel *label = new TestMLabel();
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

void Ut_MLabel::testColor_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "this is just some plain text";
    QTest::newRow("rich") << "<b>this is just some rich text.</b>";
}

void Ut_MLabel::testColor()
{
    QFETCH(QString, text);

    MLabel *label = new TestMLabel();
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

void Ut_MLabel::linefeedBeforeFirstTag()
{
    label->setWordWrap(true);
    label->resize(400,400);

    label->setText("\n<b></b>\na\nb\nc\nd\ne\nf\ng\n");
    QImage image1  = captureImage(label);
    
    label->setText("\na<br>b<br>c<br>d<br>e<br>");
    QImage image2  = captureImage(label);
    
    //Linefeed characters are ignored and but <br> tags are interpreted as line breaks

    //Ignoring linefeeds causes content to be horizontal
    QVERIFY2(contentWidth(image1) > contentHeight(image1), "'\n' needs to be ignored in rich label");
    //Interpreting <br> tags causes content to be vertical
    QVERIFY2(contentHeight(image2) > contentWidth(image2), "<br> needs to work in rich label");

}

void Ut_MLabel::highlighterEmptyRegexp()
{   
    //This test verifies that label doesn't
    //go to an infinite loop when a highlighter 
    //matching an empty string is used.
    
    QRegExp regexp("^");
    MCommonLabelHighlighter highlighter(regexp);
    label->setText("test");
    label->addHighlighter(&highlighter);
    captureImage(label);
}


void Ut_MLabel::removeAllHighlighters()
{
    label->setText("Label <b>rich</b>");

    QImage nonhighlighted = captureImage(label);

    MCommonLabelHighlighter highlighter(QRegExp("Label"));
    label->addHighlighter(&highlighter);
    QImage highlighted = captureImage(label);

    QVERIFY(nonhighlighted != highlighted);

    label->removeAllHighlighters();
    QImage highlightingRemoved = captureImage(label);
    QVERIFY(nonhighlighted == highlightingRemoved);
    
    
}

QRect Ut_MLabel::contentRect(const QImage& image)
{
    int minX, minY, maxX, maxY;

    bool found = false;
    for(minX = 0; !found && minX < image.width(); ++minX) {
        for(int i = 0; !found && i < image.height(); ++i) {
            if (image.pixel(minX, i) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(minY = 0; !found && minY < image.height(); ++minY) {
        for(int i = 0; !found && i < image.width(); ++i) {
            if (image.pixel(i, minY) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(maxX = image.width() - 1; !found && maxX >= 0; --maxX) {
        for(int i = 0; !found && i < image.height(); ++i) {
            if (image.pixel(maxX, i) != 0) {
                found = true;
            }
        }
    }

    found = false;
    for(maxY = image.height() - 1; !found && maxY >= 0; --maxY) {
        for(int i = 0; !found && i < image.width(); ++i) {
            if (image.pixel(i, maxY) != 0) {
                found = true;
            }
        }
    }

    return QRect(QPoint(minX, minY), QPoint(maxX, maxY));

}

int Ut_MLabel::contentHeight(const QImage& image)
{
    QRect rect = contentRect(image);
    return rect.bottom() - rect.top();
}

int Ut_MLabel::contentWidth(const QImage& image)
{
    QRect rect = contentRect(image);
    return rect.right() - rect.left();
}

void Ut_MLabel::wrapModes_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plain") << "aaa bbb ccc dddddddddddddddd eeeeeeeeeeeeeee fffffffffffffffff gggggggggggggggg hhhhhhhhhhhhhhhh iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj kkkkkkkkkkk";
    
    QTest::newRow("rich") << "<b>aaa bbb ccc dddddddddddddddd eeeeeeeeeeeeeee fffffffffffffffff gggggggggggggggg hhhhhhhhhhhhhhhh iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj kkkkkkkkkkk</b>";
}
    
void Ut_MLabel::wrapModes()
{
    QFETCH(QString, text);
    label->setText(text);
    label->setMaximumSize(100,800);
    label->resize(100, 800);

    QImage defaultMode = captureImage(label);

    label->setWordWrap(true);

    label->setWrapMode(QTextOption::ManualWrap);
    QCOMPARE(label->wrapMode(), QTextOption::ManualWrap);
    QImage manualWrap = captureImage(label);

    label->setWrapMode(QTextOption::WordWrap);
    QCOMPARE(label->wrapMode(), QTextOption::WordWrap);
    QImage wordWrap = captureImage(label);
    
    label->setWrapMode(QTextOption::WrapAnywhere);
    QCOMPARE(label->wrapMode(), QTextOption::WrapAnywhere);
    QImage wrapAnywhere = captureImage(label);

    label->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    QCOMPARE(label->wrapMode(), QTextOption::WrapAtWordBoundaryOrAnywhere);
    QImage wrapAtWordBoundaryOrAnywhere = captureImage(label);

    label->setWrapMode(QTextOption::NoWrap);
    QCOMPARE(label->wrapMode(), QTextOption::NoWrap);
    QImage noWrap = captureImage(label);
    
    //No wrapping by default
    QVERIFY(contentWidth(defaultMode) > contentHeight(defaultMode));

    QVERIFY(contentWidth(manualWrap) > contentHeight(manualWrap));
    QVERIFY(contentHeight(wordWrap) > contentWidth(wordWrap));
    QVERIFY(contentHeight(wrapAnywhere) > contentWidth(wrapAnywhere));
    QVERIFY(contentHeight(wrapAtWordBoundaryOrAnywhere) > contentWidth(wrapAtWordBoundaryOrAnywhere));
    QVERIFY(contentWidth(noWrap) > contentHeight(noWrap));

    //Words that don't fit one line are not displayed 
    //completely in wordWrap mode. In wrapAtWordBoundaryOrAnywhere mode these long 
    //words are displayed by using more than one line.
    QVERIFY(contentHeight(wrapAtWordBoundaryOrAnywhere) > contentHeight(wordWrap));

}

void Ut_MLabel::multiLengthSeparator()
{
    label->resize(100, 100);
    const MLabelView* view = qobject_cast<const MLabelView*>(label->view());

    QChar separatorChar(0x9c, 0);
    QString separator(separatorChar);

    QString shortWord = "short";
    QString longWord = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    QString shortAndShort = shortWord + separator + shortWord;
    QString longAndShort = longWord + separator + shortWord;

    label->setText(shortAndShort);
    QImage shortAndShortCapture = captureImage(label);
    QCOMPARE(view->renderedText(), shortWord);
    label->setText(shortWord);
    QImage shortCapture = captureImage(label);
    QCOMPARE(view->renderedText(), shortWord);

    QCOMPARE(shortCapture.isNull(), false);
    QCOMPARE(shortAndShortCapture.isNull(), false);
    QCOMPARE(shortCapture, shortAndShortCapture);

    label->setText(longAndShort);
    QImage longAndShortCapture = captureImage(label);

    QCOMPARE(view->renderedText(), shortWord);
    QCOMPARE(shortCapture, longAndShortCapture);

}

QSizeF Ut_MLabel::requiredTextSize(const QString &text, const QFont &font,
                                   QTextOption::WrapMode wrapMode, int width)
{
    QSizeF requiredSize;

    QTextOption textOption;
    textOption.setWrapMode(wrapMode);

    if (Qt::mightBeRichText(text)) {
        QTextDocument textDocument(text);

        const QString spanTag("<span>%1</span>");
        textDocument.setHtml(spanTag.arg(text));

        textDocument.setDocumentMargin(0);
        textDocument.setDefaultFont(font);
        textDocument.setDefaultTextOption(textOption);
        textDocument.setTextWidth(width);

        requiredSize = textDocument.size();
    } else {
        // For simple text QStaticText is used, which does a slightly different
        // layouting than QTextDocument (see QStaticText::paintText() as reference).
        QTextLayout textLayout;
        textLayout.setText(text);
        textLayout.setFont(font);
        textLayout.setTextOption(textOption);
        qreal leading = QFontMetricsF(font).leading();
        qreal height = -leading;

        textLayout.beginLayout();
        while (1) {
            QTextLine line = textLayout.createLine();
            if (!line.isValid()) {
                break;
            }

            if (width >= 0.0) {
                line.setLineWidth(width);
            }
            height += leading;
            line.setPosition(QPointF(0.0, height));
            height += line.height();
        }
        textLayout.endLayout();

        requiredSize = textLayout.boundingRect().size();
    }

    return requiredSize;
}

QImage Ut_MLabel::captureText(Qt::TextFormat textFormat, QString text)
{
    label->setTextFormat(textFormat);
    label->setText(text);
    
    return captureImage(label);
}

void Ut_MLabel::testAutoTextModeRecognizesPlainText()
{
    QString plainText("text");
    QCOMPARE(captureText(Qt::AutoText, plainText), captureText(Qt::PlainText, plainText));
}

void Ut_MLabel::testAutoTextModeRecognizesRichText()
{
    QString richText("<b>text</b>");
    QCOMPARE(captureText(Qt::AutoText, richText), captureText(Qt::RichText, richText));
}

void Ut_MLabel::testPlainAndRichTextModes()
{
    QString richText("<b>text</b>");
    QVERIFY2(captureText(Qt::PlainText, richText) != captureText(Qt::RichText, richText), 
             "Plain text mode renders rich text differently");
}

void Ut_MLabel::rightBearing()
{
    label->setWordWrap(true);
    label->setWrapMode(QTextOption::WrapAnywhere);
    label->setFont(QFont("Times", 30, -1, true));
    label->setText("f");
    QImage f_image = captureImage(label);
    label->setText("ff");
    QImage ff_image = captureImage(label);
    //Check that the "ff" label is not wrapped by comparing its height
    //with the minimum height needed to put f's in two lines (the real height
    //needed is more than 2 * contentHeight(f_image), because of the space
    //between lines, so this test is more restrictive)
    QVERIFY(contentHeight(ff_image) < 2 * contentHeight(f_image));
}

void Ut_MLabel::testTextFormat()
{
    label->setText("Hello world!");
    label->setTextFormat(Qt::RichText);
    QCOMPARE(label->textFormat(), Qt::RichText);

    label->setTextFormat(Qt::PlainText);
    QCOMPARE(label->textFormat(), Qt::PlainText);
}

void Ut_MLabel::testRichTextTiles()
{
    // A MSceneWindow is required as parent of MLabel for testing the tiles.
    // The tiling functionality relies on QPixmapCache which can be cleared
    // at any time. This makes it necessary to check whether tiling is available
    // at all before triggering a wrong failure.
    MApplicationWindow window;
    MApplicationPage page;

    const int labelY = 300;
    MLabel *tiledLabel = new MLabel("Short richtext, only <i>one</i> tile should be used", page.centralWidget());
    tiledLabel->setWordWrap(true);
    tiledLabel->setWrapMode(QTextOption::WrapAnywhere);
    tiledLabel->setGeometry(QRectF(0, labelY, 400, 400));

    page.appear(&window);
    window.show();

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    const MLabelView* view = qobject_cast<const MLabelView*>(tiledLabel->view());

    int y0;
    int y1;
    QPixmap tile0;
    QPixmap tile1;

    bool ok = view->tileInformation(0, tile0, y0);
    ok = ok && view->tileInformation(1, tile1, y1);
    if (!ok) {
        qWarning() << "Skipped testRichTextTiles(): No tiles are available.";
        return;
    }

    QVERIFY(!tile0.isNull());
    QVERIFY(tile1.isNull());
    QVERIFY(!contentRect(tile0.toImage()).isEmpty());
    QCOMPARE(y0, 0);

    QString longText;
    for (int i = 0; i < 1000; ++i) {
        longText.append("Long text: <i>Two</i> tiles should be used if the height of the label is large enough.");
    }
    tiledLabel->setText(longText);

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    ok = ok && view->tileInformation(0, tile0, y0);
    ok = ok && view->tileInformation(1, tile1, y1);
    if (!ok) {
        qWarning() << "Skipped testRichTextTiles(): No tiles are available.";
        return;
    }

    QVERIFY(!tile0.isNull());
    QVERIFY(tile1.isNull());
    QCOMPARE(y0, 0);

    tiledLabel->resize(QSizeF(400, 10000));

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    MPannableViewport* pannableViewport = 0;
    foreach(QGraphicsItem *childItem, page.childItems()) {
        MPannableViewport *viewport = dynamic_cast<MPannableViewport*>(childItem);
        if (viewport) {
            pannableViewport = viewport;
            break;
        }
    }
    QVERIFY(pannableViewport);

    const int maxViewportY = tiledLabel->size().height() / 2;
    for (int viewportY = 0; viewportY < maxViewportY; viewportY += 200) {
        const QPointF newPos(0, viewportY);
        pannableViewport->physics()->setPosition(newPos);

        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

        ok = ok && view->tileInformation(0, tile0, y0);
        ok = ok && view->tileInformation(1, tile1, y1);
        if (!ok) {
            qWarning() << "Skipped testRichTextTiles(): No tiles are available.";
            return;
        }

        const int tileHeight = tile0.height();
        QCOMPARE(tile1.height(), tileHeight);

        if (y0 > y1) {
            qSwap(y0, y1);
        }

        QVERIFY(y0 <= viewportY + labelY);
        QVERIFY(y1 + tileHeight >= viewportY + labelY);
        QCOMPARE(y0 + tileHeight, y1);
        QVERIFY(!tile0.isNull());
        QVERIFY(!tile1.isNull());
        QVERIFY(!contentRect(tile0.toImage()).isEmpty());
        QVERIFY(!contentRect(tile1.toImage()).isEmpty());
    }
}

void Ut_MLabel::testDiacritics_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("textWithDiacritics");

    const QString text = "ABC gyj";
    const QString textWithDiacritics = QString::fromWCharArray(L"\u00C5\u00C4\u00C3\u00DA gyj");

    QTest::newRow("plain") << text << textWithDiacritics;
    QTest::newRow("rich")  << QString("<b>" + text + "</b>") << QString("<b>" + textWithDiacritics + "</b>");
}

void Ut_MLabel::testDiacritics()
{
    QFETCH(QString, text);
    QFETCH(QString, textWithDiacritics);

    label->setFont(QFont("Nokia Pure Text", 30));
    label->setStyleName("margins");

    label->setText(text);
    const QRect textRect = contentRect(captureImage(label));

    label->setText(textWithDiacritics);
    qApp->processEvents();
    const QRect textWithDiacriticsRect = contentRect(captureImage(label));

    QVERIFY(textWithDiacriticsRect.height() >= textRect.height() + 6);
}

void Ut_MLabel::testLineBreakReplacement_data()
{
    QTest::addColumn<bool>("wordWrap");
    QTest::addColumn<QTextOption::WrapMode>("wrapMode");
    QTest::addColumn<int>("expectedLineCount");

    QTest::newRow("no wordWrap, NoWrap") << false << QTextOption::NoWrap << 1;
    QTest::newRow("wordWrap, NoWrap") << true << QTextOption::NoWrap << 1;
    QTest::newRow("wordWrap, WrapAnywhere") << true << QTextOption::WrapAnywhere << 3;
}

void Ut_MLabel::testLineBreakReplacement()
{
    QFETCH(bool, wordWrap);
    QFETCH(QTextOption::WrapMode, wrapMode);
    QFETCH(int, expectedLineCount);

    label->resize(300, 300);

    label->setText("A\nB\nC");
    QImage img = label->getLabelImage();
    const int singleLineContentHeight = contentRect(img).height();

    label->setWordWrap(wordWrap);
    label->setWrapMode(wrapMode);
    img = label->getLabelImage();
    const int contentHeight = contentRect(img).height();
    QVERIFY(contentHeight >= singleLineContentHeight * expectedLineCount);
}

void Ut_MLabel::testPreferredLineCount_data()
{
    QTest::addColumn<bool>("richText");
    QTest::addColumn<bool>("useCSS");
    QTest::addColumn<bool>("wordWrap");
    QTest::addColumn<QTextOption::WrapMode>("wrapMode");

    for (int richText = 1; richText < 2; richText++)
        for (int useCSS = 0; useCSS < 1; useCSS++)
            for (int wrapMode = 0; wrapMode < 1; wrapMode++) {
                QString description = QString(richText?"Richtext ":"Plaintext ") + (useCSS?"CSS ":"setPreferredLineCount() ");
                if (wrapMode == 0)
                    QTest::newRow((description + "no wordWrap, NoWrap").toLatin1()) << (bool)richText << (bool)useCSS << false << QTextOption::NoWrap;
                else if (wrapMode == 1)
                    QTest::newRow((description + "wordWrap, NoWrap").toLatin1()) << (bool)richText << (bool)useCSS << true << QTextOption::NoWrap;
                else
                    QTest::newRow((description + "wordWrap, WrapAnywhere").toLatin1()) << (bool)richText << (bool)useCSS << true << QTextOption::WrapAnywhere;
            }
}

QString removeHtml(QString string) {
    return string.replace(QRegExp("<head>.*</head>"), "").replace(QRegExp("<br[^>]*>"), "\n").replace(QRegExp("<[^>]*>"), "").trimmed();
}

void Ut_MLabel::testPreferredLineCount()
{
    QFETCH(bool, richText);
    QFETCH(bool, useCSS);
    QFETCH(bool, wordWrap);
    QFETCH(QTextOption::WrapMode, wrapMode);

    QCOMPARE(label->preferredLineCount(), -1);
    label->setWordWrap(wordWrap);
    label->setWrapMode(wrapMode);
    label->setTextElide(true);
    if (richText)
        label->setText("<qt>AAA<br>BBB<br>CCC");
    else
        label->setText("A\nB\nC");
    label->resize(label->preferredSize());
    const MLabelView* view = qobject_cast<const MLabelView*>(label->view());
    if (wrapMode == QTextOption::WrapAnywhere || richText)
        QCOMPARE(removeHtml(view->renderedText()), QString("AAA\nBBB\nCCC"));
    else
        QCOMPARE(removeHtml(view->renderedText()), QString("A B C"));
    if (useCSS)
        label->setStyleName("lineCount3");
    else
        label->setPreferredLineCount(3);
    label->resize(label->preferredSize());
    if (wrapMode == QTextOption::WrapAnywhere || richText)
        QCOMPARE(removeHtml(view->renderedText()), QString("AAA\nBBB\nCCC"));
    else
        QCOMPARE(removeHtml(view->renderedText()), QString("A B C"));
    if (useCSS)
        label->setStyleName("lineCount2");
    else
        label->setPreferredLineCount(2);
    QVERIFY(label->size() != label->preferredSize()); // Size should have changed since we decreased the number of visible lines
    label->resize(label->preferredSize());
    QString ellipsisChar("...");
    if (wrapMode == QTextOption::WrapAnywhere || richText)
        QCOMPARE(removeHtml(view->renderedText()), QString("AAA\nBBB") + ellipsisChar);
    else
        QCOMPARE(removeHtml(view->renderedText()), QString("A B C"));
    if (useCSS)
        label->setStyleName("lineCount1");
    else
        label->setPreferredLineCount(1);
    label->resize(label->preferredSize());
    if (wrapMode == QTextOption::WrapAnywhere || richText)
        QCOMPARE(removeHtml(view->renderedText()), QString("AAA") + ellipsisChar);
    else
        QCOMPARE(removeHtml(view->renderedText()), QString("A B C"));
    if (useCSS)
        label->setStyleName("lineCount0");
    else
        label->setPreferredLineCount(0);
    label->setMinimumSize(0,0);
    label->resize(label->preferredSize());
    QCOMPARE(removeHtml(view->renderedText()), QString(""));

    if (useCSS)
        QCOMPARE(label->preferredLineCount(), -1);
    else
        QCOMPARE(label->preferredLineCount(), 0);

    // Set both model and CSS line count, and make sure that model overrides CSS
    label->setStyleName("lineCount3");
    label->setPreferredLineCount(1);
    label->resize(label->preferredSize());
    if (wrapMode == QTextOption::WrapAnywhere || richText)
        QCOMPARE(removeHtml(view->renderedText()), QString("AAA") + ellipsisChar);
    else
        QCOMPARE(removeHtml(view->renderedText()), QString("A B C"));
}
void Ut_MLabel::testSingleLineElidingWithWordwrap()
{
    //Test that a long single line elides the same whether or not wordwrap is enabled
    label->setTextElide(true);
    label->setText("12345677535345234523532545342553245234523");
    label->setMaximumWidth(label->preferredSize().width() / 2);
    label->resize(label->preferredSize());

    QImage imageWithoutWordWrap = captureImage(label);

    label->setWordWrap(true);
    QImage imageWithWordWrap = captureImage(label);

    QVERIFY(imageWithoutWordWrap == imageWithWordWrap);
}

void Ut_MLabel::testPaintOffsetNoAffectOnGeometry()
{
    MLabelView *view = const_cast<MLabelView*>(qobject_cast<const MLabelView*>(label->view()));
    MLabelStyle *style = const_cast<MLabelStyle*>(view->style().operator ->());
    style->setPaintOffset(QPointF());
    view->applyStyle();

    label->setText("Sample");
    qApp->processEvents();

    QCOMPARE(style->paintOffset(), QPointF());
    QRectF noOffsetGeometry = label->geometry();

    style->setPaintOffset(QPointF(1000, 1000));
    view->applyStyle();
    qApp->processEvents();

    QCOMPARE(style->paintOffset(), QPointF(1000,1000));
    QCOMPARE(noOffsetGeometry, label->geometry());
}

void Ut_MLabel::testRichTextPreferredLineCount_data()
{
    QTest::addColumn<QString>("twoLinesOfText");

    QTest::newRow("Richtext with <br>") << "<qt>Hello<br>There";
    QTest::newRow("Richtext with <p>") << "<qt>Hello<p>There";
    QTest::newRow("Richtext with <b>") << "<qt>Hello<p><b>There</b>";
    QTest::newRow("Richtext with <font size=20>") << "<qt>Hello<p><font size=20>There</font>";
    QTest::newRow("Richtext with mixed font sizes on single line 1") << "<qt>Hello<p><font size=8>There</font> <font size=30>big</font>";
}

void Ut_MLabel::testRichTextPreferredLineCount()
{
    QFETCH(QString, twoLinesOfText);
    label->setTextElide(true);
    label->setText(twoLinesOfText);
    qreal height = label->preferredHeight();

    label->setText(twoLinesOfText + "<p>lines that<p>should not be shown");
    label->setPreferredLineCount(2);
    QCOMPARE(height, label->preferredHeight());

    label->setTextElide(false);
    QCOMPARE(height, label->preferredHeight());
}
QTEST_APPLESS_MAIN(Ut_MLabel);
