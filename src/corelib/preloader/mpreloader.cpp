#include "mpreloader.h"

#include "msystemdirectories.h"

#include <QApplication>
#include <QDebug>
#include <QList>
#include <QFont>
#include <QFontMetrics>
#include <QSettings>
#include <QDir>

namespace {
bool populating = false;
}

MPreloader::MPreloader()
{
    populating = true;

    preloadFonts();
    preloadStyles();

    populating = false;
}

bool MPreloader::isPopulating()
{
    return populating;
}

// Loads fonts from a config file to fill Qts internal caches.
// The config file could look like this:
// [FontList]
// toLoad=SansSerif,SansSerifItalic
//
// [SansSerif]
// name=Sans Serif
// sizes=16,18,20,22,24,26,28,30,32,34,40,42,48,60,62
// weights=25,50,75
// styles=0
//
// [SansSerifItalic]
// name=Sans Serif
// sizes=12
// weights=50
// styles=1
void MPreloader::preloadFonts()
{
    QString configFile = MSystemDirectories::cacheDirectory() +
            QLatin1String("themedaemon/currentTheme/meegotouch/font-preloading.conf");

    if (!QFile::exists(configFile)) {
        return;
    }

    QApplication *app = 0;
    if (!qApp)
    {
        static int argc = 1;
        static char *argv[1] = { (char *) "mpreloader_initialization_application" };

        app = new QApplication(argc, argv);
    }

    QSettings settings(configFile, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup("FontList");
    QStringList fontGroups = settings.value(QLatin1String("toLoad")).toStringList();
    settings.endGroup();

    if (fontGroups.isEmpty()) {
        return;
    }

    foreach(const QString &fontGroup, fontGroups) {
        settings.beginGroup(fontGroup);
        QString name = settings.value(QLatin1String("name")).toString();
        if (name.isEmpty()) {
            qCritical() << "name must not be empty in group" << fontGroup;
            continue;
        }
        QStringList sizes = settings.value(QLatin1String("sizes")).toStringList();
        if (sizes.isEmpty()) {
            qCritical() << "sizes must not be empty in group" << fontGroup;
            continue;
        }
        QStringList weights = settings.value(QLatin1String("weights")).toStringList();
        if (weights.isEmpty()) {
            qCritical() << "weights must not be empty in group" << fontGroup;
            continue;
        }
        QStringList styles = settings.value(QLatin1String("styles")).toStringList();
        if (styles.isEmpty()) {
            qCritical() << "styles must not be empty in group" << fontGroup;
            continue;
        }

        foreach (const QString &size, sizes) {
            foreach (const QString &weight, weights) {
                foreach (const QString &style, styles) {
                    QString fontString = QString("%1,-1,%2,5,%3,%4,0,0,0,0").arg(name).arg(size).arg(weight).arg(style);
                    QFont font;
                    if (font.fromString(fontString)) {
                        // force Qt to fill its internal cache
                        QFontMetrics fm(font);
                        fm.width(QLatin1Char('x'));
                    }
                }
            }

        }
        settings.endGroup();
    }

    delete app;
}

void MPreloader::preloadStyles()
{
    //TODO: fill with life
}
