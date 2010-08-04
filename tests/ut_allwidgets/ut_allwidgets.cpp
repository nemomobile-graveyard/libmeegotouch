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

#include "ut_allwidgets.h"
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsScene>
#include <QtGui/QLabel>
#include <QtGui/QWindowsStyle>

//Gain access to the view..
#define protected public
#include <mwidgetcontroller.h>
#undef protected

#include <mapplication.h>
#include <mlabel.h>
#include <mbutton.h>
#include <mtheme.h>
#include <mwidgetview.h>
#include <mbubbleitem.h>
#include <mslider.h>

Q_DECLARE_METATYPE(MWidgetController*)
MApplication *app;

Ut_AllWidgets::Ut_AllWidgets()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_allwidgets" };
    app = new MApplication(argc, argv);
    m_scene = new QGraphicsScene;
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_allwidgets.css");
    qRegisterMetaType<MWidgetController *>("MWidgetController *");
}

Ut_AllWidgets::~Ut_AllWidgets()
{
}

void Ut_AllWidgets::cleanupTestCase()
{
    delete m_scene; // Deletes the items in the scene, too!
    m_scene = 0;
    delete app;
}

void Ut_AllWidgets::init()
{
}

void Ut_AllWidgets::cleanup()
{
}

void Ut_AllWidgets::debugDisplayImage(const QImage &image, int seconds)
{
    debugDisplayPixmap(QPixmap::fromImage(image), seconds);
}
void Ut_AllWidgets::debugDisplayPixmap(const QPixmap &pixmap, int seconds)
{
    QLabel *label = new QLabel;
    label->setStyle(new QWindowsStyle);
    label->setPixmap(pixmap);
    label->show();
    QTest::qWait(seconds*1000);
    delete label;
}

bool Ut_AllWidgets::paintAndTestWidget(QGraphicsWidget *widget, bool strict)
{
    //Because of the async nature, make sure we've got the pixmaps
    app->processEvents();
    while (MTheme::instance()->hasPendingRequests()) {
        QTest::qWait(100);
        app->processEvents();
    }

    //Create a pixmap big enough for the widget plus room to check if it paints outside of its bounding box
    QRectF boundingRect = widget->mapToScene(widget->boundingRect()).boundingRect();
    if(!(widget->flags() & QGraphicsItem::ItemClipsChildrenToShape))
           boundingRect |= widget->mapToScene(widget->childrenBoundingRect()).boundingRect();
    QSize size =  boundingRect.size().toSize();

    QImage image(size + QSize(200,200), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPointF offset = QPointF(100,100) - boundingRect.topLeft();
    QRectF pixmapBoundingRect = boundingRect.translated(offset);
    QRectF pixmapGeometry = widget->geometry().translated(offset);

    {
        QPainter painter(&image);
        m_scene->render(&painter, QRectF(0,0,image.width(), image.height()), boundingRect.adjusted(-100,-100,100,100));
        //Draw in the bounding rect and geometry, for showing to the user
        if(pixmapBoundingRect.adjusted(0,0,-1,-1).isValid()) {
            painter.setPen(Qt::red);
            painter.drawRect(pixmapBoundingRect.adjusted(0,0,-1,-1));
        }
        if(pixmapGeometry.adjusted(0,0,-1,-1).isValid()) {
            painter.setPen(Qt::blue);
            painter.drawRect(pixmapGeometry.adjusted(0,0,-1,-1));
        }
    }

    //Now check that we haven't painted outside of the bounding box
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x) {
            if(pixmapBoundingRect.contains(x,y)) {
                if(!pixmapGeometry.contains(x,y) && image.pixel(x,y) != Qt::transparent) {
                    //It's not wrong to paint outside of your geometry but inside the bounding rect, but it is unusual
                    qWarning() << "Pixel at" << x << y << "is not transparent but geometry is" << pixmapGeometry;
                    if(strict) {
                        debugDisplayImage(image, 10);
                        return false;
                    }
                }
            } else if(image.pixel(x,y) != Qt::transparent) {
                qWarning() << "Pixel at" << x << y << "is not transparent (" << QColor(image.pixel(x,y)) << ") but bounding rect is" << pixmapBoundingRect;
/*                {
                    QPainter painter(&image);
                    painter.setPen(Qt::red);
                    painter.drawEllipse(QPoint(x,y),5,5);
                }*/
                debugDisplayImage(image, 10);
                return false;
            }
        }
    // Uncomment to see what the widget looks like:
    // debugDisplayImage(image, 3);
    return true;
}
bool Ut_AllWidgets::checkChildrenBoundingRect(const QGraphicsWidget *widget) const
{
    //Make sure the widget has a chance to relayout if it needs to
    app->processEvents();
    // Although it is legal to have children outside the parent's geometry, I think we should
    // have a policy to not allow it
    QRectF childrenBoundingRect = widget->childrenBoundingRect();
    if(!childrenBoundingRect.isEmpty() && !widget->boundingRect().contains(childrenBoundingRect)) {
        // If this flag has been set, then it's okay to have children outside
        if(widget->flags() & QGraphicsItem::ItemClipsChildrenToShape)
            return true;
        qWarning() << "The widget's children are outside of the parent's bounding rect.  Children:" << childrenBoundingRect << "Parent:" << widget->boundingRect();
        return false;
    }
    return true;
}
QList< QPointer<QGraphicsObject> > Ut_AllWidgets::getAllLayoutItemObjects(QGraphicsLayoutItem *item)
{
    QList< QPointer<QGraphicsObject> > children;
    if(!item)
        return children;

    QGraphicsObject* object = dynamic_cast<QGraphicsObject *>(item->graphicsItem());
    if(object)
        children << object;
    if(item->isLayout()) {
        QGraphicsLayout *layout = static_cast<QGraphicsLayout *>(item);
        for(int i = 0 ; i < layout->count(); i++)
            children << getAllLayoutItemObjects(layout->itemAt(i));
    }

    return children; 
}
QList< QPointer<QGraphicsObject> > Ut_AllWidgets::getAllSubWidgets(QGraphicsItem *item)
{
    QList< QPointer<QGraphicsObject> > children;
    foreach(QGraphicsItem *child, item->childItems()) {
        QGraphicsObject *object = child->toGraphicsObject();
        if(object)
            children << object;
        children << getAllSubWidgets(child);
    }

    if(item->isWidget())
        children << getAllLayoutItemObjects( static_cast<QGraphicsWidget *>(item)->layout() );

    return children;
}

bool Ut_AllWidgets::checkSubWidgets(MWidgetController * parentWidget, QList< QPointer<QGraphicsObject> > subwidgets) {
    foreach(const QPointer<QGraphicsObject> &widget, subwidgets) {
        if(widget->parent()) {
            qWarning() << "A subwidget has QObject::parent set - it is better to keep this NULL and let QGraphicsItem::parentItem() delete the object";
            return false;
        }
        QGraphicsItem *parent = widget;
        while(parent != parentWidget) {
            if(!parent) {
                qDebug() << "A subwidget does not have parentItem pointing to the widget.";
                return false;
            }
            parent = parent->parentWidget();
        };
    }
    return true;
}
void Ut_AllWidgets::testAllWidgets_data()
{
    QTest::addColumn<MWidgetController*>("widget");
    QTest::addColumn<bool>("strict"); //Make warnings into errors.
    QTest::addColumn<bool>("leftToRight");

#define TEST_WIDGET(description, widget, strict) \
    QTest::newRow(description "(LTR)") << static_cast<MWidgetController*>(widget) << (strict) << false; \
    QTest::newRow(description "(RTL)") << static_cast<MWidgetController*>(widget) << (strict) << true;

    TEST_WIDGET("MLabel - simple", new MLabel("Hello there"), true);
    TEST_WIDGET("MLabel - simple multiline", new MLabel("Hello there\nHow are you?"), true);
    TEST_WIDGET("MLabel - rich", new MLabel("<qt><b>Hello</b> there"), true);
    TEST_WIDGET("MLabel - crazy", new MLabel(QString::fromUtf8("T̩̦̗̜̭̠͒͊ͬͮ̔ͮ̋͢ͅȞ̸̨͙̗͓̞̯̤͖͖̂ͦ̿ͥ̑̏̌ͤͭͬͧ̄ͭĮ̷̨̫͔̖͍͖̼̱̭̎̿̃̍̌ͧ͆͑ͧ̾̉͜͜S̢̡̨̥̮̩̥̞͙̺̤̦̰̺̮̝ͩ͗ͧͧ̑͗̾ͪ͒ͤ̚͢͝ ̸̔͆ͩ̈͌̓͢҉̢͚͕̯̤͙̼͟Į̵̳͕̭̭͎̩̙̺͇̇̄̃̌̋̎ͧ̍ͮ̕S͕̮͍̺̹̖̝̾̅͛̊ͬ́ͬ͋ͩ͠ ̨ͪͣͨ̈́̓͛̓͒͞͝҉͍̦̦̙̺̙͈̟̤S̢͖̺͔̘̘͉͇͍͚̣̻̺̬̝̰͚̖͎̗ͪͯ̇͂ͬ̐̍͋̈̽͋̿ͩ̌͘O̸͓̞͇̠̔̐͐͊͌ͮͮ̌ͩ͒ͦ͊̋ͥ̉́͟M̵̧̥̣̼̩̻̤̘̰̩͉ͯͫ̾̈͢͢Ẻ̍ͭ̀̾̂ͭ̊ͣ̃̓ͣ̂̓҉͕̘̰͙̥̼͙̲ ̢̰͓̫̹̣̱͈̺̔̅̽̀ͩ̀̏́̌̒̀́͠ͅC̵̢̛͉̬̼͕̺͍̻̞̼̙̻̼̯̺̙̖̲̋̋ͣ̉̅ͫͧ̓͂ͣ̓̐ͤ̃̑͋́͝R̐͑͛ͥ̆̇̓ͨ͋ͯͧ̚͏͙̘̭̲̗̭̹̮̱̥̬̜̗̻̹̬̲̀́́Aͫͥͯͤ̓̀̈ͤ͜҉̟͖̥̖̦̗͎̫̘̫̠̟ͅͅZ̏͗ͬ̇͆͊͆͑̄ͯͧ̈̋̐ͤ̚͞҉̸̜͖̬̞̙͕̼̞͚̖͓̭͓̮͖͔̟͜͞ͅY̅̊ͬͫͪ́ͤͮͯ͆̇ͫͪ̋͆͛ͧ͏̛͎̬̰̫̩̦̰̥̫͉̫̰̭͝ ̉ͬͤ̎͊̽̍̏̈́ͧ̓̊ͥ̊͋̉̚̕͏̘͍̪̬̞͙̥̝̖͓͖Ț̨̛̹͔͔̳̖̣͉͔͍ͯ͊̂͗̔̈̋͂͑̇ͩ͒̓̄̍̃̋̕͢E̵͚͕̞̮͙̖͂̃̍̄̈́͊́ͣ̄͌͒ͭͬ͆͂͘͘͟͞ͅX̵̢̝̲̖̲̤͙̀̿̒̔ͬ̓̇ͤT̍̽̇͑҉҉̙̗̟͈̠̺̖̮̼̦̘̠́ͅ")), true);
    TEST_WIDGET("MButton - simple", new MButton("Hello there"), true);
    TEST_WIDGET("MButton - simple multiline", new MButton("Hello there\nHow are you?"), true);
    TEST_WIDGET("MButton - rich", new MButton("<qt><b>Hello</b> there"), true);
    TEST_WIDGET("MButton - crazy", new MButton(QString::fromUtf8("T̩̦̗̜̭̠͒͊ͬͮ̔ͮ̋͢ͅȞ̸̨͙̗͓̞̯̤͖͖̂ͦ̿ͥ̑̏̌ͤͭͬͧ̄ͭĮ̷̨̫͔̖͍͖̼̱̭̎̿̃̍̌ͧ͆͑ͧ̾̉͜͜S̢̡̨̥̮̩̥̞͙̺̤̦̰̺̮̝ͩ͗ͧͧ̑͗̾ͪ͒ͤ̚͢͝ ̸̔͆ͩ̈͌̓͢҉̢͚͕̯̤͙̼͟Į̵̳͕̭̭͎̩̙̺͇̇̄̃̌̋̎ͧ̍ͮ̕S͕̮͍̺̹̖̝̾̅͛̊ͬ́ͬ͋ͩ͠ ̨ͪͣͨ̈́̓͛̓͒͞͝҉͍̦̦̙̺̙͈̟̤S̢͖̺͔̘̘͉͇͍͚̣̻̺̬̝̰͚̖͎̗ͪͯ̇͂ͬ̐̍͋̈̽͋̿ͩ̌͘O̸͓̞͇̠̔̐͐͊͌ͮͮ̌ͩ͒ͦ͊̋ͥ̉́͟M̵̧̥̣̼̩̻̤̘̰̩͉ͯͫ̾̈͢͢Ẻ̍ͭ̀̾̂ͭ̊ͣ̃̓ͣ̂̓҉͕̘̰͙̥̼͙̲ ̢̰͓̫̹̣̱͈̺̔̅̽̀ͩ̀̏́̌̒̀́͠ͅC̵̢̛͉̬̼͕̺͍̻̞̼̙̻̼̯̺̙̖̲̋̋ͣ̉̅ͫͧ̓͂ͣ̓̐ͤ̃̑͋́͝R̐͑͛ͥ̆̇̓ͨ͋ͯͧ̚͏͙̘̭̲̗̭̹̮̱̥̬̜̗̻̹̬̲̀́́Aͫͥͯͤ̓̀̈ͤ͜҉̟͖̥̖̦̗͎̫̘̫̠̟ͅͅZ̏͗ͬ̇͆͊͆͑̄ͯͧ̈̋̐ͤ̚͞҉̸̜͖̬̞̙͕̼̞͚̖͓̭͓̮͖͔̟͜͞ͅY̅̊ͬͫͪ́ͤͮͯ͆̇ͫͪ̋͆͛ͧ͏̛͎̬̰̫̩̦̰̥̫͉̫̰̭͝ ̉ͬͤ̎͊̽̍̏̈́ͧ̓̊ͥ̊͋̉̚̕͏̘͍̪̬̞͙̥̝̖͓͖Ț̨̛̹͔͔̳̖̣͉͔͍ͯ͊̂͗̔̈̋͂͑̇ͩ͒̓̄̍̃̋̕͢E̵͚͕̞̮͙̖͂̃̍̄̈́͊́ͣ̄͌͒ͭͬ͆͂͘͘͟͞ͅX̵̢̝̲̖̲̤͙̀̿̒̔ͬ̓̇ͤT̍̽̇͑҉҉̙̗̟͈̠̺̖̮̼̦̘̠́ͅ")), true);
    TEST_WIDGET("MSlider", new MSlider, true);
    TEST_WIDGET("MBubbleItem", new MBubbleItem, false);
}


void Ut_AllWidgets::testAllWidgets()
{
    QFETCH(MWidgetController *, widget);
    QFETCH(bool, strict);
    QFETCH(bool, leftToRight);
    widget->setLayoutDirection(leftToRight?Qt::LeftToRight:Qt::RightToLeft);

    m_scene->addItem(widget);
    widget->style();

    widget->setMinimumSize(0,0);
    widget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    static_cast<MWidget *>(widget)->setGeometry(100,100,100,100);

    QVERIFY(checkChildrenBoundingRect(widget) || !strict);
    QVERIFY(paintAndTestWidget(widget, strict));

    //Now resize to just 1,1  and make sure it paints at maximum only 1 pixel
    static_cast<MWidget *>(widget)->setGeometry(100,100,1,1);

    QVERIFY(checkChildrenBoundingRect(widget) || !strict);
    QVERIFY(paintAndTestWidget(widget, strict));

    //Now resize to 0,0  and make sure it paints nothing at all
    static_cast<MWidget *>(widget)->setGeometry(100,100,0,0);

    QVERIFY(checkChildrenBoundingRect(widget) || !strict);
    QVERIFY(paintAndTestWidget(widget, strict));

    // Check that the CSS values override
    widget->setObjectName("cssWidth");
    QCOMPARE(widget->minimumSize(), QSizeF(0,0));
    QCOMPARE(widget->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    widget->setMinimumSize(-1,-1);
    widget->setMaximumSize(-1,-1);
    QCOMPARE(widget->minimumSize().width() - widget->style()->marginLeft() - widget->style()->marginRight(), 9.0); //The values in CSS should now take precedence
    QCOMPARE(widget->maximumSize().width() -  widget->style()->marginLeft() - widget->style()->marginRight(), 11.0);
  
    // Test for memory leaks by checking that the view, model and all children are deleted
    QList< QPointer<QGraphicsObject> > subwidgets = getAllSubWidgets(widget);
    QVERIFY(checkSubWidgets(widget, subwidgets));
    QPointer<MWidgetView> view = const_cast<MWidgetView*>(widget->view());
    QPointer<MWidgetModel> model = widget->model();
    delete widget;
    foreach(const QPointer<QGraphicsObject> subwidget, subwidgets) {
        QVERIFY(!subwidget);
    }

    QVERIFY(!view);
    QVERIFY(!model);
}


QTEST_APPLESS_MAIN(Ut_AllWidgets)
