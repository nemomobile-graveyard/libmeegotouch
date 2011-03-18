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

#include "ft_theme.h"
#include "mtheme.h"
#include <QtTest>
#include <MLibrary>
#include <MWidgetController>
#include <MWidgetView>
#include <MTheme>
#include <QTextStream>
#include <MComponentCache>

#include "mcomponentdata.h"
#include "../../src/corelib/theme/mtheme_p.h"
#include "testthemedaemon.h"
#include "ft_theme_data.h"

#ifdef HAVE_GCONF
static QString languageGConfValue = "en";
static QString targetGConfValue = "Default";
QVariant MGConfItem::value() const
{
    QString rv = "";
    if( key() == "/meegotouch/i18n/language" )
        rv = languageGConfValue;
    else if(key() == "/meegotouch/target/name")
        rv = targetGConfValue;
    qWarning() << "MGConfItem::value()" << key() << rv;
    return rv;
}
#endif

void removeDirectoryRecursive(const QString &path)
{
    QDir root(path);
    if (root.exists()) {
        QFileInfoList entries = root.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        for (int i = 0; i < entries.count(); i++) {
            if (entries[i].isDir()) {
                removeDirectoryRecursive(entries[i].filePath());
                root.rmdir(entries[i].absoluteFilePath());
            } else {
                root.remove(entries[i].absoluteFilePath());
            }
        }
    }
}

void makeFile(const QString &path, const QString &content)
{
    QFile file(QDir::tempPath() + QDir::separator() + QString("Ft_Theme") + QDir::separator() + path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << content;
        file.close();
    } else {
        qWarning() << "Failed to create file:" << path;
    }
}

void Ft_Theme::init()
{
    QStringList paths;
    paths << QString("Ft_Theme") + QDir::separator()
      + "theme1" + QDir::separator()
      + "meegotouch" + QDir::separator()
      + "libmeegotouchcore" + QDir::separator()
      + "style";
    paths << QString("Ft_Theme") + QDir::separator()
      + "theme2" + QDir::separator()
      + "meegotouch" + QDir::separator()
      + "libmeegotouchcore" + QDir::separator()
      + "style";
    paths << QString("Ft_Theme") + QDir::separator()
      + "theme2" + QDir::separator()
      + "meegotouch" + QDir::separator()
      + "Ft_Theme" + QDir::separator()
      + "style";
    paths << QString("Ft_Theme") + QDir::separator()
      + "theme1" + QDir::separator()
      + "meegotouch" + QDir::separator()
      + "locale" + QDir::separator()
      + "fi";

    foreach(QString path, paths) {
        // create test theme to tmp/
        if (!QDir::temp().mkpath(path))
            qWarning() << "Failed to create path:" << path;
    }

    // create constant declarations
    makeFile(QString("theme1") + QDir::separator() + QString("meegotouch") + QDir::separator() + QString("constants.ini"), THEME_1_CONSTANTS_INI);
    makeFile(QString("theme2") + QDir::separator() + QString("meegotouch") + QDir::separator() + QString("constants.ini"), THEME_2_CONSTANTS_INI);

    makeFile(QString("theme2") + QDir::separator() + QString("meegotouch") + QDir::separator() +  QString("locale") + QDir::separator() + QString("fi") + QDir::separator() + QString("constants.ini"), FINNISH_CONSTANTS_INI);

    // create view configuration files
    makeFile( QString("theme1") + QDir::separator()
	      + QString("meegotouch") + QDir::separator()
	      + QString("libmeegotouchcore") + QDir::separator()
	      + QString("libmeegotouchcore.conf"), LIBM_THEME_1_VIEW_CONFIGURATION);

    makeFile( QString("theme2") + QDir::separator()
	      + QString("meegotouch") + QDir::separator()
	      + QString("libmeegotouchcore") + QDir::separator()
	      + QString("libmeegotouchcore.conf"), LIBM_THEME_2_VIEW_CONFIGURATION);
    makeFile( QString("theme2") + QDir::separator()
	      + QString("meegotouch") + QDir::separator()
	      + QString("Ft_Theme") + QDir::separator()
	      + QString("Ft_Theme.conf"), TESTAPP_VIEW_CONFIGURATION);

    // create stylesheets
    makeFile( QString("theme1") + QDir::separator()
	      + QString("meegotouch") + QDir::separator()
	      + QString("libmeegotouchcore") + QDir::separator()
	      + QString("style") + QDir::separator()
	      + QString("libmeegotouchcore.css"), THEME_1_LIBM_CSS);

    makeFile( QString("theme2") + QDir::separator()
	      + QString("meegotouch") + QDir::separator()
	      + QString("libmeegotouchcore") + QDir::separator()
	      + QString("style") + QDir::separator()
	      + QString("libmeegotouchcore.css"), THEME_2_LIBM_CSS);

    makeFile(QString("theme2") + QDir::separator()
	     + QString("meegotouch") + QDir::separator()
	     + QString("Ft_Theme") + QDir::separator()
	     + QString("style") + QDir::separator()
	     + QString("Ft_Theme.css"), THEME_2_APP_CSS);
}

void Ft_Theme::cleanup()
{
    // remove test theme from tmp/
    removeDirectoryRecursive(QDir::tempPath() + QDir::separator() + "Ft_Theme");
    QDir::temp().rmdir("Ft_Theme");
}

void Ft_Theme::initTestCase()
{
    // MComponentCache creates MApplication. That allows us to test 
    // theme reinit with a new application name (first test case)
    MComponentCache::populateForMApplication();

    // ComponentData is created in populateForMApplication()
    componentData = MComponentData::instance();

    // store the "original" themedaemon
    daemon = MTheme::instance()->d_ptr->themeDaemon;

    // replace themedaemon with our own implementation
    testDaemon = new TestThemeDaemon;
    MTheme::instance()->d_ptr->themeDaemon = testDaemon;
    // a bit hackish, but connect the signals
    connect(testDaemon, SIGNAL(themeChanged(QStringList, QStringList)),
            MTheme::instance(), SLOT(themeChangedSlot(QStringList, QStringList)));

    connect(testDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)),
            MTheme::instance(), SLOT(pixmapCreatedOrChangedSlot(QString, QSize, MPixmapHandle)));
}

void Ft_Theme::cleanupTestCase()
{
    // restore the original daemon we replaced
    MTheme::instance()->d_ptr->themeDaemon = daemon;
    delete testDaemon;
}

void Ft_Theme::testThemeReinit()
{
    QSignalSpy themeReinitedSpy(testDaemon, SIGNAL(themeReinited()));
    static int argc = 1;
    static char *argv[ 1 ];
    argv[ 0 ] = (char*)"./Ft_Theme";
    QCOMPARE(themeReinitedSpy.count(), 0);
    MComponentCache::mApplication(argc, argv);
    QCOMPARE(themeReinitedSpy.count(), 1);
}

void Ft_Theme::testViews_data()
{
    QTest::addColumn<QString>("theme");
    QTest::addColumn<QString>("viewType");
    QTest::addColumn<QString>("viewClass");

    QTest::newRow("m library, theme 1") << "theme 1" << "" << "MWidgetView";
    QTest::newRow("application, theme 2") << "theme 2" << "" << "TestView";
    QTest::newRow("m library, theme 2") << "theme 2" << "test" << "TestView2";
}

void Ft_Theme::testViews()
{
    QFETCH(QString, theme);
    QFETCH(QString, viewType);
    QFETCH(QString, viewClass);

    testDaemon->changeTheme(theme);
    testDaemon->emitThemeChange();

    MWidgetController widget;
    widget.setViewType(viewType);

    // Create view for a simple widget
    MWidgetView *view = MTheme::view(&widget);

    // Make sure that we got view at the first place
    if (!viewClass.isEmpty()) {
        QVERIFY(view);
        // Make sure it's correct type
        QCOMPARE(QString(view->metaObject()->className()), viewClass);
    }
}

void Ft_Theme::testStyles_data()
{
    QTest::addColumn<QString>("theme");
    QTest::addColumn<QString>("styleClass");
    QTest::addColumn<QColor>("backgroundColor");

    QTest::newRow("test library, theme 1") << "theme 1" << "MWidgetStyle" << QColor("#ffffff");
    QTest::newRow("test library, theme 2 (custom css)") << "theme 2" << "MWidgetStyle" << QColor("#f0f0f0");
}

void Ft_Theme::testStyles()
{
    QFETCH(QString, theme);
    QFETCH(QString, styleClass);
    QFETCH(QColor, backgroundColor);

    testDaemon->changeTheme(theme);
    testDaemon->emitThemeChange();

    const MWidgetStyle *style = dynamic_cast<const MWidgetStyle *>(MTheme::style(styleClass.toStdString().c_str(), QString()));
    QVERIFY(style);

    QCOMPARE(style->backgroundColor(), backgroundColor);

    MTheme::releaseStyle(style);
}

void Ft_Theme::testPixmaps_data()
{
    QTest::addColumn<QStringList>("pixmaps");

    QStringList unique;
    unique << "id0,64,64" << "id1,32,32" << "id2,16,16" << "id3,8,8" << "id4,4,4" << "id5,2,2" << "id6,1,1";
    QTest::newRow("unique") << unique;

    QStringList duplicates;
    duplicates << "id0,64,64" << "id0,64,64" << "id2,16,16" << "id2,16,16" << "id4,4,4" << "id4,4,4";
    QTest::newRow("duplicates") << duplicates;

}

void Ft_Theme::testPixmaps()
{
    QSKIP("Resource cleanup is temporary disabled...", SkipAll);
    // this keeps track what we have requested
    QSet<QString> unique;
    QList<const QPixmap *> allocated;

    // reset test theme daemon
    testDaemon->reset();

    // get test data
    QFETCH(QStringList, pixmaps);

    // allocate pixmap resources
    foreach(QString string, pixmaps) {

        // resolve parameters
        QStringList parameters = string.split(",");

        // request pixmap
        const QPixmap *pixmap = MTheme::pixmap(parameters[0], QSize(parameters[1].toInt(), parameters[2].toInt()));
        QVERIFY(pixmap != NULL);
        unique.insert(string);
        allocated.append(pixmap);
    }

    // verify that the pixmaps were requested only once if there are duplicates
    QCOMPARE(testDaemon->pixmapCount(), unique.count());

    foreach(const QPixmap * pixmap, allocated) {
        // release pixmap
        MTheme::releasePixmap(pixmap);
    }

    // FIXME: Skip until proper resource release.
    //MTheme::cleanupGarbage();
    // verify that all pixmaps were released
    //QCOMPARE(testDaemon->pixmapCount(), 0);
}

void Ft_Theme::testScalables_data()
{
    QTest::addColumn<QStringList>("scalables");

    QStringList unique;
    unique << "id0,0,0,0,0" << "id1,1,1,1,1" << "id2,2,2,2,2" << "id3,3,3,3,3" << "id4,4,4,4,4" << "id5,5,5,5,5" << "id6,6,6,6,6";
    QTest::newRow("unique") << unique;

    QStringList duplicates;
    duplicates << "id0,0,0,0,0" << "id0,0,0,0,0" << "id2,0,0,0,0" << "id2,0,0,0,0" << "id4,0,0,0,0" << "id4,0,0,0,0";
    QTest::newRow("duplicates") << duplicates;

}
void Ft_Theme::testScalables()
{
    // this keeps track what we have requested
    QSet<QString> unique;
    QSet<QString> uniquePixmaps;
    QList<const MScalableImage *> allocated;

    // reset test theme daemon
    testDaemon->reset();

    // get test data
    QFETCH(QStringList, scalables);

    // allocate scalable image resources
    foreach(QString string, scalables) {

        // resolve parameters
        QStringList parameters = string.split(",");

        // request scalable image
        const MScalableImage *scalable = MTheme::scalableImage(parameters[0], parameters[1].toInt(),
                                           parameters[2].toInt(), parameters[3].toInt(), parameters[4].toInt());

        QVERIFY(scalable != NULL);

        // check whether this is a duplicate, which is alredy requested
        if (unique.contains(string)) {
            // this should already exist in allocated list
            QVERIFY(allocated.contains(scalable));
        } else {
            unique.insert(string);
        }
        uniquePixmaps.insert(parameters[0]);
        allocated.append(scalable);
    }

    // FIXME: Skipping until proper resource releasing.
    // verify that the pixmaps were requested only once if there are duplicates
    //QCOMPARE(testDaemon->pixmapCount(), uniquePixmaps.count());

    foreach(const MScalableImage * scalable, allocated) {
        // release scalable image
        MTheme::releaseScalableImage(scalable);
    }

    //FIXME: Skipping until proper resource releasing.
    //MTheme::cleanupGarbage();
    // verify that all pixmaps were released
    //QCOMPARE(testDaemon->pixmapCount(), 0);
}

void Ft_Theme::testPalette_data()
{
    QTest::addColumn<QString>("theme");
    QTest::addColumn<QColor>("foregroundColor");
    QTest::addColumn<QColor>("secondaryForegroundColor");
    QTest::addColumn<QColor>("backgroundColor");
    QTest::addColumn<QColor>("invertedForegroundColor");
    QTest::addColumn<QColor>("invertedSecondaryForegroundColor");
    QTest::addColumn<QColor>("invertedBackgroundColor");
    QTest::addColumn<QColor>("selectionColor");
    QTest::addColumn<QColor>("warningColor");
    QTest::addColumn<QColor>("attentionColor");
    QTest::addColumn<QColor>("notificationColor");
    QTest::addColumn<QColor>("linkColor");
    QTest::addColumn<QColor>("activeLinkColor");
    QTest::addColumn<QColor>("firstAccentColor");
    QTest::addColumn<QColor>("secondAccentColor");
    QTest::addColumn<QColor>("thirdAccentColor");
    QTest::addColumn<QColor>("fourthAccentColor");
    QTest::addColumn<QColor>("fifthAccentColor");


    QTest::newRow("theme 1") << "theme 1" << QColor("#000000") << QColor("#666666") << QColor("#000000") <<
                             QColor("#ffffff") << QColor("#cccccc") << QColor("#FFFFFF") <<
                             QColor("#f5bf00") << QColor("#CC0000") << QColor("#CC9900") <<
                             QColor("#C3F500") << QColor("#3465a4") << QColor("#f5bf00") <<
                             QColor("#aad400") << QColor("#69bfde") << QColor("#ff9955") <<
                             QColor("#de87cd") << QColor("#d8b427");

    QTest::newRow("theme 2") << "theme 2" << QColor("#ffffff") << QColor("#666666") << QColor("#000000") <<
                             QColor("#ffffff") << QColor("#dddddd") << QColor("#FFFFFF") <<
                             QColor("#f5bf00") << QColor("#CC0000") << QColor("#CC9900") <<
                             QColor("#C3F500") << QColor("#3465a4") << QColor("#f5bf00") <<
                             QColor("#aad400") << QColor("#69bfde") << QColor("#ff9955") <<
                             QColor("#de87cd") << QColor("#d8b427");
}

void Ft_Theme::testPalette()
{
    QFETCH(QString, theme);
    testDaemon->changeTheme(theme);
    testDaemon->emitThemeChange();

    // create palette object
    const MPalette &palette = MTheme::palette();

    // check all values against palette
    QFETCH(QColor, foregroundColor);
    QFETCH(QColor, secondaryForegroundColor);
    QFETCH(QColor, backgroundColor);
    QFETCH(QColor, invertedForegroundColor);
    QFETCH(QColor, invertedSecondaryForegroundColor);
    QFETCH(QColor, invertedBackgroundColor);
    QFETCH(QColor, selectionColor);
    QFETCH(QColor, warningColor);
    QFETCH(QColor, attentionColor);
    QFETCH(QColor, notificationColor);
    QFETCH(QColor, linkColor);
    QFETCH(QColor, activeLinkColor);
    QFETCH(QColor, firstAccentColor);
    QFETCH(QColor, secondAccentColor);
    QFETCH(QColor, thirdAccentColor);
    QFETCH(QColor, fourthAccentColor);
    QFETCH(QColor, fifthAccentColor);

    QCOMPARE(palette.foregroundColor(), foregroundColor);
    QCOMPARE(palette.secondaryForegroundColor(), secondaryForegroundColor);
    QCOMPARE(palette.backgroundColor(), backgroundColor);
    QCOMPARE(palette.invertedForegroundColor(), invertedForegroundColor);
    QCOMPARE(palette.invertedSecondaryForegroundColor(), invertedSecondaryForegroundColor);
    QCOMPARE(palette.invertedBackgroundColor(), invertedBackgroundColor);
    QCOMPARE(palette.selectionColor(), selectionColor);
    QCOMPARE(palette.warningColor(), warningColor);
    QCOMPARE(palette.attentionColor(), attentionColor);
    QCOMPARE(palette.notificationColor(), notificationColor);
    QCOMPARE(palette.linkColor(), linkColor);
    QCOMPARE(palette.activeLinkColor(), activeLinkColor);
    QCOMPARE(palette.firstAccentColor(), firstAccentColor);
    QCOMPARE(palette.secondAccentColor(), secondAccentColor);
    QCOMPARE(palette.thirdAccentColor(), thirdAccentColor);
    QCOMPARE(palette.fourthAccentColor(), fourthAccentColor);
    QCOMPARE(palette.fifthAccentColor(), fifthAccentColor);
}

void Ft_Theme::testFonts_data()
{
    QTest::addColumn<QString>("theme");
    QTest::addColumn<QString>("language");
    QTest::addColumn<QFont>("extraLargeFont");
    QTest::addColumn<QFont>("largeFont");
    QTest::addColumn<QFont>("defaultFont");
    QTest::addColumn<QFont>("smallFont");
    QTest::addColumn<QFont>("extraSmallFont");

    QFont extraLargeFont("Arial");
    extraLargeFont.setPixelSize(32);
    QFont largeFont("Arial");
    largeFont.setPixelSize(28);
    QFont defaultFontTheme1("Arial");
    defaultFontTheme1.setPixelSize(24);
    QFont defaultFontTheme2("Courier");
    defaultFontTheme2.setPixelSize(24);
    QFont smallFont("Arial");
    smallFont.setPixelSize(20);
    QFont extraSmallFont("Arial");
    extraSmallFont.setPixelSize(16);

    QTest::newRow("theme 1, en") << "theme 1" << "en" << extraLargeFont << largeFont << defaultFontTheme1 << smallFont << extraSmallFont;

    QTest::newRow("theme 2, en") << "theme 2" << "en" << extraLargeFont << largeFont << defaultFontTheme2 << smallFont << extraSmallFont;

#ifdef HAVE_GCONF
    QTest::newRow("theme 1, fi") << "theme 1" << "fi" << extraLargeFont << largeFont << defaultFontTheme1 << smallFont << extraSmallFont;
#endif
}
void Ft_Theme::testFonts()
{
    QFETCH(QString, theme);
    QFETCH(QString, language);

    testDaemon->changeTheme(theme);

#ifdef HAVE_GCONF
    languageGConfValue = language;
#endif

    testDaemon->emitThemeChange();

    const MDefaultFonts &fonts = MTheme::fonts();

    QFETCH(QFont, extraLargeFont);
    QFETCH(QFont, largeFont);
    QFETCH(QFont, defaultFont);
    QFETCH(QFont, smallFont);
    QFETCH(QFont, extraSmallFont);

    QCOMPARE(fonts.extraLargeFont(), extraLargeFont);
    QCOMPARE(fonts.largeFont(), largeFont);
    QCOMPARE(fonts.defaultFont(), defaultFont);

    QCOMPARE(fonts.smallFont(), smallFont);
    QCOMPARE(fonts.extraSmallFont(), extraSmallFont);
}

QTEST_APPLESS_MAIN(Ft_Theme)
