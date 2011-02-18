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
#include <mtextedit.h>
#include <mrichtextedit.h>

//#define SHOW_DEBUG_IMAGE

Q_DECLARE_METATYPE(MWidgetController*)
MApplication *app;

Ut_AllWidgets::Ut_AllWidgets()
    : m_scene( 0 )
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_allwidgets" };
    app = new MApplication(argc, argv);
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_allwidgets.css");
    qRegisterMetaType<MWidgetController *>("MWidgetController *");
}

Ut_AllWidgets::~Ut_AllWidgets()
{
}

void Ut_AllWidgets::cleanupTestCase()
{
    delete app;
}

void Ut_AllWidgets::init()
{
    m_scene = new QGraphicsScene;
}

void Ut_AllWidgets::cleanup()
{
    delete m_scene; // Deletes the items in the scene, too!
    m_scene = 0;
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
    bool givenWarning = false;
    for(int y = 0; y < image.height(); ++y)
        for(int x = 0; x < image.width(); ++x) {
            if(pixmapBoundingRect.contains(x,y)) {
                if(!pixmapGeometry.contains(x,y) && image.pixel(x,y) != Qt::transparent) {
                    //It's not wrong to paint outside of your geometry but inside the bounding rect, but it is unusual
                    if(!givenWarning)
                        qWarning() << "Pixel at" << x << y << "is not transparent but geometry is" << pixmapGeometry;
                    if(strict) {
#ifdef SHOW_DEBUG_IMAGE
                        debugDisplayImage(image, 10);
#endif
                        return false;
                    }
                    givenWarning = true;
                }
            } else if(image.pixel(x,y) != Qt::transparent) {
                qWarning() << "Pixel at" << x << y << "is not transparent (" << QColor(image.pixel(x,y)) << ") but bounding rect is" << pixmapBoundingRect;
/*                {
                    QPainter painter(&image);
                    painter.setPen(Qt::red);
                    painter.drawEllipse(QPoint(x,y),5,5);
                }*/
#ifdef SHOW_DEBUG_IMAGE
                debugDisplayImage(image, 10);
#endif
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

#define TEST_WIDGET(description, widget) \
    QTest::newRow(description "(LTR)") << static_cast<MWidgetController*>(widget) << true << true << false << false; \
    QTest::newRow(description "(RTL)") << static_cast<MWidgetController*>(widget) << true << true << false << true;
#define TEST_WIDGET_HFW(description, widget) \
    QTest::newRow(description "(LTR)") << static_cast<MWidgetController*>(widget) << true << true << true << false; \
    QTest::newRow(description "(RTL)") << static_cast<MWidgetController*>(widget) << true << true << true << true;
#define TEST_WIDGET_SKIP_SMALL(description, widget) \
    QTest::newRow(description "(LTR)") << static_cast<MWidgetController*>(widget) << false << false << false << false; \
    QTest::newRow(description "(RTL)") << static_cast<MWidgetController*>(widget) << false << false << false << true;
#define TEST_WIDGET_HFW_SKIP_SMALL(description, widget) \
    QTest::newRow(description "(LTR)") << static_cast<MWidgetController*>(widget) << false << false << true << false; \
    QTest::newRow(description "(RTL)") << static_cast<MWidgetController*>(widget) << false << false << true << true;

//Create a crazy unicode string to stress test
#define CRAZYSTRING QString::fromUtf8("T̩̦̗̜̭̠͒͊ͬͮ̔ͮ̋͢ͅȞ̸̨͙̗͓̞̯̤͖͖̂ͦ̿ͥ̑̏̌ͤͭͬͧ̄ͭĮ̷̨̫͔̖͍͖̼̱̭̎̿̃̍̌ͧ͆͑ͧ̾̉͜͜S̢̡̨̥̮̩̥̞͙̺̤̦̰̺̮̝ͩ͗ͧͧ̑͗̾ͪ͒ͤ̚͢͝ ̸̔͆ͩ̈͌̓͢҉̢͚͕̯̤͙̼͟Į̵̳͕̭̭͎̩̙̺͇̇̄̃̌̋̎ͧ̍ͮ̕S͕̮͍̺̹̖̝̾̅͛̊ͬ́ͬ͋ͩ͠ ̨ͪͣͨ̈́̓͛̓͒͞͝҉͍̦̦̙̺̙͈̟̤S̢͖̺͔̘̘͉͇͍͚̣̻̺̬̝̰͚̖͎̗ͪͯ̇͂ͬ̐̍͋̈̽͋̿ͩ̌͘O̸͓̞͇̠̔̐͐͊͌ͮͮ̌ͩ͒ͦ͊̋ͥ̉́͟M̵̧̥̣̼̩̻̤̘̰̩͉ͯͫ̾̈͢͢Ẻ̍ͭ̀̾̂ͭ̊ͣ̃̓ͣ̂̓҉͕̘̰͙̥̼͙̲ ̢̰͓̫̹̣̱͈̺̔̅̽̀ͩ̀̏́̌̒̀́͠ͅC̵̢̛͉̬̼͕̺͍̻̞̼̙̻̼̯̺̙̖̲̋̋ͣ̉̅ͫͧ̓͂ͣ̓̐ͤ̃̑͋́͝R̐͑͛ͥ̆̇̓ͨ͋ͯͧ̚͏͙̘̭̲̗̭̹̮̱̥̬̜̗̻̹̬̲̀́́Aͫͥͯͤ̓̀̈ͤ͜҉̟͖̥̖̦̗͎̫̘̫̠̟ͅͅZ̏͗ͬ̇͆͊͆͑̄ͯͧ̈̋̐ͤ̚͞҉̸̜͖̬̞̙͕̼̞͚̖͓̭͓̮͖͔̟͜͞ͅY̅̊ͬͫͪ́ͤͮͯ͆̇ͫͪ̋͆͛ͧ͏̛͎̬̰̫̩̦̰̥̫͉̫̰̭͝ ̉ͬͤ̎͊̽̍̏̈́ͧ̓̊ͥ̊͋̉̚̕͏̘͍̪̬̞͙̥̝̖͓͖Ț̨̛̹͔͔̳̖̣͉͔͍ͯ͊̂͗̔̈̋͂͑̇ͩ͒̓̄̍̃̋̕͢E̵͚͕̞̮͙̖͂̃̍̄̈́͊́ͣ̄͌͒ͭͬ͆͂͘͘͟͞ͅX̵̢̝̲̖̲̤͙̀̿̒̔ͬ̓̇ͤT̍̽̇͑҉҉̙̗̟͈̠̺̖̮̼̦̘̠́ͅ")
 
#define DATA_FUNCTION_FOR_ALL_WIDGETS(functionName) \
void Ut_AllWidgets::functionName##_data() \
{ \
    QTest::addColumn<MWidgetController*>("widget"); \
    QTest::addColumn<bool>("strict"); /* Make warnings into errors. */ \
    QTest::addColumn<bool>("testSmallSize"); /* Whether to test at small sizes */ \
    QTest::addColumn<bool>("heightForWidth"); \
    QTest::addColumn<bool>("leftToRight"); \
\
    TEST_WIDGET_HFW("MLabel - simple", new MLabel("Hello there"));\
    TEST_WIDGET_HFW("MLabel - simple multiline", new MLabel("Hello there\nHow are you?"));\
    TEST_WIDGET_HFW("MLabel - rich", new MLabel("<qt><b>Hello</b> there"));\
/*    TEST_WIDGET_HFW("MLabel - crazy", new MLabel(CRAZYSTRING)); */ \
/*    TEST_WIDGET_HFW("MLabel - rich crazy", new MLabel("<qt><b>" + CRAZYSTRING)); */ \
    TEST_WIDGET("MButton - simple", new MButton("Hello there")); \
    TEST_WIDGET("MButton - simple multiline", new MButton("Hello there\nHow are you?")); \
    TEST_WIDGET("MButton - rich", new MButton("<qt><b>Hello</b> there")); \
/*    TEST_WIDGET("MButton - crazy", new MButton(CRAZYSTRING));*/ \
/*    TEST_WIDGET("MButton - rich crazy", new MButton("<qt><b>" + CRAZYSTRING)); */ \
    TEST_WIDGET("MSlider", new MSlider); \
    TEST_WIDGET_HFW_SKIP_SMALL("MBubbleItem", new MBubbleItem); /* MBubbleItem has lots of assumptions on how big it is at minimum */ \
    TEST_WIDGET("MTextEdit - simple", new MTextEdit(MTextEditModel::SingleLine, "Hello there, how are you?")); \
    TEST_WIDGET_HFW("MTextEdit - simple multiline", new MTextEdit(MTextEditModel::MultiLine, "Hello there\nHow are you?")); \
    TEST_WIDGET_HFW("MTextEdit - rich", new MTextEdit(MTextEditModel::MultiLine, "<qt><b>Hello</b> there")); \
/*    TEST_WIDGET("MTextEdit - crazy", new MTextEdit(MTextEditModel::SingleLine, CRAZYSTRING)); */ \
/*    TEST_WIDGET("MTextEdit - rich crazy", new MTextEdit(MTextEditModel::SingleLine, "<qt><b>" + CRAZYSTRING)); */ \
    TEST_WIDGET("MRichTextEdit - simple", new MRichTextEdit(MTextEditModel::SingleLine, "Hello there, how are you?")); \
    TEST_WIDGET_HFW("MRichTextEdit - simple multiline", new MRichTextEdit(MTextEditModel::MultiLine, "Hello there\nHow are you?")); \
    TEST_WIDGET_HFW("MRichTextEdit - rich", new MRichTextEdit(MTextEditModel::MultiLine, "<qt><b>Hello</b> there")); \
/*    TEST_WIDGET("MRichTextEdit - crazy", new MRichTextEdit(MTextEditModel::SingleLine, CRAZYSTRING)); */ \
/*    TEST_WIDGET("MRichTextEdit - rich crazy", new MRichTextEdit(MTextEditModel::SingleLine, "<qt><b>" + CRAZYSTRING)); */ \
}

#define SETUP_WIDGET(widget) \
    QFETCH(MWidgetController *, widget); \
    QFETCH(bool, leftToRight); \
    widget->setLayoutDirection(leftToRight?Qt::LeftToRight:Qt::RightToLeft); \
    m_scene->addItem(widget); \
    widget->style();

DATA_FUNCTION_FOR_ALL_WIDGETS(testAllWidgets)

void Ut_AllWidgets::testAllWidgets()
{
    SETUP_WIDGET(widget);

    QFETCH(bool, strict);
    QFETCH(bool, testSmallSize);

    widget->setMinimumSize(0,0);
    widget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    static_cast<MWidget *>(widget)->setGeometry(100,100,100,100);

    QVERIFY(checkChildrenBoundingRect(widget) || !strict);
    QVERIFY(paintAndTestWidget(widget, strict));

    if(testSmallSize) {
        //Now resize to just 1,1  and make sure it paints at maximum only 1 pixel
        static_cast<MWidget *>(widget)->setGeometry(100,100,1,1);

        QVERIFY(checkChildrenBoundingRect(widget) || !strict);
        QVERIFY(paintAndTestWidget(widget, strict));

        //Now resize to 0,0  and make sure it paints nothing at all
        static_cast<MWidget *>(widget)->setGeometry(100,100,0,0);

        QVERIFY(checkChildrenBoundingRect(widget) || !strict);
        QVERIFY(paintAndTestWidget(widget, strict));
    }

    // Check that the CSS values override
    widget->setObjectName("cssWidth");
    QCOMPARE(widget->minimumSize(), QSizeF(0,0));
    QCOMPARE(widget->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    widget->setMinimumSize(-1,-1);
    widget->setMaximumSize(-1,-1);
    QCOMPARE(widget->minimumSize().width() - widget->style()->marginLeft() - widget->style()->marginRight(), 9.0); //The values in CSS should now take precedence
    QCOMPARE(widget->maximumSize().width() -  widget->style()->marginLeft() - widget->style()->marginRight(), 11.0);
  
}

DATA_FUNCTION_FOR_ALL_WIDGETS(testMemoryLeaks)
void Ut_AllWidgets::testMemoryLeaks()
{
    SETUP_WIDGET(widget);

    // Test for memory leaks by checking that the view, model and all children are deleted
    QList< QPointer<QGraphicsObject> > subwidgets = getAllSubWidgets(widget);
    foreach(const QPointer<QGraphicsObject> &subwidget, subwidgets) {
        QVERIFY(!widget->parent()); // A subwidget has QObject::parent set - it is better to keep this NULL and let QGraphicsItem::parentItem() delete the object
        QGraphicsItem *parent = subwidget;
        while(parent != widget) {
            QVERIFY(parent); // A subwidget does not have parentItem pointing to the widget
            parent = parent->parentWidget();
        };
    }
    QPointer<MWidgetView> view = const_cast<MWidgetView*>(widget->view());
    QPointer<MWidgetModel> model = widget->model();
    delete widget;
    foreach(const QPointer<QGraphicsObject> subwidget, subwidgets) {
        QVERIFY(!subwidget);
    }

    QVERIFY(!view);
    QVERIFY(!model);
}

DATA_FUNCTION_FOR_ALL_WIDGETS(testSizeHints)
void Ut_AllWidgets::testSizeHints()
{
    SETUP_WIDGET(widget);
    QFETCH(bool, heightForWidth);

    QCOMPARE(widget->sizePolicy().hasHeightForWidth(), heightForWidth);

    widget->setObjectName("noCssSizes");
    QSizeF preferredSize = widget->preferredSize();
    QSizeF minimumSize = widget->minimumSize();
    QSizeF maximumSize = widget->maximumSize();

    QSizeF preferredSize100 = widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1));
    QSizeF preferredSize200 = widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1));
    QSizeF preferredSize10 = widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1));
    QSizeF minimumSize100 = widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1));
    QSizeF minimumSize200 = widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1));
    QSizeF minimumSize10 = widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1));
    QSizeF maximumSize100 = widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1));
    QSizeF maximumSize200 = widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1));
    QSizeF maximumSize10 = widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1));

    if(!widget->sizePolicy().hasHeightForWidth()) {
        //Check that our height does not depend on our width
        QCOMPARE(preferredSize.height(), preferredSize200.height());
        QCOMPARE(preferredSize.height(), preferredSize100.height());
        QCOMPARE(preferredSize.height(), preferredSize10.height());
        QCOMPARE(minimumSize.height(), minimumSize200.height());
        QCOMPARE(minimumSize.height(), minimumSize100.height());
        QCOMPARE(minimumSize.height(), minimumSize10.height());
        QCOMPARE(maximumSize.height(), maximumSize200.height());
        QCOMPARE(maximumSize.height(), maximumSize100.height());
        QCOMPARE(maximumSize.height(), maximumSize10.height());
    }

    //Force a refresh of the sizehint.  This should not affect anything
    widget->updateGeometry();

    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), preferredSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), preferredSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1)), preferredSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1)), minimumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1)), minimumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1)), minimumSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1)), maximumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1)), maximumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1)), maximumSize10);

    widget->resize(200, 200);

    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), preferredSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), preferredSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1)), preferredSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1)), minimumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1)), minimumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1)), minimumSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1)), maximumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1)), maximumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1)), maximumSize10);

    //Force a refresh of the sizehint.  This should not affect anything
    widget->updateGeometry();

    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), preferredSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), preferredSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1)), preferredSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1)), minimumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1)), minimumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1)), minimumSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1)), maximumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1)), maximumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1)), maximumSize10);

    widget->resize(10, 20);

    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), preferredSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), preferredSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1)), preferredSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1)), minimumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1)), minimumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1)), minimumSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1)), maximumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1)), maximumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1)), maximumSize10);

    //Force a refresh of the sizehint.  This should not affect anything
    widget->updateGeometry();

    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(100,-1)), preferredSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(200,-1)), preferredSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10,-1)), preferredSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(100,-1)), minimumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(200,-1)), minimumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MinimumSize, QSizeF(10,-1)), minimumSize10);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(100,-1)), maximumSize100);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(200,-1)), maximumSize200);
    QCOMPARE(widget->effectiveSizeHint(Qt::MaximumSize, QSizeF(10,-1)), maximumSize10);

    widget->setMinimumSize(0,0);
    widget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    widget->setPreferredWidth(10);
    QCOMPARE(widget->preferredSize(), preferredSize10);
    widget->setPreferredWidth(100);
    QCOMPARE(widget->preferredSize(), preferredSize100);
    widget->setPreferredWidth(200);
    QCOMPARE(widget->preferredSize(), preferredSize200);

    QSizeF preferredSizeCSS = widget->effectiveSizeHint(Qt::PreferredSize, QSizeF(10 + widget->style()->marginLeft() + widget->style()->marginRight(),-1));

    widget->setObjectName("cssWidth");
    QCOMPARE(widget->minimumSize(), QSizeF(0,0));
    QCOMPARE(widget->maximumSize(), QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    QCOMPARE(preferredSize200, widget->preferredSize());
    widget->setMinimumSize(-1,-1);
    widget->setMaximumSize(-1,-1);
    widget->setPreferredWidth(-1);

    //Preferred width is now read from the CSS, which sets the preferred width to 10
    QCOMPARE(widget->minimumSize().width() - widget->style()->marginLeft() - widget->style()->marginRight(), 9.0); //The values in CSS should now take precedence
    QCOMPARE(widget->maximumSize().width() -  widget->style()->marginLeft() - widget->style()->marginRight(), 11.0);
    QCOMPARE(widget->preferredSize(), preferredSizeCSS);

    //Force a refresh of the sizehint.  This should not affect anything
    widget->updateGeometry();

    //Preferred width is now read from the CSS, which sets the preferred width to 10
    QCOMPARE(widget->preferredSize(), preferredSizeCSS);
}

QTEST_APPLESS_MAIN(Ut_AllWidgets)
